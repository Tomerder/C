/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-27
    Description: generic binary tree - test functions
***************************************************************************/

#ifndef __TREE_GEN_TEST_H__
#define __TREE_GEN_TEST_H__


/* create & destroy a tree */
int 	TreeTestCreateDestroy();

/* insert items to a tree, print (using TreeGetData) */
/* then destroy - using remove which also uses TreeNext & TreeEnd */
int 	TreeTestInsertGetDestroy();

/* test iterator-moving functions: Begin, End, Prev, Next */
int 	TreeTestIterators();

/* test TreeFind, TreeCount, TreeRemove */
int 	TreeTestFindCountRemove();

/* test TreeForEach */
int 	TreeTestForEach();

/* test TreePrintInIterStack */
int 	TreeTestStackTraverse();

#endif  /* __TREE_GEN_TEST_H__ */
