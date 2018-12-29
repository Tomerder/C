/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-29    
    Last modified date:		2013-07-29
    Description: generic heap - test functions
***************************************************************************/

#ifndef __HEAP_GENERIC_TEST_H__
#define __HEAP_GENERIC_TEST_H__

/* build & destroy a heap */
int 	HeapTestBuildDestroy();
/* extract max and heap sort */
int 	HeapTestMaxSort();
/* testing insert, extract, numItems */
int 	HeapTestInsertExtractNum();
/* testing ForEach */
int		HeapTestForEach();

#endif /* __HEAP_GENERIC_TEST_H__ */


