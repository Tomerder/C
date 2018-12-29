/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-21    
    Last modified date:		2013-07-22
    Description: generic double linked list - test functions.
********************************************************************************/

#ifndef __LLIST_TEST_H__
#define __LLIST_TEST_H__

typedef struct City City;
typedef struct Student Student;

#define LIST_SIZE	6


#ifdef CHECK_NULL
void LlistTestNull();
#endif


/* create & destroy a list */
int 	LlistTestCreateDestroy();

/* test pushHead for integers & cities */
int 	LlistTestPushHead	(size_t size);
/* test pushTail for integers & cities */
int 	LlistTestPushTail	(size_t size);

/* test popHead for integers & cities */
int 	LlistTestPopHead	(size_t _size);
/* test popTail for integers & cities */
int 	LlistTestPopTail	(size_t _size);

/* test countItems */
int 	ListTestCountItems	(size_t _size);
/* test isEmpty */
int 	ListTestIsEmpty		(size_t _size);


/********* Iterators *********/

#ifdef ITER

/* test begin & end */
int 	ListItrTestBeginEnd	(size_t _size);
/* test get data */
int 	ListItrTestGet		(size_t _size);
/* test next */
int 	ListItrTestNext		(size_t _size);
/* test prev */
int 	ListItrTestPrev		(size_t _size);
/* test insert */
int 	ListItrTestInsert	(size_t _size);
/* test remove */
int 	ListItrTestRemove	(size_t _size);
/* test ListFindFirst */
int 	ListTestFindFirst	(size_t _size);
/* test ListFindFirst */
int 	ListTestForEach		(size_t _size);

#endif

#endif  /* __LLIST_TEST_H__ */

