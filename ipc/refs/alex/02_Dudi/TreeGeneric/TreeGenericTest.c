/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-28
    Description: generic binary tree - test functions
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TreeGeneric.h"
#include "TreeGenericTest.h"

/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define MAGIC 0xdeadbeef
#define MAX 20

#define	PARENT(X)	(TreeItr)((Node*)(X))->m_parent

typedef struct Node
{
	TreeData		m_data;		/* generic data */
	struct Node*	m_Sons[2];	/* 0 is left, 1 is right */
	struct Node*	m_parent;	
} Node;

static int 		CompFuncInt		(TreeData _data1, TreeData _data2);
static void 	printFuncInt	(TreeData _data);
static void		AvParent		(TreeItr _itr, void* _params);
static void		AddBelow100		(TreeItr _itr, void* _params);

static void 	ErrorCheckPrint	(int *_errTemp, int *_errTot, char *_text, int _number);
 static void 	PrintDebug		(char *_text, int _number); 

/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*						TEST FUNCTIONS						*/
/*															*/
/* 		return 0 for success, positive # for failure,		*/
/* 		-1 for error unrelated to tested function(s)		*/
/*----------------------------------------------------------*/


/* create & destroy a tree */
int TreeTestCreateDestroy()
{
	Tree* tree = NULL;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return 1;
	}
	PrintDebug("Tree Created.", -1);
	TreeDestroy(tree);
	PrintDebug("Tree Destroyed.\n", -1);
	return 0;
}
/*----------------------------------------------------------*/


/* insert items to a tree, print (using TreeGetData), destroy tree (using remove) */
int TreeTestInsertGetDestroy()
{
	Tree* tree = NULL;
	TreeItr	tempItr, arrItr[12];
	TreeData tempData;
	int arrDataSmall[3] = {10, 15, 5};
	int arrDataLarge[12] = {100, 50, 30, 70, 150, 170, 40, 10, 120, 60, 110, 140};
	int arrLargeOrder[12] = {10, 30, 40, 50, 60, 70, 100, 110, 120, 140, 150, 170};
	int i, tempInt, errorTot = 0, errorTemp = 0;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	
	PrintDebug("\n", -1);
	
	/* insert 3 items & check success by comparing initial data to iterator data */
	/* 1st insertion is (by definition) to an empty tree */
	/* then destroy - using remove which also uses TreeNext & TreeEnd */
	
	for(i=0; i<3; ++i)
	{
		arrItr[i] = TreeInsert(tree, (TreeData)&arrDataSmall[i]);
		tempData = TreeGetData(arrItr[i]);
		errorTemp += ( *(int*)tempData != arrDataSmall[i] );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: Insert, line ", __LINE__-5);
	
	/* prints only in debug mode */
	PrintDebug("Small tree:", -1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	TreePrint(tree, printFuncInt, POST_ORDER);
	
	TreeDestroy(tree);
	
	/* new tree, insert 12 items */
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	for(i=0; i<12; ++i)
	{
		arrItr[i] = TreeInsert(tree, (TreeData)&arrDataLarge[i]);
		errorTemp += ( arrItr[i] == TreeEnd(tree) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: Insert, line ", __LINE__-4);
	
	tempItr = TreeBegin(tree);
	for(i=0; i<12; ++i, tempItr = TreeNext(tempItr))
	{
		tempData = TreeGetData(tempItr);
		errorTemp += ( *(int*)tempData != arrLargeOrder[i] );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: Insert, wrong order, line ", __LINE__-15);
	
	/* prints only in debug mode */
	PrintDebug("\n\nLarger tree:", -1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	TreePrint(tree, printFuncInt, POST_ORDER);
	
	/* attempt to insert already existing item */
	tempInt = arrDataLarge[3];
	tempItr = TreeInsert(tree, (TreeData)&tempInt);
	errorTemp = ( tempItr != arrItr[3] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: Insert with duplicate data, line ", __LINE__-3);
	
	PrintDebug(" ", -1);
	
	/* destroy tree */
	TreeDestroy(tree);	
	
	return errorTot;
}
/*----------------------------------------------------------*/


/* test iterator-moving functions: Begin, End, Prev, Next */
int TreeTestIterators()
{
	Tree* tree = NULL;
	TreeItr testItr = NULL;
	TreeData tempData;
	int arrData[12] = {100, 50, 30, 70, 150, 170, 40, 10, 120, 60, 110, 140};
	int i, errorTot = 0, errorTemp = 0;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	PrintDebug(" ", -1);
	
	/*** empty tree ***/
	
	/* TreeBegin - empty tree */
	testItr = TreeBegin(tree);
	tempData = TreeGetData(testItr);	
	errorTemp = ( tempData != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeBegin with empty tree, line ", __LINE__-4);

	/* TreeEnd - empty tree */
	testItr = TreeEnd(tree);
	tempData = TreeGetData(testItr);	
	errorTemp = ( tempData != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeEnd with empty tree, line ", __LINE__-4);

	/* TreePrev - empty tree */
	testItr = TreePrev(testItr);
	errorTemp = ( NULL != testItr );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreePrev with empty tree, line ", __LINE__-3);
	
	/* TreeNext - empty tree */
	testItr = TreeBegin(tree);
	testItr = TreeNext(testItr);
	errorTemp = ( NULL != testItr );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeNext with empty tree, line ", __LINE__-3);
	
	/*** non-empty tree ***/
	
	/* fill tree */
	for(i=0; i<12; ++i)
	{
		TreeInsert(tree, (TreeData)&arrData[i]);
	}
	
	TreePrint(tree, printFuncInt, IN_ORDER);
	
	/* TreeBegin */
	testItr = TreeBegin(tree);
	tempData = TreeGetData(testItr);
	errorTemp = ( *(int*)tempData != 10 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeBegin, line ", __LINE__-4);
	PrintDebug("\nfirst item is:\t\t", *(int*)tempData);
	
	/* treeNext */
	testItr = TreeNext(testItr);
	testItr = TreeNext(testItr);
	tempData = TreeGetData(testItr);	
	errorTemp = ( *(int*)tempData != 40 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeNext, line ", __LINE__-4);
	PrintDebug("going 2 steps foreward:\t", *(int*)tempData);
	
	for(i=0; i<5; ++i) {
		testItr = TreeNext(testItr);
	}
	tempData = TreeGetData(testItr);	
	errorTemp = ( *(int*)tempData != 110 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeNext, line ", __LINE__-5);
	PrintDebug("5 more steps:\t\t", *(int*)tempData);
	
	/* TreePrev */
	testItr = TreePrev(testItr);
	testItr = TreePrev(testItr);
	tempData = TreeGetData(testItr);	
	errorTemp = ( *(int*)tempData != 70 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreePrev, line ", __LINE__-4);
	PrintDebug("now 2 steps back:\t", *(int*)tempData);
	
	/* TreeEnd */
	testItr = TreeEnd(tree);
	tempData = TreeGetData(testItr);
	errorTemp = ( tempData != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeEnd, line ", __LINE__-4);

	/* TreePrev */
	testItr = TreePrev(testItr);
	tempData = TreeGetData(testItr);	
	errorTemp = ( *(int*)tempData != 170 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreePrev, line ", __LINE__-4);
	PrintDebug("last item before end:\t", *(int*)tempData);
	
	for(i=0; i<3; ++i) {
		testItr = TreePrev(testItr);
	}
	tempData = TreeGetData(testItr);	
	errorTemp = ( *(int*)tempData != 120 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreePrev, line ", __LINE__-4);
	PrintDebug("3 steps back:\t\t", *(int*)tempData);
	
	/* TreePrev from beginning */
	testItr = TreeBegin(tree);
	testItr = TreePrev(testItr);
	errorTemp = ( testItr != NULL );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreePrev from beginning, line ", __LINE__-3);

	PrintDebug(" ", -1);
	TreeDestroy(tree);	
	return errorTot;
}
/*----------------------------------------------------------*/


/* test TreeFind, TreeCount, TreeRemove */
int TreeTestFindCountRemove()
{
	Tree* tree = NULL;
	TreeItr testItr = NULL, arrItr[12];
	TreeData tempData;
	int arrData[12] = {100, 50, 30, 70, 150, 170, 40, 10, 120, 60, 110, 140};
	int count;
	int i, errorTot = 0, errorTemp = 0;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	
	PrintDebug(" ", -1);
	
	/*** empty tree ***/
	
	/* TreeFind */
	testItr = TreeFind(tree, (TreeData)&arrData[8]);
	errorTemp = ( TreeGetData(testItr) != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeFind with empty tree, line ", __LINE__-3);

	/* TreeRemove */
	tempData = TreeRemove(testItr);
	errorTemp = ( tempData != NULL );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeRemove on end stub, line ", __LINE__-3);

	/* TreeCount */
	count = TreeCount(tree);
	errorTemp = ( count != 0 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeCount on empty tree, line ", __LINE__-3);

	/*** non-empty tree ***/
	
	/* fill tree */
	for(i=0; i<12; ++i)
	{
		arrItr[i] = TreeInsert(tree, (TreeData)&arrData[i]);
	}
	
	/* TreeCount */
	count = TreeCount(tree);
	errorTemp = ( count != 12 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeCount, line ", __LINE__-3);
	
	/* TreeFind - non-existing item */
	testItr = TreeFind(tree, (TreeData)&count);
	errorTemp = ( TreeGetData(testItr) != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeFind with non-existing item, line ", __LINE__-3);

	/* TreeFind - existing item */
	testItr = TreeFind(tree, (TreeData)&arrData[4]);
	errorTemp = ( testItr != arrItr[4] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeFind, line ", __LINE__-3);
	PrintDebug("before removal:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	/* TreeRemove */
	tempData = TreeRemove(testItr);
	errorTemp = ( *(int*)tempData != arrData[4] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeRemove, line ", __LINE__-3);
	PrintDebug("\nafter removal:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	/* TreeCount */
	count = TreeCount(tree);
	errorTemp = ( count != 11 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeCount after removal, line ", __LINE__-3);
	
	/* TreeFind - removed item */
	testItr = TreeFind(tree, (TreeData)&arrData[4]);
	errorTemp = ( TreeGetData(testItr) != (TreeData)MAGIC );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeFind for removed item, line ", __LINE__-3);

	/* TreeRemove - root node */
	tempData = TreeRemove(arrItr[0]);
	errorTemp = ( *(int*)tempData != arrData[0] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeRemove (root), line ", __LINE__-3);
	PrintDebug("\nafter removal of root:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	/* insert removed item - shoulb be in a different place */
	arrItr[0] = TreeInsert(tree, (TreeData)&arrData[0]);
	/* TreeFind - re-inserted item */
	testItr = TreeFind(tree, (TreeData)&arrData[0]);
	errorTemp = ( testItr != arrItr[0] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: TreeFind for re-inserted item, line ", __LINE__-3);
	PrintDebug("\nafter re-insertion of former root:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	PrintDebug(" ", -1);
	TreeDestroy(tree);	
	return errorTot;
}
/*----------------------------------------------------------*/


/* test TreeForEach */
int TreeTestForEach()
{
	Tree* tree = NULL;
	int arrData[12] = {100, 50, 30, 70, 150, 170, 40, 10, 120, 60, 110, 140};
	int arrDataTemp[12];
	int i, param = 75;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	
	PrintDebug(" ", -1);
		
	/* fill tree */
	for(i=0; i<12; ++i)
	{
		arrDataTemp[i] = arrData[i];
		TreeInsert(tree, (TreeData)&arrDataTemp[i]);
	}
	PrintDebug("\noriginal tree:",-1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	
	/* TreeForEach - pre order */
	TreeForEach(tree, PRE_ORDER, AvParent, (void*)&param);
	
	PrintDebug("\nafter TreeForEach in pre-order:",-1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	
	/* destroy tree, create & fill new one */
	TreeDestroy(tree);
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	for(i=0; i<12; ++i)
	{
		arrDataTemp[i] = arrData[i];
		TreeInsert(tree, (TreeData)&arrDataTemp[i]);
	}

	/* TreeForEach - in order */
	TreeForEach(tree, IN_ORDER, AvParent, (void*)&param);
	
	PrintDebug("\noriginal after TreeForEach in-order:",-1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	

	/* destroy tree, create & fill new one */
	TreeDestroy(tree);
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	for(i=0; i<12; ++i)
	{
		arrDataTemp[i] = arrData[i];
		TreeInsert(tree, (TreeData)&arrDataTemp[i]);
	}
	
	/* TreeForEach - post order */
	TreeForEach(tree, POST_ORDER, AvParent, (void*)&param);
	
	PrintDebug("\nnoriginal after TreeForEach post-order:",-1);
	TreePrint(tree, printFuncInt, IN_ORDER);
	
	/* destroy tree, create & fill new one */
	TreeDestroy(tree);
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	for(i=0; i<12; ++i)
	{
		arrDataTemp[i] = arrData[i];
		TreeInsert(tree, (TreeData)&arrDataTemp[i]);
	}
	PrintDebug("\n\noriginal tree:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	/* TreeForEach - changing tree order */
	TreeForEach(tree, IN_ORDER, AddBelow100, (void*)&param);
	
	PrintDebug("\nafter another TreeForEach, and rearranging tree:",-1);
	TreePrint(tree, printFuncInt, PRE_ORDER);
	
	PrintDebug(" ", -1);
	TreeDestroy(tree);	
	return 0;
}
/*----------------------------------------------------------*/


/* test TreePrintInIterStack */
int TreeTestStackTraverse()
{
	Tree *tree;
	int arrData[12] = {100, 50, 30, 70, 150, 170, 40, 10, 120, 60, 110, 140};
	int i;
	
	tree = TreeCreate(CompFuncInt);
	if(!tree) {
		return -1;
	}
	for(i=0; i<12; ++i)
	{
		TreeInsert(tree, (TreeData)&arrData[i]);
	}
	
	PrintDebug("\nIterative tree traversal using stack:\n-in order:",-1);
	TreePrintInIterStack(tree, printFuncInt);
	PrintDebug("\n-post order:",-1);
	TreePrintPostIterStack(tree, printFuncInt);
	printf("\n\n");
	
	return 0;
}


/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/*** for integer data ***/

/* compare 2 data items - integer, return a poisive # for 1 > 2, 0 for equal, negative for 1 < 2 */
int CompFuncInt(TreeData _data1, TreeData _data2)
{
	return *(int*)_data1 - *(int*)_data2;
}
/*----------------------------------------------------------*/

/* print integer data */
void printFuncInt(TreeData _data)
{
#ifdef DEBUG
	printf("%d ", *(int*)_data);
#endif
}
/*----------------------------------------------------------*/


/*** for TreeForEach ***/


/* average with value of parent */
void AvParent(TreeItr _itr, void* _params)
{
	TreeData data = TreeGetData(_itr);
	TreeData dataParent = TreeGetData(PARENT(_itr));
	
	if(dataParent == (TreeData)MAGIC) {
		return;
	}
	
	*(int*)data = ( *(int*)data + *(int*)dataParent )/2;
	((Node*)_itr)->m_data = data;
}
/*----------------------------------------------------------*/


/* add *(int*)_params to all items below 100 */
void	AddBelow100(TreeItr _itr, void* _params)
{
	TreeData data = TreeGetData(_itr);
	int add = *(int*)_params;
	
	if( *(int*)data < 100 ) {
		*(int*)data = *(int*)data + add;
		((Node*)_itr)->m_data = data;
	}
}
/*----------------------------------------------------------*/


/* for printing in debug mode */
void PrintDebug(char *_text, int _number)
{
#ifdef DEBUG
	printf("%s", _text);
	if( _number != -1 ) {
		printf("%d", _number);
	}
	printf("\n");
#endif
}
/*----------------------------------------------------------*/


/* error handling for test functions */
void ErrorCheckPrint(int *_errTemp, int *_errTot, char *_text, int _number)
{
	if(*_errTemp)
	{
		PrintDebug(_text, _number);
		*_errTot += *_errTemp;
		*_errTemp = 0;
	}
}
/*----------------------------------------------------------*/

