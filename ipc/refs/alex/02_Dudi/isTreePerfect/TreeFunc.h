/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-06-20    
    Last modified date:		2013-06-21
    Description: header file for binary search tree
***************************************************************************************/

#ifndef __TREE_H__
#define __TREE_H__

typedef struct Tree_t Tree;
typedef struct TreeNode TreeNode;

/* order of printing */
typedef enum
{
	PRE_ORDER,		/* parent -> left -> right */
	IN_ORDER,		/* left -> parent -> right */
	POST_ORDER		/* left -> right -> parent */

} TreeTraverse;

/* allocate memory for a tree */
Tree* TreeCreate();
/* destroy tree - free everything */
void TreeDestroy(Tree * _tree);

/* insert new data item at the right place in the tree */
ADTErr TreeInsert(Tree * _tree, int _data);
/* find if an item already exists in the tree (1 for yes, 0 for no */
int TreeIsFound(Tree * _tree, int _data);

/* print tree in order specified by _t */
void TreePrint(Tree * _tree, TreeTraverse _t);


#endif /* __TREE_H__ */
