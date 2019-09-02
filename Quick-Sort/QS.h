/*
 * QS.h
 *
 *  Created on: Nov 25, 2014
 *      Author: rdmclean
 */

#ifndef QS_H_
#define QS_H_
#include "QSInterface.h"
#include <sstream>

class QS : public QSInterface
{
	public:
		QS()
		{
			myArray = NULL;
			index = 0;
			Arraysize = 0;
		}
		~QS(){clear();}

		/*
		 * sortAll()
		 *
		 * Sorts elements of the array.  After this method is called, every
		 * element in the array is less than or equal to the following element.
		 *
		 * Does nothing if the array is empty.
		 */
		void sortAll();

		/*
		 * medianOfThree()
		 *
		 * Performs median-of-three pivot selection from among the values in
		 * the array between the two given indices. Median-of-three pivot
		 * selection will sort the first, middle, and last elements in a given
		 * array with respect to each other. After this method is called,
		 * data[first] <= data[middle] <= data[last], where middle =
		 * (first + last)/2.
		 *
		 * Returns -1 if the array is empty, if either of the given integers
		 * is out of bounds, or if the first integer is not less than the second
		 * integer.
		 *
		 * @param left
		 * 		the left boundary for the subarray from which to find a pivot
		 * @param right
		 * 		the right boundary for the subarray from which to find a pivot
		 * @return
		 *		the index of the pivot; -1 if provided with invalid input
		 */
		int medianOfThree(int left, int right);

		/*
		 * Partitions a subarray around a pivot value selected according to
		 * median-of-three pivot selection.
		 *
		 * The values which are smaller than the pivot should be placed to the left
		 * of the pivot; the values which are larger than the pivot should be placed
		 * to the right of the pivot.
		 *
		 * Does nothing and returns -1 if the array is null, if either of the
		 * given integers is out of bounds, or if the first integer is not less than
		 * the second integer, or if the pivot is not between the two boundaries.
		 *
		 * @param left
		 * 		the left boundary for the subarray to partition
		 * @param right
		 * 		the right boundary for the subarray to partition
		 * @param pivotIndex
		 * 		the index of the pivot in the subarray
		 * @return
		 *		the pivot's ending index after the partition completes; -1 if
		 * 		provided with bad input
		 */
		int partition(int left, int right, int pivotIndex);

		/*
		 * Gets the array of values and puts them into a string. For example: if my array 			 * looked like {5,7,2,9,0}, then the string to be returned would look like "5,7,2,9,0" 			 * with no trailing comma.
		 *
		 * Does nothing and returns an empty string, if the array is null or empty.
		 *
		 * @return
		 *		the string representation of the current array
		 */
		string getArray();

		/*
		 * Gets the current size of the current array.
		 *
		 * @return
		 * 		the current size
		 */
		int getSize();

		/*
		 * Adds the given value to the array.
		 */
		void addToArray(int value);

		/*
		 * Creates an array of with the given size.
		 *
		 * Returns false if the given value is non-positive, true otherwise.
		 *
		 * @param
		 *		size of array
		 * @return
		 *		true if the array was created, false otherwise
		 */
		bool createArray(int size);

		/*
		 * Clears the array.
		 */
		void clear();

		/*
		 * Prints the Array
		 */
		void print();

		/*
		 * Switches the two elements which are passed in
		 * The elements are indexes
		 */
		void swap(int ele1, int ele2);
		
		/*
		 * The Recursion Function
		 */
		void recurse(int left, int right);

		bool compare();


	private:
		int* myArray;
	//	int* myArray = new int[15];
		int index; // SCARY INITIALIZATION
		int Arraysize; // SCARY INITIALIZATION
};





#endif /* QS_H_ */
