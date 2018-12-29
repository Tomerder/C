/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-05    
    Last modified date:		2013-09-05
    Description: Exam problem: turn sorted linked list to tree and vice-varsa,
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "List22Tree.h"


#define SIZE 10

TreeNode*	List2Tree		(ListNode* _head, int _nItems);
void		List2TreeImp	(ListNode* _root, int _nItems);

ListNode*	Tree2List		(TreeNode* _root);
void		Tree2ListImp	(TreeNode* _root);


/*######################################################################*/
/*								MAIN									*/
/*######################################################################*/
int main()
{
	TreeNode* treeRoot		= NULL;
	TreeNode* treeFromList	= NULL;
	ListNode* listHead		= NULL;
	ListNode* listFromTree	= NULL;
	int i;
	int treeArr[SIZE] = { 5, 15, 2, 7, 13, 18, 3, 6, 9, 14};
	int listArr[SIZE] = { 4, 7, 2, 9, 8, 5, 6, 11, 3, 10};
	
	
	/*** TREE ***/
	
	treeRoot = TreeNodeCreate(10);
	if(!treeRoot)
	{
		fprintf(stderr, "allocation failed, line %d\n", __LINE__ -3);
		return 1;
	}
	
	for(i=0; i<SIZE; ++i)
	{
		TreeInsert(treeRoot, treeArr[i]);
	}
	
	printf("Tree:  ");
	TreePrint(treeRoot, PRE);
	printf("\n\n");
	
	/* LIST FROM TREE */
	
	listFromTree = Tree2List(treeRoot);
	
	printf("List from tree:  ");
	ListPrint(listFromTree);
	printf("\n");
	
	
	/*** LIST ***/
	
	listHead = ListNodeCreate(1);
	if(!listHead)
	{
		fprintf(stderr, "allocation failed, line %d\n", __LINE__ -3);
		return 1;
	}
	
	for(i=0; i<SIZE; ++i)
	{
		ListInsert(listHead, listArr[i]);
	}
	
	printf("List:  ");
	ListPrint(listHead);
	printf("\n");
	
	/* TREE FROM LIST */
	
	treeFromList = List2Tree(listHead, SIZE+1);
	
	printf("Tree from list:  ");
	TreePrint(treeFromList, PRE);
	printf("\n\n");
	

	TreeDestroy(treeFromList);
	ListDestroy(listFromTree);
	
	return 0;
}
/*######################################################################*/



/*######################################################################*/
/*							LIST 2 TREE									*/
/*######################################################################*/


TreeNode*	List2Tree(ListNode* _head, int _nItems)
{
	ListNode* root = _head;
	int i;
	
	assert(_head && _nItems);
	
	/* find root - median value */
	for(i=0; i<_nItems/2; ++i)
	{
		root = root->m_next;
	}
	
	List2TreeImp(root, _nItems);
	
	return (TreeNode*)root;
}
/*######################################################################*/

/* recursive implementation */
void	List2TreeImp(ListNode* _root, int _nItems)
{
	ListNode* rootLeft	= NULL;
	ListNode* rootRight	= NULL;
	int i;
	
	if(_nItems <= 1) {
		return;
	}
	
	rootLeft = _root->m_prev;
	rootRight = _root->m_next;
	
	/* find next nodes - median value */
	for(i=0; i<(_nItems-1)/4; ++i)
	{
		rootLeft = rootLeft->m_prev;
		rootRight = rootRight->m_next;
	}
	
	/* rearrange as tree */
	
	if(_root->m_prev) {
		_root->m_prev->m_next = NULL;
	}
	_root->m_prev = rootLeft;
	
	if(_root->m_next) {
		_root->m_next->m_prev = NULL;
	}
	_root->m_next = rootRight;
	
	
	List2TreeImp(rootLeft, (_nItems-1)/2);
	List2TreeImp(rootRight, (_nItems-1)/2);
}
/*######################################################################*/



/*######################################################################*/
/*							 TREE 2 LIST								*/
/*######################################################################*/


ListNode*	Tree2List(TreeNode* _root)
{
	assert(_root);
	
	Tree2ListImp(_root);
	
	while(_root->m_left) {
		_root = _root->m_left;
	}
	
	return (ListNode*)_root;
}

/*######################################################################*/

void	Tree2ListImp(TreeNode* _root)
{
	TreeNode* left	= NULL;
	TreeNode* right	= NULL;
	
	if( !_root || (!_root->m_left && !_root->m_right) ) {
		return;
	}
	
	left = _root->m_left;
	right = _root->m_right;
	
	Tree2ListImp(left);
	Tree2ListImp(right);
	
	if(left) {
		while(left->m_right) {
			left = left->m_right;
		}
		
		left->m_right = _root;
		_root->m_left = left;
	}
	
	if(right) {
		while(right->m_left) {
			right = right->m_left;
		}
		
		right->m_left = _root;
		_root->m_right = right;
	}
}
/*######################################################################*/




