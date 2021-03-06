#ifndef __SORTS_H__
#define __SORTS_H__

#include "err.h"
#include "vec.h"





/* User-supplied comparator function
 * Compare two data items
 *   returns 0 - if the items are equal
 *   positive  - _data1 > _data2
 *   negative  - _data1 < _data2
 */
typedef int(*CompFunc)(void* _item1 , void* _item2);



/* User-supplied key getter
 * Extract an integer key value from data item
 * used by counting sort 
 */
typedef int (*GetKey) (void* _data);





/* User-supplied key getter
 * Extract an integer key value from _digitNum digit of the number data item
 *   154674,2 => 7  (second digit from right)
 * used by radix sort 
 */
typedef int (*GetKeyForRadix) (void* _data, int _digitNum);



/* pointer to sort function*/
typedef ADTERR(*SortFunc)(Vector* _vec , CompFunc _compFunc);




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
ADTERR BubbleSort (Vector* _vec, CompFunc _compFunc);                          /*************1*************/



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
ADTERR ShakeSort (Vector* _vec, CompFunc _compFunc);                            /*************2*************/



/**
 *  Insertion Sort - a comparison sort in which the sorted array (or list)
 * is built one entry at a time.
 * while next item from vec is smaller then previous item at arr => move the hole "down" by advancing items in arr
 * Insertion sort is better than bubble sort because if given a almost sorted array
 * it will perform much less operations than bubble sort:
 * e.g. {2,3,4,5,1} - bubble sort (5 comparison) Insertion Sort (only one)
 * Complexity:
 * for sorted array - O(n)
 * for random array - O(n^2)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTERR InsertionSort (Vector* _vec, CompFunc _compFunc);                       /*************3*************/



/**
 *  Quicksort - first divides a large list into two smaller sub-lists:
 * the low elements and the high elements.
 * Quicksort can then recursively sort the sub-lists.
 * for every sublist - put the pivot on the right 
 * then go through the sub list from left to right and move to left index (++every swap) every item that is smaller then the pivot 
 * at the end puts the pivot in his right location (left index that we incremented in every	swap)
 * 5,4,3,2,1 : pivot 3 to right -> 5,4,2,1,3 : (5<->2) -> 2,4,5,1,3 : (4<->1) -> 2,1,5,4,3 (5<->3) -> 2,1,3,4,5 -> now pivot in place
 * 2,1 : left part : pivot 2 -> 1,2
 * 4,5 : right part : pivot 4 -> all good	
 * Complexity:
 * for sorted array - O(n)
 * for random array - O(n logn)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTERR QuickSort  (Vector* _vec, CompFunc _comp);                                 /**************4************/



/** 
 *   Merge Sort - Divide the unsorted list into two sublists, and so on
 *  recursively until each part contain 1 item only.
 *  then merge the two sublists back into one sorted list
 * the sorting is done at the merging stage with 2 indexes on the right side and the left 	
 * 
 * Time  complexity - O(n logn)
 * Space complexity - O(n)
 * 
 * Arguments:
 * 		_vec	ADT Vector to be sorted
 * 		_comp	Data item comparator
 */
ADTERR MergeSort (Vector* _vec, CompFunc _compFunc);                               /*************5*************/



/** Counting Sort - a collection of objects according to keys that are
 * small integers int the range (0..maxValue)
 * 
 * Time Complexity  - O(n+k)
 * Space Complexity - O(n+k) 
 * 
 * Arguments:
 * 		_vec		    ADT Vector to be sorted
 * 		_keyGet		    Key extractor function
 * 		_numOfValues	data item range = k  (range of differant values)
 */
ADTERR CountingSort (Vector* _vec, GetKey _keyGet, int _numOfValues);               /**************6************/



/** Radix Sort - sort data with integer keys by splitting the key into
 * individual digits (in the given Radix) 
 * 
 * Time Complexity  - O(n*k)  : k = (radix)
 * Space Complexity - O(n+k) 
 * 
 * Arguments:
 * 		_vec		         ADT Vector to be sorted
 * 		_keyGet				 Key extractor function
 *      _getKeyForRadixFunc  user supplied function that get an item and digit num and calculates the key of the item by this digit  
 *      _length(radix)     	 number of digits/chars to each item            - how many digits
 *		_numOfValues	data item range = k  (range of differant values)    - range of each digit 
 */
ADTERR RadixSort (Vector* _vec, GetKeyForRadix _getKeyForRadixFunc, int _length , int _numOfValues);




#endif /* #ifndef __VEC_H__ */
