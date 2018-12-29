/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-28
    Description: generic binary tree - printing "in-order" iteratively, 
    			 using generic stack
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TreeGeneric.h"
#include "GenericStack.h"

#define MAGIC 			0xdeadbeef

#define	ROOTNODE(X)		(X)->m_root.m_Sons[0]
#define	LEFT(X)			(TreeItr)((Node*)(X))->m_Sons[0]
#define	RIGHT(X)		(TreeItr)((Node*)(X))->m_Sons[1]
#define	PARENT(X)		(TreeItr)((Node*)(X))->m_parent
#define DATA(X)			((Node*)(X))->m_data



typedef struct Node
{
	TreeData		m_data;		/* generic data */
	struct Node*	m_Sons[2];	/* 0 is left, 1 is right */
	struct Node*	m_parent;	
} Node;

struct Tree
{
	Node		m_root;		/* stub - TreeEnd. has only a left son - the "actual" root */
	CmpFunc		m_cmpFunc;	/* comparison function */
};


int CompFuncInt(TreeData _data1, TreeData _data2);
void printFuncInt(TreeData _data);



/* iterative in-order */
void TreePrintInIterStack(Tree *_tree, PrintItemFunc _printFunc)
{
	TreeItr itr;
	Stack* stack;
	
	assert(_tree);
	stack = StackCreate();
	if(!stack) {
		return;
	}
	
	itr = (TreeItr)(ROOTNODE(_tree));
	while(1)
	{
		while( itr )
		{
			StackPush(stack, itr);
			itr = LEFT(itr);
		}
		
		if(StackIsEmpty(stack))
		{
			break;
		}
		
		itr = StackPop(stack);
		_printFunc(DATA(itr));
		itr = RIGHT(itr);	
	}
}
/*----------------------------------------------------------*/


/*** MAIN ***/
int main()
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
	
	TreePrintInIterStack(tree, printFuncInt);
	printf("\n\n");
	
	return 0;
}
/*----------------------------------------------------------*/


/* compare 2 data items - integer, return a poisive # for 1 > 2, 0 for equal, negative for 1 < 2 */
int CompFuncInt(TreeData _data1, TreeData _data2)
{
	return *(int*)_data1 - *(int*)_data2;
}
/*----------------------------------------------------------*/

/* print integer data */
void printFuncInt(TreeData _data)
{
	printf("%d ", *(int*)_data);
}
/*----------------------------------------------------------*/


