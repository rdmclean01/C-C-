/*
 * detector.c
 *
 *  Created on: Mar 9, 2017
 *      Author: rpn314
 */

#include "detector.h"               // Detector header file (it's own header file)
#include "filter.h"                 // Use the filters!
#include <stdbool.h>                // Standard boolean support
#include <stdint.h>                 // Standard int support
#include <stdio.h>                  // Standard I/O (for printf functions)
#include "isr.h"                    // Access the ADC to get its count and remove values from the buffer
#include "lockoutTimer.h"           // Lockout timer to not allow multiple hits in short succession
#include "hitLedTimer.h"            // Turn on the Hit LED when a hit is detected
#include "supportFiles/switches.h"  // Read the switches to determine what our own frequency is
#include "supportFiles/interrupts.h"// Disable interrupts before removing values from adc queue


#define DETECTOR_ADC_MAPPING_FACTOR 2048.0      // Value to map ADC values (from 0 to 4095) to -1.0 to 1.0, with 2048 being 0
#define FORCE_COMPUTE_FROM_SCRATCH true         // Tell filter_computePower() to compute from scratch
#define NO_FORCE_COMPUTE_FROM_SCRATCH false     // Tell filter_computePower() to not compute from scratch
#define NO_DEBUG_PRINT false                    // Tell filter_computePower() to not print out debug statements
#define MEDIAN_INDEX FILTER_FREQUENCY_COUNT/2   // Index at the middle of the array
#define MAX_INDEX (FILTER_FREQUENCY_COUNT-1)    // Index of last array location (length - 1)
#define ELEMENT_COUNTER_RESET 1                 // Reset value for the element counter.
#define FUDGE_FACTOR 10                         // Our fudge factor. Will probably change when we use the guns

static bool detector_hitDetectedFlag;           // Global flag to indicate that a hit was detected
static uint16_t hits[FILTER_FREQUENCY_COUNT];   // Keep track of hit count for each player frequency

// Structure to keep track of power values and their original indices (player/frequency number)
typedef struct {
    double value;   // stored value
    uint16_t index; // stored index
} power_Value_t;

// Returns the current switch-setting
uint16_t detector_getFrequencySetting() {
    uint16_t switchSetting = switches_read() & 0xF;  // Bit-mask the results.
    // Provide a nice default if the slide switches are in error.
    if (!(switchSetting < FILTER_FREQUENCY_COUNT))
        return FILTER_FREQUENCY_COUNT - 1;
    else
        return switchSetting;
}

// Helper function to detect hits. Runs sorting algorithm
bool detector_hitDetector(bool ignoreSelf)
{
    double powerValues[FILTER_FREQUENCY_COUNT];             // Create an array to store the power values from the filter
    filter_getCurrentPowerValues(powerValues);              // Get the current power values
    power_Value_t sortedPowerValues[FILTER_FREQUENCY_COUNT];// Array of power value structs (value and their original location)

    // Copy power values into power values array w/ original indices
    for (uint16_t filter = 0; filter < FILTER_FREQUENCY_COUNT; filter++)    // Iterate through all of the player frequency filter
    {
        sortedPowerValues[filter].value = powerValues[filter]; // Copy the value to the value location
        sortedPowerValues[filter].index = filter;              // Get the index of the power value
    }

    // Sort power values array. Selection Sort Algorithm
    for (uint16_t filterNumber = 0; filterNumber < FILTER_FREQUENCY_COUNT - 1; filterNumber++)  // Iterate through the array
    {
        uint16_t lowestIndex = filterNumber;    // Initial lowest index (assume it's the place we're currently at)
        // Iterate through the remaining unsorted parts of the array, looking for a lower value than where we are.
        for (uint16_t subFilterNumber = filterNumber+1; subFilterNumber < FILTER_FREQUENCY_COUNT; subFilterNumber++)
        {
            if (sortedPowerValues[subFilterNumber].value < sortedPowerValues[lowestIndex].value)    // If the value in the array is lower than the current lowest value
                lowestIndex = subFilterNumber;    // Update lowest index, corresponding to the lowest value of the unsorted portion
        }
        if (lowestIndex != filterNumber)    // If the lowest index is not the current index (i.e. the index changed)
        {
            power_Value_t tempPowerValue = sortedPowerValues[lowestIndex];      // Save a temporary copy of the lowest of the unsorted parts of the array
            sortedPowerValues[lowestIndex] = sortedPowerValues[filterNumber];   // Copy the power value in the filter location to where the lowest index was
            sortedPowerValues[filterNumber] = tempPowerValue;                   // Copy the temporary copy of the lowest into where the filter is
        }
    }

    power_Value_t medianPower = sortedPowerValues[MEDIAN_INDEX];// Get the median value
    power_Value_t maxPower = sortedPowerValues[MAX_INDEX];      // Get the max value

    if (maxPower.value > (medianPower.value * FUDGE_FACTOR))    // If the max power value is greater than the median power value multiplied by the fudge factor
    {
        if (ignoreSelf)    // If ignore self is true (friendly fire off)
        {
            uint16_t switchValue = detector_getFrequencySetting();          // Compute a safe number from the switches.
            if (switchValue != maxPower.index)                              // If the hit value is not our switch value
            {
                detector_hitDetectedFlag  = true;   // Hit was detected
                hits[maxPower.index]++;             // Increment the hit count for the dominating player number (given by the maxPower original index number)
            }
            // else do nothing. We are ignoring friendly fire
        }
        else
        {
            detector_hitDetectedFlag  = true;   // Hit was detected, set the flag
            hits[maxPower.index]++;             // Increment the hit count for the dominating player number
        }
    }
    return detector_hitDetectedFlag ;   // Return the hit detector flag
}

// Always have to init things.
void detector_init()
{
    detector_hitDetectedFlag  = false;  // Set the hit detected flag to false initially
}

// Runs the entire detector: decimating fir-filter, iir-filters, power-computation, hit-detection.
// if interruptsEnabled = false, interrupts are not running. If interruptsEnabled = false
// you can pop values from the ADC queue without disabling interrupts.
// If interruptsEnabled = true, do the following:
// 1. disable interrupts.
// 2. pop the value from the ADC queue.
// 3. re-enable interrupts.
// Use this to determine whether you should disable and re-enable interrupts when accessing the adc queue.
// if ignoreSelf == true, ignore hits that are detected on your frequency.
// Your frequency is simply the frequency indicated by the slide switches.
void detector(bool interruptsEnabled, bool ignoreSelf)
{
    static bool firstPass = true;   // If this is the first pass, force the compute power from scratch
    static uint32_t counter = ELEMENT_COUNTER_RESET;// Create and set/reset counter to count by the decimation factor

    uint32_t elementCount = isr_adcBufferElementCount();    // Get the number of elements in the ADC buffah
    for (uint32_t i = 0; i < elementCount; i++) // Iterate through each element in the ADC buffer
    {
        uint32_t rawAdcValue = 0;
        if (interruptsEnabled)  // If interrupts are enabled, we need to stop them, grab the next value in the buffer, and then re-enable them.
        {
            interrupts_disableArmInts();                // Stop interrupts.
            rawAdcValue = isr_removeDataFromAdcBuffer();// Get the value from the ADC
            interrupts_enableArmInts();                 // Start Interrupts again
        }
        else    // If the interrupts are disabled, we don't need to worry about them
        {
            rawAdcValue = isr_removeDataFromAdcBuffer();   // Get the value from the ADC
        }

        double scaledAdcValue = (((double)rawAdcValue) - DETECTOR_ADC_MAPPING_FACTOR) / DETECTOR_ADC_MAPPING_FACTOR;  // Scale raw ADC value to be between -1.0 and 1.0
        filter_addNewInput(scaledAdcValue);             // Add most recent ADC value to the xQueue

        if (counter++ == FILTER_FIR_DECIMATION_FACTOR)   // If we are a multiple of 10 in the loop, decimate
        {
            filter_firFilter();                         // Run the FIR Filter
            for (uint16_t filter = 0; filter < FILTER_FREQUENCY_COUNT; filter++)    // Iterate through all of the player frequency filter
            {
                filter_iirFilter(filter);   // Run the IIR filter for the current filter number
                if(firstPass)   // If this is the first pass, we need to compute the power from scratch
                {
                    filter_computePower(filter, FORCE_COMPUTE_FROM_SCRATCH, NO_DEBUG_PRINT); // Compute from scratch the first time
                    firstPass = false;  // Set the first pass to false now that we've hit the first pass
                }
                else    // If this is not the first pass
                    filter_computePower(filter, NO_FORCE_COMPUTE_FROM_SCRATCH, NO_DEBUG_PRINT);// Simply compute the updated power value
            }
            if (!lockoutTimer_running()) // Checks if lockoutTimer is not (done or never was) running
            {
                if (detector_hitDetector(ignoreSelf))   // If lockout timer is done running, checks for a detected hit again
                {
                    lockoutTimer_start();   // Start lockout timer
                    hitLedTimer_start();    // Start Hit LED Timer
                }
            }
            counter = ELEMENT_COUNTER_RESET;    // Reset counter
        }
    }

}

// Returns true if a hit was detected.
bool detector_hitDetected()
{
    return detector_hitDetectedFlag ;   // Return the hit detected flag
}

// Clear the detected hit once you have accounted for it.
void detector_clearHit()
{
    detector_hitDetectedFlag  = false;  // Set the hit detected to false
}

// Get the current hit counts.
// Copy the current hit counts into the user-provided hitArray
// using a for-loop.
void detector_getHitCounts(detector_hitCount_t hitArray[])
{
    for (uint16_t filter = 0; filter < FILTER_FREQUENCY_COUNT; filter++)    // Iterate through all of the player frequency filter
    {
        hitArray[filter] = hits[filter];    // copy the hits from our internal hit array into the hit array sent into the function
    }
}

// Test function
// 1. Create a first set of data and provide a fudge-factor value that will detect a hit when you invoke detector().
//      After running detector(), you would invoke detector_hitDetected() to determine if a hit occurred.
// 2. Create a second set of data that will not detect a hit when you invoke detector().
//      Use the same fudge-factor as you used for the first set of data. Again, invoke detector_hitDetected() to determine if a hit occurred.
void detector_runTest()
{
    // Will detect hit
    // Data Set that will have a hit.
    double powerValuesHit[FILTER_FREQUENCY_COUNT] = {30000.0, 10000.0, 10.0, 20.0, 34.0, 15.0, 13.0, 19.0, 30.0, 80.0};
    filter_setCurrentPowerValues(powerValuesHit); // Set the power values in filter.
    printf("detector_runTest(): Hit should be detected.....");
    detector_hitDetector(false);    // Run hit detector (run with "ignore self" off)
    if (detector_hitDetected())     // If the hit detector returns a hit
        printf("Hit was detected\n\r");
    else
        printf("Hit was not detected\n\r");


    // Will not detect hit
    // Data Set that will not have a hit detected
    double powerValuesNoHit[FILTER_FREQUENCY_COUNT] = {200.0, 100.0, 10.0, 20.0, 34.0, 15.0, 13.0, 19.0, 30.0, 80.0};
    filter_setCurrentPowerValues(powerValuesNoHit);  // Set the power values in filter
    printf("detector_runTest(): Hit should not be detected.....");
    detector_hitDetector(false);    // Run hit detector (run with "ignore self" off)
    if (detector_hitDetected())     // If the hit detector returns a hit
        printf("Hit was detected\n\r");
    else
        printf("Hit was not detected\n\r");
}
