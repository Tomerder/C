#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__
/*
 * Misc definitions of strong types
 */
#ifndef _DATA_PARAM_COMP_DO
#define _DATA_PARAM_COMP_DO
typedef void* 	Data;
typedef void*	Param;
typedef int		(*DataComp) (Data, Data);  /* 0 if equal */
typedef int		(*DoFunc)   (Data, Param); /* 0=success */
typedef void	(*PrintFunc)(Data);
#endif /* _DATA_PARAM_COMP_DO */

typedef struct SkipList SkipList;
typedef void * SkipListItr;

/****************************  SkipListCreate  ******************************
 * The function creates a new empty skip list
 * Input: Compare Function
 * Output: a pointer to the created skip list
 ****************************************************************************/
SkipList *	SkipListCreate (DataComp _compFunc);

/**************************** SkipListDestroy  ******************************
 * The function destroy the given skip list
 * Input: skip list to destroy
 * Remember to save the pointers to your data before execute this function!
 ****************************************************************************/
void 		SkipListDestroy(SkipList * _skipList);

/*****************************  SkipListInsert  *****************************
 * The function insert new node to the skip list with speciffied data
 * Cannot have duplications	!!!
 * Input:  skip list & data to insert
 * Output: iterator to new item or SkipListEnd if error
 ****************************************************************************/
SkipListItr SkipListInsert (SkipList * _skipList, Data _data);

/*****************************  SkipListRemove  *****************************
 * The function removes data at specified iterator
 * Input:  skip list & iterator of the data
 * Output: removed data
 ****************************************************************************/
Data 		SkipListRemove (SkipList * _skipList, SkipListItr _itr);

/******************************  SkipListFind  ******************************
 * The function search data in the skip list. if data not found it returns
 * the end iteratror. if data found it returns iterator to the item
 * Input: skip list & data to find
 * Output: iterator to the data or SkipListEnd
 * Complexity = O(log n)
 ****************************************************************************/
SkipListItr SkipListFind(SkipList * _skipList, Data _data);

/*****************************  SkipListGetData  ****************************
 * The function get the data of the which the iterator points at
 * Input:  skip list iterator
 * Output: data pointer
 ****************************************************************************/
Data		SkipListGetData(SkipListItr _itr);

/*********************  SkipList Iterator functions *************************/
SkipListItr SkipListEnd	 (SkipList * _skipList);
SkipListItr SkipListBegin(SkipList * _skipList);
SkipListItr SkipListNext (SkipListItr _skipItr);
SkipListItr SkipListPrev (SkipListItr _skipItr);

/********************* SkipList Access functions ***************************/
size_t		SkipListCountItems  (SkipList * _skipList);
int			IsSkipListEmpty		(SkipList * _skipList);

/********************* SkipListForEach **************************************
 * This function performs specified operation on all items in the range
 * until operation returns failure or out of range
 * Input: start and end iterators
 * Input: function to perform
 * Output: success
 ****************************************************************************/
int			SkipListForEach(SkipListItr _from, SkipListItr _to, DoFunc _do, Param);
							
/********************************  SkipListPrint ****************************
 * The function prints the given skip list
 * Input: skip list to print & print function for printing the data
 ****************************************************************************/
void 		SkipListPrint(SkipList * _skipList, PrintFunc _printFunc);


/*************************** DEBUG ****************************************/
void PrintSkip(SkipList * _skipList);
void PrintSkipAllLevels(SkipList * _skipList);

int GetNumOfLevels(SkipList * _skipList);
/**************************************************************************/


#endif  /* __SKIP_LIST_H__ */

