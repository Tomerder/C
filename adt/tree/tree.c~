/**************************************************************************************
    Author : Tomer Dery
    Creation date :      08.01.14
    Date last modified : 09.01.14
     Description : search tree - binaric tree - left is smaller then parent , right is bigger or equal.  
    			   mainly used for search => if balanced : log(n)	
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "stack.h"

/*------------------------------------------------------------------------------------*/

#define SONS_NUM 2

enum{LEFT_IND, RIGHT_IND} ;

#define TREE_STUB(TREE_PTR) (&(TREE_PTR->m_root))
#define TREE_ROOT(TREE_PTR) (TREE_STUB(TREE_PTR)->m_sons[LEFT_IND])

#define IS_EMPTY_TREE(TREE_PTR) (TREE_ROOT(TREE_PTR) ==  NULL)

#define LEFT_SON(NODE) (NODE->m_sons[LEFT_IND])
#define RIGHT_SON(NODE) (NODE->m_sons[RIGHT_IND])
#define PARENT(NODE) (NODE->m_parent)

#define IS_LEFT_SON(NODE) (LEFT_SON(PARENT(NODE)) == NODE)
#define IS_RIGHT_SON(NODE) (RIGHT_SON(PARENT(NODE)) == NODE)

#define COMP_FUNC(TREE_PTR) (TREE_PTR->m_CompFunc)

#define GET_SON_LR(NODE,IS_LEFT) (NODE->m_sons[1 ^ IS_LEFT])
#define IS_SON_LR(NODE,IS_LEFT)  (GET_SON_LR(PARENT(NODE), IS_LEFT) == NODE)

#define IS_NODE_STUB(NODE) ((PARENT(NODE)) == NULL)
#define PARENT_NOT_STUB(NODE) (PARENT(PARENT(NODE)) != NULL)

#define HAS_BOTH_SONS(NODE) ((LEFT_SON(NODE) != NULL) && (RIGHT_SON(NODE) != NULL))
/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE}; 


typedef struct Node Node;

/*------------------------------------------------------------------------------------*/

struct Node{
	Data m_data;
	Node* m_sons[SONS_NUM];
	Node* m_parent;
};


struct Tree{
	Node m_root; /*stub*/
	CompFunc m_CompFunc;
};


/*------------------------------------------------------------------------------------*/

/* returns the first node that is equal or should be the parent of _data
   beeing used from TreeInsert - will returns the node that should be the parent of _data
   and from TreeFind - if the node that is returned contains the same data then node has been found
   balanced: o(log n) - worst:o(n)*/ 
static Node* FindNodeFromData(Tree* _tree, Data _data);

/* used for creating new node that will contain the new data to insert and insert it as left\right son of _node 
   left/right is decided by _isInsertToLeft parameter - called from TreeInsert function
   balanced: o(log n) - worst:o(n)*/ 
static Node* InsertImp( Node* _node , Data _data , int _isInsertToLeft);

/* recursive functions - called from ForEach Function 
   executes _doFunc for every node at the wanted order
   o(n)*/
static int RecInOrder(Node* _startNode, DoFunc _DoFunc, Parm _parm);
static int RecPreOrder(Node* _startNode, DoFunc _DoFunc, Parm _parm);
static int RecPostOrder(Node* _startNode,  DoFunc _DoFunc, Parm _parm);

/*returns next/prev node of tree - called from TreeNext/TreePrev functions
  prev or next is decided by _isLeft parameter (TRUE=get next) 
  balanced: o(log n) - worst:o(n)*/ 
static Node* TreeNextPrevImp(TreeItr _iter, int _isNext);

/* returns the smallest/biggest item on tree - decided by _isLeft(smallest if TRUE) parameter
   called from TreeBegin/TreeLargest functions
   balanced: o(log n) - worst:o(n)*/ 
static Node* TreeBeginEndImp(Tree* _tree, int _isLeft);

/* removes _iter from tree and connectes its son and its father in order to fix the tree
   called from TreeRemove function
   o(1)*/  
static Data RemoveNodeWith1Son(TreeItr _iter, int hasLeftSon);

/*recursively prints the tree - is used for checking and debuging*/ 
static void PrintTreeImp(Node* _treeNode);
/*------------------------------------------------------------------------------------*/

Tree* TreeCreate(CompFunc _CompFunc)
{   
	Tree* tree;		

	assert(_CompFunc);

	tree = (Tree*)calloc(1, sizeof(Tree));    /*malloc : vector*/
	if (tree == NULL){
		return NULL;
	}	

	COMP_FUNC(tree) = _CompFunc;    

	return tree;
}

/*------------------------------------------------------------------------------------*/


void TreeDestroy(Tree* _tree){
	Node* curNode = NULL;
	Node* savNode = NULL;
	
	assert(_tree);
	
	curNode = TreeBegin(_tree);
	
	while( curNode != TREE_STUB(_tree) )
	{	
		savNode = TreeNext(curNode);
		TreeRemove(curNode);
		curNode = savNode; 
	}

	free(_tree);
}

/*------------------------------------------------------------------------------------*/


static Node* FindNodeFromData(Tree* _tree, Data _data )
{
	int lastCompResult;
	CompFunc compFunc = COMP_FUNC(_tree);
	Node* node = TREE_ROOT(_tree);
	Node* nextNode = NULL;

	if(node == NULL){
		return TREE_STUB(_tree);
	}
	

	lastCompResult = compFunc(node->m_data , _data) ;

	while(lastCompResult && (nextNode=GET_SON_LR(node, (lastCompResult>0) ))   ){
		node = nextNode;
		lastCompResult = compFunc(node->m_data , _data) ;
	}

	return node;
}

/*------------------------------------------------------------------------------------*/

static Node* InsertImp( Node* _nodeParent , Data _data, int _isInsertToLeft )
{
	Node* parentCurSon = NULL;
	
	Node* newNode = (Node*)malloc(sizeof(Node)); 
	if(newNode == NULL){
		return FALSE;
	}
	
	newNode->m_data = _data;
	PARENT(newNode)	 = 	_nodeParent;
	
	/*connect old son as son of new node*/
    parentCurSon = GET_SON_LR(_nodeParent, _isInsertToLeft);
	GET_SON_LR(newNode , _isInsertToLeft ) = parentCurSon;     
	GET_SON_LR(newNode , !_isInsertToLeft ) = NULL;        
	
	/*connect old parent as parent of new node*/ 
	GET_SON_LR(_nodeParent , _isInsertToLeft ) = newNode;

	/*connect new node as parent of old son*/ 	
	if(parentCurSon != NULL){
		PARENT(parentCurSon) = newNode;	
	}				

	return newNode;
}


/*------------------------------------------------------------------------------------*/

TreeItr TreeInsert(Tree* _tree, Data _data)
{
	CompFunc compFunc = COMP_FUNC(_tree);
	int isInsertToLeft = FALSE;
	Node* nodeParent = NULL;
	Node* newNode = NULL;
	
	assert(_tree);
	
	if(IS_EMPTY_TREE(_tree)){
		newNode = InsertImp( TREE_STUB(_tree) , _data, TRUE ); /*insert root*/
	}else{	
		nodeParent = FindNodeFromData(_tree, _data);
		isInsertToLeft = ( compFunc(nodeParent->m_data , _data) > 0 ); /*if node is bigger->insert to the left*/
		newNode = InsertImp(nodeParent, _data , isInsertToLeft);
	}
	
	return (TreeItr)(newNode);
}


/*------------------------------------------------------------------------------------*/


TreeItr TreeFind(Tree* _tree, Data _data)
{
	CompFunc compFunc;
	Node* nodeFound;
	
	assert(_tree);

	if(IS_EMPTY_TREE(_tree)){
		return TREE_STUB(_tree);  /*if empty tree*/
	}

	compFunc = COMP_FUNC(_tree);
	nodeFound = FindNodeFromData(_tree, _data);

	if( compFunc(nodeFound->m_data, _data)  ){
		return TREE_STUB(_tree);  /*if not found*/
	}

	return nodeFound;
}

/*------------------------------------------------------------------------------------*/

Data TreeGetData(TreeItr _iter)
{
	Node* node = NULL;
	
	assert(_iter);	
	
	node = (Node*)(_iter);
	
	return ( node->m_data );
}

/*------------------------------------------------------------------------------------*/

TreeItr TreeBegin(Tree* _tree)
{
	Node* node = NULL;
	
	assert(_tree);
	
	if(IS_EMPTY_TREE(_tree)){
		return TREE_STUB(_tree);
	}

	node = TreeBeginEndImp(_tree, TRUE);	

	return node;
}

TreeItr TreeLargest(Tree* _tree)
{	
	Node* node = NULL;

	assert(_tree);

	if(IS_EMPTY_TREE(_tree)){
		return TREE_STUB(_tree);
	}

	node = TreeBeginEndImp(_tree, FALSE);	

	return node;
}


TreeItr TreeEnd(Tree* _tree)
{
	return TREE_STUB(_tree);
}

TreeItr TreeRoot(Tree* _tree)
{
	return TREE_ROOT(_tree);
}


static Node* TreeBeginEndImp(Tree* _tree, int _isLeft)
{
	Node* node = TREE_STUB(_tree);
	
	if(node != NULL){
		node = TREE_ROOT(_tree);
	}	

	while( GET_SON_LR(node, _isLeft) != NULL ){
		node = GET_SON_LR(node, _isLeft);
	}

	return node;
}

/*------------------------------------------------------------------------------------*/

TreeItr TreeNext(TreeItr _iter)
{
	assert(_iter);	
	
	if(_iter == NULL || IS_NODE_STUB(((Node*)(_iter)))){
		return NULL;
	}
	
	return (TreeNextPrevImp(_iter, TRUE));
}


TreeItr TreePrev(TreeItr _iter)
{	
	assert(_iter);
	
	if(_iter == NULL){   /*  => prev of stub is the largest  */
		return NULL;
	}
		
	return (TreeNextPrevImp(_iter, FALSE));
}

/*comments are for next - opposite for prev */  
static Node* TreeNextPrevImp(TreeItr _iter, int _isNext)
{
	Node* node = (Node*)(_iter);
	
	if(GET_SON_LR(node , !_isNext) != NULL){    /*if has right son*/
		node = GET_SON_LR(node, !_isNext);
		while(GET_SON_LR(node, _isNext) != NULL){
			node = GET_SON_LR(node, _isNext);   /*returns to the most left of its sons*/
		}
	}else{
		/*if node is left son & its parent is not the stub - returns parent of node*/
		if(IS_SON_LR(node, _isNext) && PARENT_NOT_STUB(node)){ 
			node = PARENT(node); 
		}else{
		    /*otherwise goes up from parent to parent and returns the first parent which we got to from the left (from left son*/ 
			while(IS_SON_LR(node, !_isNext)){
				if(!PARENT_NOT_STUB(node)){
					break;				
				}
				node = PARENT(node);
			}
			node = PARENT(node); 
		}
	}
	
	return node;

}
/*------------------------------------------------------------------------------------*/

int TreeForEach(Tree* _tree, TraversMode _mode, DoFunc _DoFunc, Parm _parm)
{
	int toRet = 0;
	Node* startNode = NULL;
	
	assert(_tree);
	assert(_parm);
	assert(_DoFunc);

	if(_mode == IN_ORDER){
	    startNode = TREE_ROOT(_tree);
		toRet = RecInOrder(startNode, _DoFunc, _parm);
	}else if(_mode == PRE_ORDER){
		startNode = TREE_ROOT(_tree);
		toRet = RecPreOrder(startNode, _DoFunc, _parm);
	}else if(_mode == POST_ORDER){
		startNode = TREE_ROOT(_tree);
		toRet = RecPostOrder(startNode, _DoFunc, _parm);
	}

	return toRet;
}


static int RecInOrder(Node* _startNode,  DoFunc _DoFunc, Parm _parm)
{
	int toRet = 0;
	Node* nextNode = NULL;	

	if(_startNode == NULL){
		return 0;	
	}

	/*left*/
	nextNode = LEFT_SON(_startNode);
	RecInOrder(nextNode, _DoFunc, _parm); 

	/*parent*/
	toRet = _DoFunc( _startNode->m_data, _parm);

	/*right*/
	nextNode = RIGHT_SON(_startNode);
	RecInOrder(nextNode, _DoFunc, _parm); 

	return toRet;
}



static int RecPreOrder(Node* _startNode,  DoFunc _DoFunc, Parm _parm)
{
	int toRet = 0;
	Node* nextNode = NULL;	

	if(_startNode == NULL){
		return 0;	
	}

	/*parent*/
	toRet = _DoFunc( _startNode->m_data, _parm);

	/*left*/
	nextNode = LEFT_SON(_startNode);
	RecPreOrder(nextNode, _DoFunc, _parm); 

	/*right*/
	nextNode = RIGHT_SON(_startNode);
	RecPreOrder(nextNode, _DoFunc, _parm); 

	return toRet;
}


static int RecPostOrder(Node* _startNode,  DoFunc _DoFunc, Parm _parm)
{
	int toRet = 0;
	Node* nextNode = NULL;	

	if(_startNode == NULL){
		return 0;	
	}

	/*left*/
	nextNode = LEFT_SON(_startNode);
	RecPostOrder(nextNode, _DoFunc, _parm); 

	/*right*/
	nextNode = RIGHT_SON(_startNode);
	RecPostOrder(nextNode, _DoFunc, _parm); 

	/*parent*/
	toRet = _DoFunc( _startNode->m_data, _parm);

	return toRet;
}
/*------------------------------------------------------------------------------------*/

size_t TreeCount(Tree* _tree)
{
	int count = 0;	
	Node* node = NULL;  
	
	assert(_tree);

	node = TreeBegin(_tree);
	
	while(node != NULL  && (node != TREE_STUB(_tree)) ){
		++count;
		node = TreeNext(node);
	}
	
	return count;
}

/*------------------------------------------------------------------------------------*/

Data TreeRemove(TreeItr _iter)
{
	int hasLeftSon;	
	Data dataToReturn = NULL;
	Node* nodeToRemove = _iter;
	Node* nodeToRmvNxt = NULL;	

	assert(_iter);
	if(nodeToRemove == NULL || IS_NODE_STUB(nodeToRemove) ){
		return NULL;
	}

	if(! HAS_BOTH_SONS(nodeToRemove)){
		hasLeftSon = (LEFT_SON(nodeToRemove) != NULL) ;
		dataToReturn = RemoveNodeWith1Son(_iter, hasLeftSon);
	}else{
		/*find next*/ 		
		nodeToRmvNxt = TreeNextPrevImp(nodeToRemove, TRUE);	
		/*switch data with next*/
		dataToReturn = nodeToRemove->m_data;
		nodeToRemove->m_data = nodeToRmvNxt->m_data;
		/*remove next- can have only right son */
		RemoveNodeWith1Son(nodeToRmvNxt, FALSE);
	}

	return dataToReturn;
}


static Data RemoveNodeWith1Son(TreeItr _iter, int _hasLeftSon)
{
	Node* nodeToRemove = _iter;
	Node* parent = PARENT(nodeToRemove);
	Node* son = GET_SON_LR(nodeToRemove, _hasLeftSon);
	Data dataToReturn = nodeToRemove->m_data;
	int isNodeToRmvLeftSon;

	/*connect old parent as parent of old son*/
	if(son !=NULL){
		PARENT(son) = parent;
	}

	/*connect old son as son of old parent - left/right according to removed node*/
	isNodeToRmvLeftSon = IS_LEFT_SON(nodeToRemove);
	GET_SON_LR(parent, isNodeToRmvLeftSon) = son;
	
	free(nodeToRemove);

	return dataToReturn;
}


/*------------------------------------------------------------------------------------*/

/*Iterative preorder traverse using stack - complexity O(n)
  Algorithem in general lines:	
  while stack is not empty
  - if node != NULL : push node to stack and go to left
  - else: pop node from stack and execute func on it + go to right 	*/
int TreeIterInorderA(TreeItr _iter, DoFunc _DoFunc, Parm _parm)
{
	Stack* stack;
	Node* curNode = NULL;
	int toRet = 0;
	
	stack = StackCreate();

	curNode = _iter;
		
	while(!IsStackEmpty(stack) || curNode != NULL ){
		if( curNode != NULL){
			StackPush(stack, curNode);  /*stack PUSH*/
			curNode = LEFT_SON(curNode); /*continue - right son*/
		}else{
			/*we will get here if there is no left son*/
			StackPop(stack, (void**)&curNode);   /*stack POP*/
			toRet = _DoFunc(curNode->m_data, _parm); /*do - parent*/	
			curNode = RIGHT_SON(curNode);    /*continue - right son*/
		}
	}
	
	return toRet;
}

/*------------------------------------------------------------------------------------*/

/*Iterative preorder traverse using flags - complexity O(n) 
  Algorithem in general lines:	
  allways go to the most left
  when getting to leaf - turn goToLeftFlag off & go back up
  if got up from left son - do node & (if there is right son - go to right & turn goToLeftFlag on)
  if got up from right son - continue going up
 */
int TreeIterInorderB(TreeItr _iter, DoFunc _DoFunc, Parm _parm)
{
	Node* curNode = NULL;
	Node* nextNode = NULL;
	int toRet = 0;
	/*will be turned of when getting to leaf & turned on when going right*/
	int goToLeftMostFlag = TRUE;
	/*will determine if the node we got up to was allready done or not*/
	int isLastUpFromLeft = TRUE;

	curNode = _iter;
		
	while(   curNode   &&   !IS_NODE_STUB(curNode)   ){
		if( (  nextNode = LEFT_SON(curNode) )   &&  goToLeftMostFlag  ){
			/*get to left most - only if last move wasnt up*/			
			curNode = nextNode;	     
		}else{
			if(isLastUpFromLeft){
				/*if last step up was from left son - do this node*/
				toRet = _DoFunc(curNode->m_data, _parm); /*do node*/
			}else{
				/*if we got from right son we have already done that node , so we will continue to next parent*/ 
				isLastUpFromLeft = IS_LEFT_SON(curNode);
				curNode = PARENT(curNode); /*parent*/
				continue;
			}	
			
			if(  ( nextNode = RIGHT_SON(curNode) )  ){  /*right son*/
				curNode = nextNode;	   
				goToLeftMostFlag = TRUE;  /*now it is allowed to go left again*/
			}else{
				/*we will get here if leaf - go back up & forbide going left again*/
				isLastUpFromLeft = IS_LEFT_SON(curNode);
				curNode = PARENT(curNode); /*parent*/
				goToLeftMostFlag = FALSE;
			}
		}	
	}
	
	return toRet;
}

/*------------------------------------------------------------------------------------*/

void PrintTree(Tree* _tree)
{
	PrintTreeImp(TREE_ROOT(_tree));
}


void PrintTreeImp(Node* _treeNode)
{
	if(_treeNode == NULL){
		return;
	}

	if(LEFT_SON(_treeNode) != NULL){
		printf("%d l->" , *(int*)_treeNode->m_data);
		PrintTreeImp(LEFT_SON(_treeNode));
	}

	if(RIGHT_SON(_treeNode)  != NULL){
		printf("%d r->" , *(int*)_treeNode->m_data);
		PrintTreeImp(RIGHT_SON(_treeNode));
	}

	if(LEFT_SON(_treeNode) == NULL  && RIGHT_SON(_treeNode)  == NULL ){
		printf("%d " , *(int*)_treeNode->m_data);
	}
	
	printf("\n");
}

/*------------------------------------------------------------------------------------*/
