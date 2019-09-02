/*
 * filter.c

 *
 *  Created on: Feb 9, 2017
 *      Author: rpn314
 */
#include "filter.h"
#include <stdio.h>
#include <stdlib.h>

#define FILTER_FIR_DECIMATION_FACTOR 10         // Decimation factor (keep 1/10 values, throw away every 9/10 values)
#define FILTER_PLAYER_COUNT 10                  // Number of players
#define FILTER_IIR_FILTER_COUNT 10              // IIR Filter Count
#define IIR_A_COEFFICIENT_COUNT 11              // IIR A Coefficients Count
#define IIR_B_COEFFICIENT_COUNT 11              // IIR B Coefficients Count
#define FIR_B_COEFFICIENT_COUNT 81              // FIR B Coefficients Count
#define X_QUEUE_SIZE FIR_B_COEFFICIENT_COUNT    // X Queue Size (match FIR B Coefficient Count)
#define Y_QUEUE_SIZE IIR_B_COEFFICIENT_COUNT    // Y Queue Size (match IIR B Coefficient Count)
#define Z_QUEUE_SIZE IIR_B_COEFFICIENT_COUNT    // Z Queue Size (match IIR B Coefficient Count)
#define OUTPUT_QUEUE_SIZE 2001                  // Size of our output queue
#define NAME_SIZE 100                           // Size of our queue names (char array length)
#define FILTER_OUTPUT_QUEUE_OLDEST_VALUE 0      // Oldest Value location in output queue
#define FILTER_OUTPUT_QUEUE_NEWEST_VALUE 2000   // Newest Value location in output queue

static queue_t xQueue;                                  // Our xQueue (Coming in from ADC, going to FIR Filter)
static queue_t yQueue;                                  // Our yQueue (Coming from FIR Filter, going to IIR Filter)
static queue_t zQueues[FILTER_IIR_FILTER_COUNT];        // Our array of zQueues ("memory" queue for each IIR Filter)
static queue_t outputQueues[FILTER_IIR_FILTER_COUNT];   // Array of outputQueues (output queue for each IIR Filter)
static double currentPower[FILTER_PLAYER_COUNT] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};    // Initialize to all zeros WARNING: CODING STANDARD = Can we initialize like this?


const static double fir_b_coeffs[FIR_B_COEFFICIENT_COUNT] = // Giant array of the coefficients for our FIR filter
{0.0000000000000000e+00, 7.7726988732185190e-06, 1.6549385043846169e-05, -1.4934785088387650e-06, -7.4573044605558809e-05, -2.2063882621877793e-04, -4.3751137216076344e-04, -6.9638138993919371e-04,
        -9.4081112319670474e-04, -1.0924816020890023e-03, -1.0636813202362151e-03, -7.7513746377567885e-04, -1.7649612541968673e-04, 7.3418878851710457e-04, 1.8959897928012464e-03, 3.1794038832831106e-03,
        4.3921609445949561e-03, 5.2976898689737206e-03, 5.6453940210773436e-03, 5.2097875905206779e-03, 3.8336739292985020e-03, 1.4692307327772748e-03, -1.7896327674621702e-03, -5.6898103276985888e-03,
        -9.8211319890380913e-03, -1.3640496871030489e-02, -1.6517525161832809e-02, -1.7798173621044456e-02, -1.6879923360001162e-02, -1.3289978857315149e-02, -6.7567670223975945e-03, 2.7348791157516769e-03,
        1.4913066212098793e-02, 2.9218345965321687e-02, 4.4835538080991562e-02, 6.0756099794843026e-02, 7.5865394996250266e-02, 8.9045913086913633e-02, 9.9285214372410918e-02, 1.0577646288796948e-01,
        1.0800000000000000e-01, 1.0577646288796948e-01, 9.9285214372410918e-02, 8.9045913086913633e-02, 7.5865394996250266e-02, 6.0756099794843026e-02, 4.4835538080991562e-02, 2.9218345965321687e-02,
        1.4913066212098793e-02, 2.7348791157516769e-03, -6.7567670223975945e-03, -1.3289978857315149e-02, -1.6879923360001162e-02, -1.7798173621044456e-02, -1.6517525161832809e-02, -1.3640496871030489e-02,
        -9.8211319890380913e-03, -5.6898103276985888e-03, -1.7896327674621702e-03, 1.4692307327772748e-03, 3.8336739292985020e-03, 5.2097875905206779e-03, 5.6453940210773436e-03, 5.2976898689737206e-03,
        4.3921609445949561e-03, 3.1794038832831106e-03, 1.8959897928012464e-03, 7.3418878851710457e-04, -1.7649612541968673e-04, -7.7513746377567885e-04, -1.0636813202362151e-03, -1.0924816020890023e-03,
        -9.4081112319670474e-04, -6.9638138993919371e-04, -4.3751137216076344e-04, -2.2063882621877793e-04, -7.4573044605558809e-05, -1.4934785088387650e-06, 1.6549385043846169e-05, 7.7726988732185190e-06,
        0.0000000000000000e+00};

const static double iir_a_coeffs[FILTER_IIR_FILTER_COUNT][IIR_A_COEFFICIENT_COUNT] = // 2D array of the A coefficients of 10 bandpass filters
{{-5.9637727070164024e+00, 1.9125339333078266e+01, -4.0341474540744237e+01, 6.1537466875368963e+01, -7.0019717951472387e+01, 6.0298814235239078e+01, -3.8733792862566460e+01, 1.7993533279581150e+01, -5.4979061224867971e+00, 9.0332828533800180e-01},
        {-4.6377947119071461e+00, 1.3502215749461575e+01, -2.6155952405269765e+01, 3.8589668330738363e+01, -4.3038990303252646e+01, 3.7812927599537147e+01, -2.5113598088113797e+01, 1.2703182701888093e+01, -4.2755083391143494e+00, 9.0332828533800225e-01},
        {-3.0591317915750933e+00, 8.6417489609637492e+00, -1.4278790253808834e+01, 2.1302268283304286e+01, -2.2193853972079197e+01, 2.0873499791105406e+01, -1.3709764520609369e+01, 8.1303553577931531e+00, -2.8201643879900464e+00, 9.0332828533799847e-01},
        {-1.4071749185996754e+00, 5.6904141470697471e+00, -5.7374718273676226e+00, 1.1958028362868870e+01, -8.5435280598354364e+00, 1.1717345583835909e+01, -5.5088290876998371e+00, 5.3536787286077310e+00, -1.2972519209655506e+00, 9.0332828533799270e-01},
        {8.2010906117760285e-01, 5.1673756579268542e+00, 3.2580350909220863e+00, 1.0392903763919165e+01, 4.8101776408668915e+00, 1.0183724507092466e+01, 3.1282000712126603e+00, 4.8615933365571715e+00, 7.5604535083144453e-01, 9.0332828533799381e-01},
        {2.7080869856154473e+00, 7.8319071217995511e+00, 1.2201607990980699e+01, 1.8651500443681549e+01, 1.8758157568004453e+01, 1.8276088095998919e+01, 1.1715361303018824e+01, 7.3684394621253020e+00, 2.4965418284511718e+00, 9.0332828533799758e-01},
        {4.9479835250075892e+00, 1.4691607003177600e+01, 2.9082414772101053e+01, 4.3179839108869317e+01, 4.8440791644688865e+01, 4.2310703962394321e+01, 2.7923434247706417e+01, 1.3822186510471003e+01, 4.5614664160654321e+00, 9.0332828533799903e-01},
        {6.1701893352279864e+00, 2.0127225876810332e+01, 4.2974193398071669e+01, 6.5958045321253422e+01, 7.5230437667866568e+01, 6.4630411355739838e+01, 4.1261591079244113e+01, 1.8936128791950530e+01, 5.6881982915180274e+00, 9.0332828533799781e-01},
        {7.4092912870072469e+00, 2.6857944460290192e+01, 6.1578787811202446e+01, 9.8258255839887752e+01, 1.1359460153696361e+02, 9.6280452143026722e+01, 5.9124742025776840e+01, 2.5268527576524427e+01, 6.8305064480743756e+00, 9.0332828533801002e-01},
        {8.5743055776347692e+00, 3.4306584753117889e+01, 8.4035290411037067e+01, 1.3928510844056819e+02, 1.6305115418161625e+02, 1.3648147221895792e+02, 8.0686288623299774e+01, 3.2276361903872122e+01, 7.9045143816244750e+00, 9.0332828533799692e-01}};

const static double iir_b_coeffs[FILTER_IIR_FILTER_COUNT][IIR_B_COEFFICIENT_COUNT] = // 2D array of the A coefficients of 10 bandpass filters
{{9.0928662987777130e-10, -0.0000000000000000e+00, -4.5464331493888556e-09, -0.0000000000000000e+00, 9.0928662987777111e-09, -0.0000000000000000e+00, -9.0928662987777111e-09, -0.0000000000000000e+00, 4.5464331493888556e-09, -0.0000000000000000e+00, -9.0928662987777130e-10},
        {9.0928680767686741e-10, 0.0000000000000000e+00, -4.5464340383843370e-09, 0.0000000000000000e+00, 9.0928680767686739e-09, 0.0000000000000000e+00, -9.0928680767686739e-09, 0.0000000000000000e+00, 4.5464340383843370e-09, 0.0000000000000000e+00, -9.0928680767686741e-10},
        {9.0928682164807969e-10, 0.0000000000000000e+00, -4.5464341082403990e-09, 0.0000000000000000e+00, 9.0928682164807979e-09, 0.0000000000000000e+00, -9.0928682164807979e-09, 0.0000000000000000e+00, 4.5464341082403990e-09, 0.0000000000000000e+00, -9.0928682164807969e-10},
        {9.0928698538098572e-10, 0.0000000000000000e+00, -4.5464349269049290e-09, 0.0000000000000000e+00, 9.0928698538098580e-09, 0.0000000000000000e+00, -9.0928698538098580e-09, 0.0000000000000000e+00, 4.5464349269049290e-09, 0.0000000000000000e+00, -9.0928698538098572e-10},
        {9.0928656742804959e-10, 0.0000000000000000e+00, -4.5464328371402487e-09, 0.0000000000000000e+00, 9.0928656742804973e-09, 0.0000000000000000e+00, -9.0928656742804973e-09, 0.0000000000000000e+00, 4.5464328371402487e-09, 0.0000000000000000e+00, -9.0928656742804959e-10},
        {9.0928675409706938e-10, -0.0000000000000000e+00, -4.5464337704853462e-09, -0.0000000000000000e+00, 9.0928675409706924e-09, -0.0000000000000000e+00, -9.0928675409706924e-09, -0.0000000000000000e+00, 4.5464337704853462e-09, -0.0000000000000000e+00, -9.0928675409706938e-10},
        {9.0928341824833496e-10, -0.0000000000000000e+00, -4.5464170912416748e-09, -0.0000000000000000e+00, 9.0928341824833496e-09, -0.0000000000000000e+00, -9.0928341824833496e-09, -0.0000000000000000e+00, 4.5464170912416748e-09, -0.0000000000000000e+00, -9.0928341824833496e-10},
        {9.0929508683806034e-10, 0.0000000000000000e+00, -4.5464754341903013e-09, 0.0000000000000000e+00, 9.0929508683806025e-09, 0.0000000000000000e+00, -9.0929508683806025e-09, 0.0000000000000000e+00, 4.5464754341903013e-09, 0.0000000000000000e+00, -9.0929508683806034e-10},
        {9.0926202970187715e-10, 0.0000000000000000e+00, -4.5463101485093860e-09, 0.0000000000000000e+00, 9.0926202970187721e-09, 0.0000000000000000e+00, -9.0926202970187721e-09, 0.0000000000000000e+00, 4.5463101485093860e-09, 0.0000000000000000e+00, -9.0926202970187715e-10},
        {9.0904364435706839e-10, 0.0000000000000000e+00, -4.5452182217853412e-09, 0.0000000000000000e+00, 9.0904364435706824e-09, 0.0000000000000000e+00, -9.0904364435706824e-09, 0.0000000000000000e+00, 4.5452182217853412e-09, 0.0000000000000000e+00, -9.0904364435706839e-10}};


// Initialize 10 zQueues (for feedback/memory)
void initZQueues()
{
    char name[NAME_SIZE];                               // Char array for name of queue
    for (uint32_t i=0; i<FILTER_IIR_FILTER_COUNT; i++)  // Iterate through to create each IIR Player queue
    {
        sprintf(name,"zQueue%lu",i);                    // Each queue having a name helps in debugging
        queue_init(&(zQueues[i]), Z_QUEUE_SIZE, name);  // Initialize each queue
        filter_fillQueue(&(zQueues[i]), 0.0);           // Fill each queue with zeros to finish initializing
    }
}


// Initialize 10 OutputQueues
void initOutputQueues()
{
    char name[NAME_SIZE];                                       // Char array for name of queue
    for (uint32_t i=0; i<FILTER_IIR_FILTER_COUNT; i++)          // Iterate through to create each IIR Player queue
    {
        sprintf(name,"OutputQueue%lu",i);                       //Each queue having a name helps in debugging
        queue_init(&(outputQueues[i]), OUTPUT_QUEUE_SIZE, name);// Initialize each queue
        filter_fillQueue(&(outputQueues[i]), 0.0);              // Fill each queue with zeros to finish initializing
    }
}


// Initialize yQueue (Coming from FIR Filter, going to IIR Filter)
void initYQueue()
{
    queue_init(&(yQueue), Y_QUEUE_SIZE, "yQueue");  // Initialize the queue
    filter_fillQueue(&(yQueue), 0.0);               // Fill queue with zeros to finish initializing
}


// Initialize xQueue (Coming in from ADC, going to FIR Filter)
void initXQueue()
{
    queue_init(&(xQueue), X_QUEUE_SIZE, "xQueue");
    filter_fillQueue(&(xQueue), 0.0);                           // Initializing means filling with zeros
}


// Initialize all filters. Must call this prior to using any filter functions.
void filter_init()
{
    initXQueue();       // Initialize X Queue & fill with zeros
    initYQueue();       // Initialize Y Queue & fill with zeros
    initZQueues();      // Initialize All 10 zQueues & fill with zeros
    initOutputQueues(); // Initialize All 10 OutputQueues & fill with zeros
}


// Use this to copy an input into the input queue of the FIR-filter (xQueue).
void filter_addNewInput(double x)
{
    queue_overwritePush(&(xQueue), x);  // Overwrite push value onto xQueue
}


// Fills a queue with the given fillValue.
void filter_fillQueue(queue_t* q, double fillValue)
{
    for(uint32_t i=0; i<q->size; i++)       // Iterate through the length of the queue
    {
        queue_overwritePush(q, fillValue);  // Overwrite push the value into every location in the given queue
    }
}


// Invokes the FIR-filter. Input is contents of xQueue.
// Output is returned and is also pushed on to yQueue.
double filter_firFilter()
{
    double newY = 0;
    for(uint32_t xLoc=0; xLoc<X_QUEUE_SIZE; xLoc++) //This is a convolution:
    {
        double elementValue = queue_readElementAt(&(xQueue),X_QUEUE_SIZE-xLoc-1);
        newY += elementValue * fir_b_coeffs[xLoc];//Flip signal and multiply by a coefficient for a given time value
    }
    queue_overwritePush(&(yQueue), newY); // After going through the FIR filter, values get placed in the yQueue
    return newY;
}


// Use this to invoke a single iir filter. Input comes from yQueue.
// Output is returned and is also pushed onto zQueue[filterNumber].
double filter_iirFilter(uint16_t filterNumber)
{
    double newOutput = 0;
    for(uint32_t i=0; i<Y_QUEUE_SIZE; i++)      // Iterate through each location in the yQueue to bring in the new data
    {                                               //This is a convolution:
        queue_data_t elementValue = queue_readElementAt(&(yQueue),Y_QUEUE_SIZE-i-1);    // Get the element from the given filter at the a particular location
        newOutput += elementValue * iir_b_coeffs[filterNumber][i]; //Flip signal and multiply by a coefficient for a given time value
    }

    for(uint32_t i=0; i<Z_QUEUE_SIZE; i++)  // Iterate through each location in the zQueue to add in the feedback/memory aspect of the IIR filter
    {
        queue_data_t elementValue = queue_readElementAt(&(zQueues[filterNumber]),Z_QUEUE_SIZE-i-1); // Get the element from the given filter at the a particular location
        newOutput -= elementValue * iir_a_coeffs[filterNumber][i]; //Flip signal and multiply by a coefficient for a given time value
    }

    queue_overwritePush(&(outputQueues[filterNumber]),newOutput);   //outputQueues saves previous 2000 values for power
    queue_overwritePush(&(zQueues[filterNumber]),newOutput);        //qQueues saves the previous 11 values for feedback
    return newOutput;
}


// Use this to compute the power for values contained in an outputQueue.
// If force == true, then recompute power by using all values in the outputQueue.
// This option is necessary so that you can correctly compute power values the first time.
// After that, you can incrementally compute power values by:
// 1. Keeping track of the power computed in a previous run, call this prev-power.
// 2. Keeping track of the oldest outputQueue value used in a previous run, call this oldest-value.
// 3. Get the newest value from the power queue, call this newest-value.
// 4. Compute new power as: prev-power - (oldest-value * oldest-value) + (newest-value * newest-value).
// Note that this function will probably need an array to keep track of these values for each
// of the 10 output queues.
double filter_computePower(uint16_t filterNumber, bool forceComputeFromScratch, bool debugPrint)
{
    if (filterNumber >= FILTER_PLAYER_COUNT)    // If the given filter number is greater than the player count, we're out of range
    {
        printf("filter_computePower: filterNumber Out of Range: %u\n\r", filterNumber);    // Print our Error Message
        return 0;
    }
    // If we need compute from scratch
    if(forceComputeFromScratch){
        // Iterate through each output queue
        for(uint32_t i=1; i<OUTPUT_QUEUE_SIZE; i++)
        {
            double currentValue = (queue_readElementAt(&(outputQueues[filterNumber]),i));//Gets a specific value of the output queue
            currentPower[filterNumber] += currentValue*currentValue;//Implements the sum of squares to find the power
        }
        if(debugPrint) // helps with debugging purposes
        {
            printf("filterComputePower, forced: Current Power: ");
            for (uint16_t i=0; i<FILTER_PLAYER_COUNT; i++)  // Iterate through each location in the currentPower queue
            {
                printf("%u:%lf, ",i,currentPower[i]);       //Print out each of the power values
            }
            printf("\n\r");
        }
    }
    else
    {
        double oldValue = queue_readElementAt(&(outputQueues[filterNumber]),FILTER_OUTPUT_QUEUE_OLDEST_VALUE);//Keeps track of the oldest value
        double newValue = queue_readElementAt(&(outputQueues[filterNumber]),FILTER_OUTPUT_QUEUE_NEWEST_VALUE);//Keeps track of the newest value
        currentPower[filterNumber] += newValue*newValue - oldValue*oldValue;//To save on computations, add in the new squared value and subtract out the oldest squared value
    }
    return currentPower[filterNumber];
}


// Returns the last-computed output power value for the IIR filter [filterNumber].
double filter_getCurrentPowerValue(uint16_t filterNumber)
{
    return currentPower[filterNumber];
}


// Get a copy of the current power values.
// This function copies the already computed values into a previously-declared array
// so that they can be accessed from outside the filter software by the detector.
// Remember that when you pass an array into a C function, changes to the array within
// that function are reflected in the returned array.
void filter_getCurrentPowerValues(double powerValues[])
{ //Getter function to give values of power without modifying it.
    for(uint32_t i=0; i<FILTER_PLAYER_COUNT; i++)                   // Iterate through each location in the currentPower queue
    {
        powerValues[i] = currentPower[i];                           // Copy the value in our power queue to the given array
    }
    return;
}


// Using the previously-computed power values that are current stored in currentPowerValue[] array,
// Copy these values into the normalizedArray[] argument and then normalize them by dividing
// all of the values in normalizedArray by the maximum power value contained in currentPowerValue[].
void filter_getNormalizedPowerValues(double normalizedArray[], uint16_t* indexOfMaxValue)
{
    *indexOfMaxValue = 0;
    for(uint32_t i=0; i<FILTER_PLAYER_COUNT; i++)               // Iterate through each location in the currentPower Array
    {
        if(currentPower[i]>currentPower[*indexOfMaxValue])      // If the current value is greater than the Max value in the currentPower array
        {
            *indexOfMaxValue = i;                               // Update the currentPower array index of the max value
        }
    }
    filter_getCurrentPowerValues(normalizedArray);              // Copy all of the current power values into the normalizedArray
    for(uint32_t i=0; i<FILTER_PLAYER_COUNT; i++)               // Iterate through each location in the normalizedArray
    {
        //To normalize, take every value and divide it by the maximum value in the Power array
        normalizedArray[i] = normalizedArray[i]/currentPower[*indexOfMaxValue];
    }
    return;
}

/*********************************************************************************************************
 ********************************** Verification-assisting functions. *************************************
 ********* Test functions access the internal data structures of the filter.c via these functions. ********
 *********************** These functions are not used by the main filter functions. ***********************
 **********************************************************************************************************/

// Returns the array of FIR coefficients.
const double* filter_getFirCoefficientArray()
{
    return fir_b_coeffs;
}


// Returns the number of FIR coefficients.
uint32_t filter_getFirCoefficientCount()
{
    return FIR_B_COEFFICIENT_COUNT;
}


// Returns the array of coefficients for a particular filter number.
const double* filter_getIirACoefficientArray(uint16_t filterNumber)
{
    return iir_a_coeffs[filterNumber];
}


// Returns the number of A coefficients.
uint32_t filter_getIirACoefficientCount()
{
    return IIR_A_COEFFICIENT_COUNT;
}


// Returns the array of coefficients for a particular filter number.
const double* filter_getIirBCoefficientArray(uint16_t filterNumber)
{
    return iir_b_coeffs[filterNumber];
}


// Returns the number of B coefficients.
uint32_t filter_getIirBCoefficientCount()
{
    return IIR_B_COEFFICIENT_COUNT;
}


// Returns the size of the yQueue.
uint32_t filter_getYQueueSize()
{
    return Y_QUEUE_SIZE;
}


// Returns the decimation value.
uint16_t filter_getDecimationValue()
{
    return FILTER_FIR_DECIMATION_FACTOR;
}


// Returns the address of xQueue.
queue_t* filter_getXQueue()
{
    return &(xQueue);
}


// Returns the address of yQueue.
queue_t* filter_getYQueue()
{
    return &(yQueue);
}


// Returns the address of zQueue for a specific filter number.
queue_t* filter_getZQueue(uint16_t filterNumber)
{
    return &(zQueues[filterNumber]);
}


// Returns the address of the IIR output-queue for a specific filter-number.
queue_t* filter_getIirOutputQueue(uint16_t filterNumber)
{
    return &(outputQueues[filterNumber]);
}

// Function to manually set the power values array, to be able to test the hit detector
void filter_setCurrentPowerValues(double powerValues[])
{ //Getter function to give values of power without modifying it.
    for(uint32_t i=0; i<FILTER_PLAYER_COUNT; i++)                   // Iterate through each location in the currentPower queue
    {
        currentPower[i] = powerValues[i];                           // Copy the value in the given array to our power queue
    }
    return;
}

// Returns the address of the firOutputDebugQueue.
//queue_t* filter_getFirOutputDebugQueue();

//void filter_runTest();
