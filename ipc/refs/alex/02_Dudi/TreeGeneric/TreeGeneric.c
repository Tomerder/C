/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-25    
    Last modified date:		2013-07-28
    Description: generic binary tree.
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TreeGeneric.h"
#include "GenericStack.h"

/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define MAGIC 0xdeadbeef

#define COMPARE(X,Y,Z)	(X)->m_cmpFunc((Y),(Z))

#define	ROOTNODE(X)		(X)->m_root.m_Sons[0]
#define	LEFT_NODE(X)	((Node*)(X))->m_Sons[0]
#define	RIGHT_NODE(X)	((Node*)(X))->m_Sons[1]
#define	PARENT_NODE(X)	((Node*)(X))->m_parent

#define	LEFT(X)		(TreeItr)((Node*)(X))->m_Sons[0]
#define	RIGHT(X)	(TreeItr)((Node*)(X))->m_Sons[1]
#define	PARENT(X)	(TreeItr)((Node*)(X))->m_parent
#define DATA(X)		((Node*)(X))->m_data

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

/* apply _doFunc in particular order */
typedef void	(*DoToOrder)	(TreeItr _itr, DoFunc _doFunc, void* _params);

static void		DoOrder			(Tree* _tree, DoFunc _doFunc, DoToOrder _doOrderFunc, void* _params);
static void 	DoPreOrder		(TreeItr _itr, DoFunc _doFunc, void* _params);
static void 	DoInOrder		(TreeItr _itr, DoFunc _doFunc, void* _params);
static void 	DoPostOrder		(TreeItr _itr, DoFunc _doFunc, void* _params);

static TreeItr	NodeCreate		(Node* _parent, TreeData _data);
static TreeItr	NodeFind		(Tree* _tree, TreeData _data);
static void		LinkSonParent	(TreeItr _itr, int _side);
static TreeItr 	TreeNextPost	(TreeItr _itr); /* go in post-order */
static void 	TreeShake		(Tree* _tree);


static DoToOrder	DoOrderFunc[3] = {DoPreOrder, DoInOrder, DoPostOrder};


/*** for printing - in debug mode only ***/
#ifdef DEBUG

/* print in particular order */
typedef void	(*PrintToOrder)	(TreeItr _itr, PrintItemFunc _printItemFunc);

static void 	PrintO	(Tree* _tree, PrintToOrder _printOrderFunc, PrintItemFunc _printItemFunc);

static void		PrintPreOrder	(TreeItr _itr, PrintItemFunc _printItemFunc);
static void		PrintInOrder	(TreeItr _itr, PrintItemFunc _printItemFunc);
static void		PrintPostOrder	(TreeItr _itr, PrintItemFunc _printItemFunc);

static PrintToOrder	PrintOrderFunc[3] = {PrintPreOrder, PrintInOrder, PrintPostOrder};
#endif /* DEBUG */
/*----------------------------------------------------------*/


/*----------------------------------------------------------*/
/*						API functions						*/
/*----------------------------------------------------------*/


/* create tree, includes tree structure. return NULL for failure */
Tree* 		TreeCreate		(CmpFunc _cmpFunc)
{
	Tree* newTree = NULL;
	
	assert(_cmpFunc);
	
	newTree = (Tree*)malloc(sizeof(Tree));
	if(!newTree) {
		return NULL;
	}
	
	newTree->m_root.m_data		= (TreeData)MAGIC;
	newTree->m_root.m_parent	= NULL;
	ROOTNODE(newTree)			= NULL;
	newTree->m_root.m_Sons[1]	= NULL;
	
	newTree->m_cmpFunc = _cmpFunc;
	
	return newTree;
}
/*----------------------------------------------------------*/


/* destroy - going by post-order */
void 		TreeDestroy		(Tree* _tree)
{	
	TreeItr toRemove, next;
	
	assert(_tree);
	toRemove = TreeBegin(_tree);
	/* free all nodes */
	while( toRemove != TreeEnd(_tree) )
	{
		next = TreeNextPost(toRemove);
		TreeRemove(toRemove);
		toRemove = next;
	}
	free(_tree);
}
/*----------------------------------------------------------*/


/* insert data & return iterator to it. for error return TreeEnd (stub) */
/* if data already exists, return its iterator */
TreeItr 	TreeInsert		(Tree* _tree, TreeData _data)
{
	TreeItr newNode, parent;
	int compare;
	
	assert(_tree);
	/* find parent node */
	parent = NodeFind(_tree, _data);
	/* if tree is empty - insert as left son of stub */
	if( parent == TreeEnd(_tree) )
	{
		compare = -1;
	}
	else {
		/* compare data */
		compare = COMPARE( _tree, _data, TreeGetData(parent) );
		/* if data already there */
		if(0 == compare) {
			return parent;
		}
	}
	
	/* allocate new node & link to parent */
	newNode = NodeCreate(parent, _data);
	if(!newNode) {
		return TreeEnd(_tree);
	}
	/* link from parent, on left/right according to compare */
	if(compare > 0) {
		RIGHT_NODE(parent) = newNode;
	} else {
		LEFT_NODE(parent) = newNode;
	}
	return newNode;
}
/*----------------------------------------------------------*/


/* count # of items */
size_t		TreeCount		(Tree* _tree)
{
	TreeItr itr;
	size_t count = 0;
	
	assert(_tree);
	itr = TreeBegin(_tree);
	while( itr != TreeEnd(_tree) )
	{
		++count;
		itr = TreeNext(itr);
	}
	return count;
}
/*----------------------------------------------------------*/


/* search for data, return iterator, if not found return end stub */
TreeItr		TreeFind		(Tree *_tree, TreeData _data)
{
	TreeItr *itr;
	int compare;
	
	assert(_tree);
	
	itr = NodeFind(_tree, _data);
	if( TreeGetData(itr) == (TreeData)MAGIC ) {
		return TreeEnd(_tree);
	}
	compare = COMPARE( _tree, _data, TreeGetData(itr) );
	
	return (0 == compare) ? itr : TreeEnd(_tree);
}
/*----------------------------------------------------------*/


/* remove item by a given iterator & rearrange tree. return NULL for error */
TreeData	TreeRemove		(TreeItr _itr)
{
	TreeItr temp = NULL;
	TreeData data;
	
	data = TreeGetData(_itr);
	
	/* if attempting to remove stub */
	if( data == (TreeData)MAGIC ) {
			return NULL;
	}
	
	/* if _itr has no children */
	if( !LEFT(_itr) && !RIGHT(_itr) )
	{
		LinkSonParent(_itr, 0);
	} /* if _itr has only left child */
	else if ( LEFT(_itr) && !RIGHT(_itr) )
	{
		LinkSonParent(_itr, 0);
	} /* if only right child */
	else if ( RIGHT(_itr) && !LEFT(_itr) )
	{
		LinkSonParent(_itr, 1);
	}/* if _itr has 2 children */
	else {
		temp = TreeNext(_itr);
		DATA(_itr) = DATA(temp);
		/* connect right child of temp to its parent */
		LinkSonParent(temp, 1);
		
	} /* end outer if-else (# of children) */
	
	free(temp);
	return data;
}
/*----------------------------------------------------------*/


/* return iterator to 1st item (leftmost), if empty tree return stub */
TreeItr		TreeBegin		(Tree* _tree)
{
	TreeItr* itr = (TreeItr)ROOTNODE(_tree);
	
	assert(_tree);
	/* no left son to root means empty tree - return */
	if(!itr) {
		return (TreeItr)&(_tree->m_root);
	}
	while( LEFT(itr) )
	{
		itr = LEFT(itr);
	}
	
	return itr;
}
/*----------------------------------------------------------*/


/* return end stub (root) */
TreeItr		TreeEnd			(Tree* _tree)
{
	assert(_tree);
	return (TreeItr)&(_tree->m_root);
}
/*----------------------------------------------------------*/


/* return iterator of following item - smallest larger than _itr->data */
/* going out of bounds results in UNDEFINED BEHAVIOR */
TreeItr		TreeNext		(TreeItr _itr)
{
	TreeItr next = _itr;
	TreeItr tempItr;
	
	/* if _itr is end stub */
	if( TreeGetData(_itr) == (TreeData)MAGIC) {
		return NULL;
	}
	
	/* if there is a right son, go to it and then left */
	if( RIGHT(next) )
	{
		next = RIGHT(next);
		while( LEFT(next) ) {
			next = LEFT(next);
		}
		return next;
	} /* end if(right(next)) */
	
	/* else: go to parent */
	tempItr = PARENT(next);
	/* if parent is to the left - go higher */
	while( RIGHT(tempItr) == next )
	{
		next = tempItr;
		tempItr = PARENT(next);
		/* if reached end - return */
		if( TreeGetData(tempItr) == (TreeData)MAGIC )
		{
			return tempItr;
		}
	} /* end while(right(tempItr)==next) */

	/* if some ancestor is to the right - this is the next */
	return tempItr;
}
/*----------------------------------------------------------*/


/* return iterator of previous item - largest smaller than _itr->data */
/* going out of bounds results in UNDEFINED BEHAVIOR */
TreeItr		TreePrev		(TreeItr _itr)
{
	TreeItr prev = _itr;
	TreeItr tempItr;
	
	/* if there is a left son, go to it and then right */
	if( LEFT(prev) )
	{
		prev = LEFT(prev);
		while( RIGHT(prev) ) {
			prev = RIGHT(prev);
		}
		return prev;
	} /* end if(left(prev)) */
	
	/* else: go to parent */
	tempItr = PARENT(prev);
	/* if tree is empty (_itr is stub) */
	if(!tempItr) {
		return NULL;
	}
	/* if parent is to the right - go higher */
	while( LEFT(tempItr) == prev )
	{
		prev = tempItr;
		tempItr = PARENT(prev);
		/* if reached end - return NULL */
		if( TreeGetData(tempItr) == (TreeData)MAGIC )
		{
			return NULL;
		}
	} /* end while(left(tempItr)==prev) */

	/* if some ancestor is to the left - this is the prev */
	return tempItr;
}
/*----------------------------------------------------------*/


/* get data in _itr, NULL for error */
TreeData	TreeGetData		(TreeItr _itr)
{
	if(!_itr) {
		return NULL;
	}
	return DATA(_itr);
}
/*----------------------------------------------------------*/


/* apply _doFunc to all tree nodes, going in _trvMode order */
/* calling DoOrder, which calls the appropriate function according to _trvMode */
/* then calling TreeShake to rearrange tree as needed */
/* note: does not handle cases where _doFunc causes duplicated data */
void	TreeForEach		(Tree* _tree, TraverseMode _trvMode, DoFunc _doFunc, void* _params)
{
	assert(_tree);
	/* if tree is empty */
	if( TreeBegin(_tree) == TreeEnd(_tree) ) {
		return;
	}
	/* apply _doFunc */
	DoOrder(_tree, _doFunc, DoOrderFunc[_trvMode], _params);
	/* rearrange tree */
	TreeShake(_tree);
}
/*----------------------------------------------------------*/


/* print - for testing */
void	TreePrint		(Tree* _tree, PrintItemFunc _printItemFunc, TraverseMode _trvMode)
{
#ifdef DEBUG
	char *order[] = { "Pre-Order", "In Order", "Post-Order"};

	assert(_tree);
	
	printf("\nTree, %s:\n", order[_trvMode]);
	PrintO(_tree, PrintOrderFunc[_trvMode], _printItemFunc);
	printf("\n");
#endif /* DEBUG */
}
/*----------------------------------------------------------*/


/* iterative in-order tree traversal using stack */
/* algorithm - go left while you can, pushing into stack, */
/* when can't - pop, print & go right from popped */
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
	
	while( itr || !StackIsEmpty(stack))
	{
		while( itr )
		{
			StackPush(stack, itr);
			itr = LEFT(itr);
		}
		if( !StackIsEmpty(stack) )
		{
			itr = StackPop(stack);
			_printFunc(DATA(itr));
			itr = RIGHT(itr);	
		}
	}
}
/*----------------------------------------------------------*/



/* iterative post-order tree traversal using stack */
void TreePrintPostIterStack(Tree *_tree, PrintItemFunc _printFunc)
{
	TreeItr itr;
	Stack* stack;
	
	assert(_tree);
	stack = StackCreate();
	if(!stack) {
		return;
	}
	
	itr = (TreeItr)(ROOTNODE(_tree));
	
	while( itr || !StackIsEmpty(stack))
	{
		
		while( itr )
		{
			StackPush(stack, itr);
			itr = LEFT(itr);
		}

		if( !StackIsEmpty(stack) )
		{
			itr = StackTop(stack);
			itr = RIGHT(itr);
			
			if(!itr)
			{
				itr = StackPop(stack);
				_printFunc(DATA(itr));
				itr = RIGHT(StackTop(stack));
			}
		}
		
		
		
	}
}
/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* create a tree node from data and a known parent */
TreeItr	NodeCreate	(Node* _parent, TreeData _data)
{
	Node* newNode	= NULL;
	
	newNode = (Node*)malloc(sizeof(Node));
	if(!newNode) {
		return NULL;
	}
	
	DATA(newNode) 			= _data;
	PARENT_NODE(newNode)	= _parent;
	LEFT_NODE(newNode)		= NULL;
	RIGHT_NODE(newNode)		= NULL;
	
	return (TreeItr)newNode;
}
/*----------------------------------------------------------*/


/* general purpose tree search */
/* returns either the iterator of the node with _data, or the parent where it can be inserted */
TreeItr	NodeFind	(Tree* _tree, TreeData _data)
{
	int compare;
	TreeItr* itr = (TreeItr)ROOTNODE(_tree);
	
	assert(_tree);
	/* if tree is empty */
	if(!itr)
	{
		return TreeEnd(_tree);
	}
	
	compare = COMPARE( _tree, _data, TreeGetData(itr) );
	while( compare )
	{	
		if( compare < 0 ) {
			if(!LEFT(itr)) {
				return itr;
			}
			itr = LEFT(itr);
		}
		else {
			if(!RIGHT(itr)) {
				return itr;
			}
			itr = RIGHT(itr);
		}
		compare = COMPARE( _tree, _data, TreeGetData(itr) );
	} /* end while(compare) */
	
	return itr;
}
/*----------------------------------------------------------*/


/* link son of _itr on side _side (0 - left, 1- right) to parent of _itr instead of _itr */
/* for use in TreeRemove */
void LinkSonParent(TreeItr _itr, int _side)
{
	/* if _itr is the left son */
	if( LEFT(PARENT(_itr)) == _itr )
	{
		LEFT_NODE(PARENT(_itr)) = ((Node*)_itr)->m_Sons[_side];
	}
	else { /* --> is a right son */
		RIGHT_NODE(PARENT(_itr)) = ((Node*)_itr)->m_Sons[_side];
	}
	/* if son is not NULL, link from son to parent as well */
	if( ((Node*)_itr)->m_Sons[_side] ) {
		PARENT_NODE( ((Node*)_itr)->m_Sons[_side] ) = PARENT_NODE(_itr);
	}
}
/*----------------------------------------------------------*/


/* go to the next node in Post-Order: l->r->p; iterative function */
/* if _itr is a left child and there is a right child, go to its leftmost leaf, else to parent */
/* for use in tree destroy */
TreeItr TreeNextPost(TreeItr _itr)
{
	TreeItr* nextPost = NULL;
	
	/* if no right child or _itr is right child */
	if( !RIGHT(PARENT(_itr)) || RIGHT(PARENT(_itr)) == _itr )
	{
		nextPost = PARENT(_itr);
	}
	else { /* else - find next leaf */
		nextPost = RIGHT(PARENT(_itr));
		
		while( LEFT(nextPost) || RIGHT(nextPost) )
		{
			if( LEFT(nextPost) )
			{
				nextPost = LEFT(nextPost);
			} else
			{
				nextPost = RIGHT(nextPost);
			}
		} /* end while(left||right) */
	} /* end outer if-else */
	
	return nextPost;
}
/*----------------------------------------------------------*/


/****** for ForEach function ******/


/* call a function which applies _doFunc with _params to all tree nodes */
void DoOrder(Tree* _tree, DoFunc _doFunc, DoToOrder _doOrderFunc, void* _params)
{
	_doOrderFunc(ROOTNODE(_tree), _doFunc, _params);
}
/*----------------------------------------------------------*/


/* apply _doFunc in Pre-Order */
void DoPreOrder(TreeItr _itr, DoFunc _doFunc, void* _params)
{
	if( !_itr ) {		/* stopping condition */
		return;
	}
	
	_doFunc(_itr, _params);		/* apply _doFunc */
	DoPreOrder( LEFT(_itr), _doFunc, _params);	/* recursion - left */
	DoPreOrder( RIGHT(_itr), _doFunc, _params);	/* recursion - right */
	return;
}
/*----------------------------------------------------------*/


/* apply _doFunc In Order */
void DoInOrder(TreeItr _itr, DoFunc _doFunc, void* _params)
{
	if( !_itr ) {		/* stopping condition */
		return;
	}
	
	DoInOrder( LEFT(_itr), _doFunc, _params);	/* recursion - left */
	_doFunc(_itr, _params);		/* apply _doFunc */
	DoInOrder( RIGHT(_itr), _doFunc, _params);	/* recursion - right */
	return;
}
/*----------------------------------------------------------*/


/* apply _doFunc in Post-Order */
void DoPostOrder(TreeItr _itr, DoFunc _doFunc, void* _params)
{
	if( !_itr ) {		/* stopping condition */
		return;
	}
	
	DoPostOrder( LEFT(_itr), _doFunc, _params);		/* recursion - left */
	DoPostOrder( RIGHT(_itr), _doFunc, _params);	/* recursion - right */
	_doFunc(_itr, _params);		/* apply _doFunc */
	return;
}
/*----------------------------------------------------------*/



/* rearrange a tree after changes to values */
/* based on shake sort */
void TreeShake(Tree* _tree)
{
	int swapped = 0;
	TreeData data1, data2;
	TreeItr itr=NULL, itr2=NULL;
	TreeItr start = TreeBegin(_tree), end = TreePrev(TreeEnd(_tree));
	
	do {
		/* bubble sort - foreward */
		for( itr = start, swapped = 0; itr != end; itr = itr2 )
		{
			itr2 = TreeNext(itr);
			data1 = TreeGetData(itr);
			data2 = TreeGetData(itr2);
			
			if( COMPARE(_tree, data1, data2) > 0 )
			{
				DATA(itr)  = data2;
				DATA(itr2) = data1;
				swapped = 1;
			}
		} /* end for */
			
		if(!swapped) {
			break;
		}
		
		end = TreePrev(end);
		/* bubble sort - backward */
		for( itr = end, swapped = 0; itr != start; itr = itr2 )
		{
			itr2 = TreePrev(itr);
			data1 = TreeGetData(itr);
			data2 = TreeGetData(itr2);
			if( COMPARE(_tree, data1, data2) < 0 )
			{
				DATA(itr)  = data2;
				DATA(itr2) = data1;
				swapped = 1;
			}
		} /* end for */
		
		start = TreeNext(start);
		
	} while(swapped);
}
/*----------------------------------------------------------*/


/****** for printing ******/

#ifdef DEBUG

/* call a PrintOrder function with the appropriate PrintItem function */
void PrintO(Tree* _tree, PrintToOrder _printOrderFunc, PrintItemFunc _printItemFunc)
{
	_printOrderFunc(ROOTNODE(_tree), _printItemFunc);
}
/*----------------------------------------------------------*/


/* print tree items in order of: p -> l -> r (recursively) */
void PrintPreOrder(TreeItr _itr, PrintItemFunc _printItemFunc)
{
	TreeData data;

	if( !_itr ) {		/* stopping condition */
		return;
	}

	data = TreeGetData(_itr);
	_printItemFunc(data);		/* print - starting from root */		

	PrintPreOrder( LEFT(_itr), _printItemFunc);		/* recursion - left */
	PrintPreOrder( RIGHT(_itr), _printItemFunc);	/* recursion - right */
	return;
}
/*----------------------------------------------------------*/


/* print tree items in order of: l -> p -> r (recursive) */
void PrintInOrder(TreeItr _itr, PrintItemFunc _printItemFunc)
{
	TreeData data;

	if( !_itr ) {			/* stopping condition */
		return;
	}

	PrintInOrder( LEFT(_itr), _printItemFunc);		/* recursion - left - all the way in */

	data = TreeGetData(_itr);
	_printItemFunc(data);			/* print */		

	PrintInOrder( RIGHT(_itr), _printItemFunc);		/* recursion - right */
	return;
}
/*----------------------------------------------------------*/


/* print tree items in order of: l -> r -> p (recursive) */
void PrintPostOrder(TreeItr _itr, PrintItemFunc _printItemFunc)
{
	TreeData data;
	
	if( !_itr ) {			/* stopping condition */
		return;
	}

	PrintPostOrder( LEFT(_itr), _printItemFunc);		/* recursion - left - all the way in */
	PrintPostOrder( RIGHT(_itr), _printItemFunc);	/* recursion - right */

	data = TreeGetData(_itr);
	_printItemFunc(data);			/* print */		
	return;
}
/*----------------------------------------------------------*/

#endif /* DEBUG */

