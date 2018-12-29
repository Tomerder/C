/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-20    
    Last modified date:		2013-06-24
    Description: binary search tree functions
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "TreeFunc.h"

#define TREE_ROOT(X) ((X)->m_root)

struct TreeNode
{
	int			m_data;
	TreeNode*	m_left;
	TreeNode*	m_right;
	TreeNode*	m_parent;
};

struct Tree_t
{
	TreeNode *m_root;
};

/*** internal functions ***/
TreeNode* NodeCreate(int _data);
void NodeDestroy(TreeNode *_node);
int NodeFind(TreeNode **_node, int _data);
ADTErr TreeInsertRoot(Tree * _tree, int _data);
void PrintPreOrder(TreeNode * _node);
void PrintInOrder(TreeNode *_node);
void PrintPostOrder(TreeNode * _node);


/****************************** functions ******************************/


		/****************** create & destroy ******************/


/* create tree */
/* allocate memory for tree & set root node pointer to NULL */

Tree* TreeCreate()
{
	Tree *tmpTree;

	tmpTree = (Tree *)malloc(sizeof(Tree));
	assert(tmpTree);	/* ?? */

	TREE_ROOT(tmpTree) = NULL;

	return tmpTree;
}


/* destroy tree - free everything */
/* calls NodeDestroy to free all node except root, then free root & tree pointers */
void TreeDestroy(Tree * _tree)
{
	if( NULL == _tree ) {							/* if tree is nonexistent - no need to do anything */
		return;
	}
	if( NULL == TREE_ROOT(_tree) ) {				/* if root is empty - free only tree */
		free(_tree);
		return;
	}

	NodeDestroy(TREE_ROOT(_tree));					/* else: free all nodes,	*/
	free(TREE_ROOT(_tree));							/* free root				*/
	free(_tree);									/* free tree				*/
}


		/****************** search & insert ******************/

/* insert item to tree */
/* calls: TreeInsertRoot and NodeFind */
ADTErr TreeInsert(Tree * _tree, int _data)
{
	TreeNode *tmpNode, *parent;
	int err, find;

	if( NULL == _tree ) {							/* errors */
		return ERR_NOT_INITIALIZED;
	}

	if( NULL == TREE_ROOT(_tree) ) {				/* if root is empty - insert first item to root */
		err = TreeInsertRoot(_tree, _data);
		return err;
	}
													/* else: create node and use NodeFind to find where to insert it */
	tmpNode = NodeCreate(_data);
	if( NULL == tmpNode) {
		return ERR_ALLOCATION_FAILED;
	}

	parent = TREE_ROOT(_tree);
	find = NodeFind(&parent, _data);

	if( NULL == parent ) {
		return ERR_GENERAL;
	}
	if ( 0 == find ) {								/* return error if item already exists */
		return ERR_ALREADY_EXISTS;
	}

	tmpNode->m_parent = parent;						/* else: insert by assigning the pointers at parent and the new node */

	if ( 1 == find ) {
		parent->m_right = tmpNode;
	} else {
		parent->m_left = tmpNode;
	}
		
	return ERR_OK;
}

/* search tree for a particular item, return 1 if exists, 0 if not, -1 for errors */
/* calls NodeFind */
int TreeIsFound(Tree * _tree, int _data)
{
	int find;
	TreeNode *tmpNode;

	if( NULL == _tree || NULL == TREE_ROOT(_tree) ) {				/* errors */
		return -1;
	}
	
	tmpNode = TREE_ROOT(_tree);

	find = NodeFind(&tmpNode, _data);								/* search tree & retyrn result */
	return ( 0 == find );
}


		/****************** print ******************/

/* print tree in order specified by _t */
/* calls functions: PrintPreOrder, PrintInOrder, PrintPostOrder */
void TreePrint(Tree * _tree, TreeTraverse _t)
{
	printf("Tree is: ");

	if( NULL == _tree ) {							/* errors */
		printf("Not initialized!\n");
		return;
	}
	if( NULL == TREE_ROOT(_tree) ) {
		printf("Empty!\n");
		return;
	}

	printf("\n");

	switch(_t) {
		case(PRE_ORDER):
			PrintPreOrder(TREE_ROOT(_tree));
			break;
		case(IN_ORDER):
			PrintInOrder(TREE_ROOT(_tree));
			break;
		case(POST_ORDER):
			PrintPostOrder(TREE_ROOT(_tree));
			break;
		default:
			break;
	}
}


/****************************** internal functions ******************************/


		/****************** create & destroy ******************/

/* create node - allocate memory, insert data, & set children pointers to NULL */
TreeNode* NodeCreate(int _data)
{
	TreeNode *tmpNode;
	
	tmpNode = (TreeNode *)malloc(sizeof(TreeNode));
	if( NULL == tmpNode ) {
		return NULL;
	}

	tmpNode->m_data = _data;
	tmpNode->m_left = NULL;
	tmpNode->m_right = NULL;

	return tmpNode;
}

/* free all nodes except root - go in Post-order order */
/* _node would start as the root node */
void NodeDestroy(TreeNode *_node)
{
	if( NULL == _node || (NULL == _node->m_left && NULL == _node->m_right) ) {
		return;												/* stopping condition: node is null, or node's children were already deleted */
	}														/* (or never existed) */

	NodeDestroy(_node->m_left);				/* recursion - left - all the way down */
	free(_node->m_left);
	_node->m_left = NULL;

	NodeDestroy(_node->m_right);			/* recursion - right */
	free(_node->m_right);
	_node->m_right = NULL;

	return;
}


		/****************** search & insert ******************/

/* general purpose tree search, top-down (recursive). 
   returns 0 for exact match, 1 for match smaller than data (add to right),
  -1 for match larger than data (add to left). */
/* _node would usually start with the address of the root node */
int NodeFind(TreeNode **_node, int _data)
{
	if ( _data == (*_node)->m_data ) {			/* stop conditions: exact match */
		return 0;
	}

	if ( _data < (*_node)->m_data ) {			/* need to go left */
		
		if ( NULL == (*_node)->m_left ) {			/* stop conditions: can't go left any further */
			return -1;
		} else {
			(*_node) = (*_node)->m_left;			/* go left */
		}

	} else {									/* need to go right */
		if ( NULL == (*_node)->m_right ) {			/* stop conditions: can't go right any further */
			return 1;
		} else {
			(*_node) = (*_node)->m_right;			/* go right */
		}
	}

	return NodeFind(_node, _data);				/* recursion occurs */
}


/* insert first item into root. set parent to NULL */
ADTErr TreeInsertRoot(Tree * _tree, int _data)
{
	TREE_ROOT(_tree) = NodeCreate(_data);

	if ( NULL == TREE_ROOT(_tree)) {
		return ERR_ALLOCATION_FAILED;
	}

	TREE_ROOT(_tree)->m_parent = NULL;

	return ERR_OK;
}


		/****************** print ******************/

/* print tree items in order of: p -> l -> r (recrsively) */
void PrintPreOrder(TreeNode * _node)
{
	int printed=0, pLeft=0, pRight=0;			/* indicators whether the node and its children were already printed */

	if( NULL == _node ) {						/* stopping condition 1: reached NULL */
		return;
	}
	if( pRight && pLeft && printed ) {			/* stopping condition 2: everything here was printed */
		return;
	}

	if( !printed ) {
		printf("%4d\n", _node->m_data);			/* print current node, if wasn't already printed */
		printed = 1;
	}

	PrintPreOrder(_node->m_left);				/* recursion - left */
	pLeft = 1;
	PrintPreOrder(_node->m_right);				/* recursion - right */
	pRight = 1;

	return;
}


/* print tree items in order of: l -> p -> r (recrsive) */
void PrintInOrder(TreeNode * _node)
{
	int printed=0, pLeft=0, pRight=0;			/* indicators whether the node and its children were already printed */

	if( NULL == _node ) {						/* stopping condition 1: reached NULL */
		return;
	}

	if( pRight && pLeft && printed ) {			/* stopping condition 2: everything here was printed */
		return;
	}


	PrintInOrder(_node->m_left);				/* recursion - left - all the way down */
	pLeft = 1;

	if( !printed ) {
		printf("%4d\n", _node->m_data);			/* print current node, if wasn't already printed */
		printed = 1;
	}

	PrintInOrder(_node->m_right);				/* recursion - right */
	pRight = 1;

	if( !printed ) {
		printf("%4d\n", _node->m_data);			/* print current node, if wasn't already printed */
		printed = 1;
	}

	return;

}


/* print tree items in order of: l -> r -> p (recrsive) */
void PrintPostOrder(TreeNode * _node)
{
	int printed=0, pLeft=0, pRight=0;			/* indicators whether the node and its children were already printed */

	if( NULL == _node ) {						/* stopping condition 1: reached NULL */
		return;
	}
	if( pRight && pLeft && printed ) {			/* stopping condition 2: everything here was printed */
		return;
	}


	PrintPostOrder(_node->m_left);				/* recursion - left - all the way down */
	pLeft = 1;

	if( !printed && pRight) {
		printf("%4d\n", _node->m_data);			/* print current node, if wasn't already printed AND its right child was */
		printed = 1;
	}

	PrintPostOrder(_node->m_right);				/* recursion - right */
	pRight = 1;

	if( !printed ) {
		printf("%4d\n", _node->m_data);			/* print current node, if wasn't already printed */
		printed = 1;
	}


	return;
}

