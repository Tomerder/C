/**
 * Author:	Klara Levi
 * Creation date: 21.05.11
 * Last Modification Date: 05.12.13		by Alex Katz: adhere to -pedantic, streamline signatures and comments
 * Desctiption: Sort Algorithms over Vector
 */
#ifndef _SORTUNG_H_
#define _SORTUNG_H_

#include "Vector.h"

/* User-supplied comparator function
 * Compare two data items
 *   returns 0 - if the items are equal
 *   positive  - _data1 > _data2
 *   negative  - _data1 < _data2
 */
typedef int (*CompFunc) (void* _data1, void* _data2);

/* User-supplied key getter
 * Extract an integer key value from data item
 */
typedef int (*GetKey) (void* _data);

/**
 *  Bubble Sort - works by repeatedly stepping through the list to be sorted,
 * comparing each pair of adjacent items and swapping them if they are in the wrong order.
 * Complexity: 
 * 	for sorted array - O(n)
 *  for random array - O(n^2)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTerr BubbleSort (Vector* _vec, CompFunc _comp);

/**
 *  Shake Sort - The algorithm differs from bubble sort in that it sorts
 * in both directions on each pass through the list. Shaking sort is better
 * than babble sort, due to ability to move objects backwords.
 * Shake sort is on average 2 times better than bubble sort --
 * e.g. {5,4,3,2,1} - bubble sort (4 passes) shake sort (only 2)
 * Complexity:
 * for sorted array - O(n)
 * for random array - O(n^2)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTerr ShakeSort  (Vector* _vec, CompFunc _comp);

/**
 *  Insertion Sort - a comparison sort in which the sorted array (or list)
 * is built one entry at a time.
 * Insertion sort is better than bubble sort becouse if given a almost sorted array
 * it will perform much less operations than bubble sort:
 * e.g. {2,3,4,5,1} - bubble sort (5 comparison) shake sort (only one)
 * Complexity:
 * for sorted array - O(n)
 * for random array - O(n^2)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTerr InsertionSort (Vector* _vec, CompFunc _comp);


/**
 *  Quicksort - first divides a large list into two smaller sub-lists:
 * the low elements and the high elements.
 * Quicksort can then recursively sort the sub-lists.
 * Complexity:
 * for sorted array - O(n)
 * for random array - O(n logn)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTerr QuickSort  (Vector* _vec, CompFunc _comp);

/** 
 *   Merge Sort - Divide the unsorted list into two sublists, sort each sublist
 *  recursively and merge the two sublists back into one sorted list
 * 
 * Time  complexity - O(n logn)
 * Space complexity - O(n)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTerr MergeSort (Vector* _vec, CompFunc _comp);

/** Counting Sort - a collection of objects according to keys that are
 * small integers int the range 0..maxValue
 * 
 * Time Complexity  - O(?)
 * Space Complexity - O(?) 
 * 
 * Arguments:
 * 		_vec		ADT Vector to be sorted
 * 		_keyGet		Key extractor function
 * 		_maxValue	data item range
 */
ADTerr_t CountingSort (Vector* _vec, GetKey _keyGet, int _maxValue);

/** Radix Sort - sort data with integer keys by splitting the key into
 * individual digits (in the given Radix) 
 * 
 * Time Complexity  - O(n)
 * Space Complexity - O(n) 
 * 
 * Arguments:
 * 		_vec		ADT Vector to be sorted
 * 		_keyGet		Key extractor function
 * 		_radix		the radix of each digit
 */
ADTerr RadixSort (Vector* _vec, GetKey _func, int _radix);

#endif /* SORTING_H_ */















