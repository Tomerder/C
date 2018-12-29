/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-09-05    
    Last modified date:		2013-09-05
    Description: Exam problem: turn sorted linked list to tree and vice-varsa,
    				general functions.
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "List22Tree.h"



/*######################################################################*/
/*							TREE FUNCTIONS								*/
/*######################################################################*/

static void	TreeDestroyImp	(TreeNode* _root);

/*######################################################################*/

/* create */
TreeNode*	TreeNodeCreate(int _key)
{
	TreeNode* newNode = NULL;
	
	newNode = (TreeNode*)malloc(sizeof(TreeNode));
	if(!newNode)
		return NULL;
	
	newNode->m_key		= _key;
	newNode->m_left		= NULL;
	newNode->m_right	= NULL;
	return newNode;
}
/*######################################################################*/


/* destroy */
void	TreeDestroy(TreeNode* _root)
{
	TreeDestroyImp(_root);
	free(_root);
}
/*######################################################################*/

/* destroy - recursive implementation */
void	TreeDestroyImp(TreeNode* _root)
{
	if( !_root || (!_root->m_left && !_root->m_right) ) {
		return;
	}
		
	TreeDestroyImp(_root->m_left);
	TreeDestroyImp(_root->m_right);
	
	if(_root->m_left) {
		free(_root->m_left);
		_root->m_left = NULL;
	}
	if(_root->m_right) {
		free(_root->m_right);
		_root->m_right = NULL;
	}
}
/*######################################################################*/

/* insert */
/* calls TreeFind */
TreeNode*	TreeInsert(TreeNode* _root, int _key)
{
	int side;
	TreeNode* parent = _root;
	TreeNode* newNode = NULL;
	
	assert(_root);
	
	side = TreeFind(&parent, _key);
	if(!side) {
		return NULL;
	}
	
	newNode = TreeNodeCreate(_key);
	
	if(LEFT == side) {
		parent->m_left = newNode;
	}
	else {
		parent->m_right = newNode;
	}
	return newNode;
}
/*######################################################################*/


/* general purpose search - return 0 for exact match, 1 for match smaller
than data (add to right), -1 for match larger than data (add to left). */
int	TreeFind(TreeNode **_root, int _key)
{
	if( _key == (*_root)->m_key) {
		return 0;
	}
	
	if( _key < (*_root)->m_key)
	{
		if(!(*_root)->m_left)
			return LEFT;
		*_root = (*_root)->m_left;
	}
	else	/* _key > (*_root)->m_key */
	{
		if(!(*_root)->m_right)
			return RIGHT;
		*_root = (*_root)->m_right;	
	}
	return TreeFind(_root, _key);
}
/*######################################################################*/


void	TreePrint(TreeNode* _root, int _mode)
{
	if(!_root)
		return;
	
	if(PRE == _mode ) { printf("%2d  ", _root->m_key); }
	
	TreePrint(_root->m_left, _mode);
	
	if(IN == _mode ) { printf("%2d  ", _root->m_key); }
	
	TreePrint(_root->m_right, _mode);
	
	if(POST == _mode ) { printf("%2d  ", _root->m_key); }
}
/*######################################################################*/


/*######################################################################*/
/*							LIST FUNCTIONS								*/
/*######################################################################*/

static void		ListInsertImp	(ListNode* _original, ListNode* _newNode, int _side);

/*######################################################################*/

/* create */
ListNode*	ListNodeCreate(int _key)
{
	ListNode* newNode = NULL;
	
	newNode = (ListNode*)malloc(sizeof(ListNode));
	if(!newNode)
		return NULL;
	
	newNode->m_key 	= _key;
	newNode->m_prev	= NULL;
	newNode->m_next	= NULL;
	return newNode;
}
/*######################################################################*/

void	ListDestroy(ListNode* _head)
{
	ListNode* temp;
	
	while(_head)
	{
		temp = _head->m_next;
		free(_head);
		_head = temp;
	}
}
/*######################################################################*/

/* insert */
/* calls ListFind */
ListNode*	ListInsert(ListNode* _head, int _key)
{
	int side;
	ListNode* parent = _head;
	ListNode* newNode = NULL;
	
	assert(_head);
	
	side = ListFind(&parent, _key);
	if(!side) {
		return NULL;
	}
	
	newNode = ListNodeCreate(_key);
	ListInsertImp(parent, newNode, side);
	
	return newNode;
}
/*######################################################################*/

/* general purpose search - return 0 for exact match, 1 for match smaller
than data (add after), -1 for match larger than data (add before). */
int	ListFind(ListNode **_head, int _key)
{
	assert(*_head);
	
	while( (*_head)->m_next )
	{
		if(_key == (*_head)->m_key) {
			return 0;
		}
		else if(_key < (*_head)->m_key) {
			return LEFT;
		}
		
		*_head = (*_head)->m_next;
	}
	
	
	if(_key == (*_head)->m_key) {
		return 0;
	}
	else if(_key < (*_head)->m_key) {
		return LEFT;
	}
	else {
		return RIGHT;
	}
}
/*######################################################################*/


void ListInsertImp(ListNode* _original, ListNode* _newNode, int _side)
{
	ListNode *temp;
	
	if(LEFT == _side)
	{
		temp = _original->m_prev;
		
		_original->m_prev = _newNode;
		_newNode->m_next = _original;
		
		_newNode->m_prev = temp;
		
		if(temp) {
			temp->m_next = _newNode;
		}
	}
	else
	{
		temp = _original->m_next;
		
		_original->m_next = _newNode;
		_newNode->m_prev = _original;
		
		_newNode->m_next = temp;
		
		if(temp) {
			temp->m_prev = _newNode;
		}
	}
}
/*######################################################################*/

void	ListPrint(ListNode* _head)
{
	while(_head)
	{
		printf("%2d  ", _head->m_key);
		_head = _head->m_next;
	}
	printf("\n");
}
/*######################################################################*/


