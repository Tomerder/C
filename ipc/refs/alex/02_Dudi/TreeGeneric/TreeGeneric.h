/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-27
    Description: generic binary tree.
***************************************************************************/

#ifndef __TREE_GEN_H__
#define __TREE_GEN_H__

typedef struct Tree 	Tree;

typedef void*		TreeData;
typedef void*		TreeItr;

/* generic data comparison func */
typedef	int		(*CmpFunc)		 (TreeData _data1, TreeData _data2);
/* modify data */
typedef void	(*DoFunc)		 (TreeItr _itr, void* _params);
/* print generic data item */
typedef void	(*PrintItemFunc) (TreeData _data);


/* order of traversing */
typedef enum
{
	PRE_ORDER,		/* parent -> left -> right */
	IN_ORDER,		/* left -> parent -> right */
	POST_ORDER		/* left -> right -> parent */

} TraverseMode;


/* create tree, includes tree structure. return NULL for failure */
Tree* 		TreeCreate		(CmpFunc _cmpFunc);

/* destroy */
void 		TreeDestroy		(Tree* _tree);



/* insert data & return iterator to it. for error return TreeEnd (stub) */
TreeItr 	TreeInsert		(Tree* _tree, TreeData _data);

/* count # of items */
size_t		TreeCount		(Tree* _tree);

/* search for data, return iterator, if not found return end stub */
TreeItr		TreeFind		(Tree *_tree, TreeData _data);

/* remove item by a given iterator & rearrange tree. return NULL for failure */
TreeData	TreeRemove		(TreeItr _itr);



/* return iterator to 1st item (leftmost), if empty tree return stub */
TreeItr		TreeBegin		(Tree* _tree);

/* return end stub (root) */
TreeItr		TreeEnd			(Tree* _tree);

/* return iterator of following item - smallest larger than _itr->data */
/* going out of bounds results in UNDEFINED BEHAVIOR */
TreeItr		TreeNext		(TreeItr _itr);

/* return iterator of previous item - largest smaller than _itr->data */
/* going out of bounds results in UNDEFINED BEHAVIOR */
TreeItr		TreePrev		(TreeItr _itr);

/* get data in _itr, NULL for error */
TreeData	TreeGetData		(TreeItr _itr);

/* apply _doFunc to all tree nodes, going in _trvMode order */
void 		TreeForEach		(Tree* _tree, TraverseMode _trvMode, DoFunc _doFunc, void* _params);

/* print - for testing */
void		TreePrint		(Tree* _tree, PrintItemFunc _printItemFunc, TraverseMode _trvMode);

/* iterative tree traversal using stack */
void 	TreePrintInIterStack	(Tree *_tree, PrintItemFunc _printFunc);
void 	TreePrintPostIterStack	(Tree *_tree, PrintItemFunc _printFunc);

#endif /* __TREE_GEN_H__ */

