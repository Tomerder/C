/**************************************************************************************
    Author : Tomer Dery
    Creation date :      08.01.14
    Date last modified : 09.01.14
    Description : search tree - binaric tree - left is smaller then parent , right is bigger or equal.  
    			  mainly used for search => if balanced : log(n)	
***************************************************************************************/
#ifndef __TREE_H__
#define __TREE_H__

#include "adt.h"
/*----------------------------------------------------------------------------------*/

typedef void* TreeItr;

typedef struct Tree Tree;

/*order to pass over the tree - given as parameter to ForEach function*/
typedef enum{ PRE_ORDER , IN_ORDER , POST_ORDER }TraversMode;

/*----------------------------------------------------------------------------------*/
/*input parameter is compere function (see above) 
  output is a pointer to the created tree*/	
Tree* TreeCreate(CompFunc _CompFunc);          

/* goes over every item in tree from smallest to largest and remove it 
   frees all allocated objects*/ 
void TreeDestroy(Tree* _tree);

/* insert new tree node to the propreate location  (iterate left if data is smaller then node and right if bigger until location is found
   returns iterator to the inserted tree node
   complexity O(log n) if tree is balanced & O(n) at worst case*/   
TreeItr TreeInsert(Tree* _tree , Data _data);

/* searchs the tree for _data 
   returns iterator of node that has been found 	
   if not found - will return stub
   complexity O(log n) if tree is balanced & O(n) at worst case*/   
TreeItr TreeFind(Tree* _tree, Data _data);

/* returns num of items in tree 
    complexity O(n)	 */
size_t TreeCount(Tree* _tree);

/* removes iterator from tree and returns its data 
   complexity O(log n) if tree is balanced & O(n) at worst case*/   
Data TreeRemove(TreeItr _iter);

/* execute _DoFunc on every node of the tree - recursivly
   order is decided by TraversMode (see above) 
   complexity O(n)*/   
int TreeForEach(Tree* _tree, TraversMode _mode, DoFunc _DoFunc, Parm _parm);

/*----------------------------------------------------------------------------------*/

/*returns smallest item - will return stub if tree is empty
 complexity O(log n) if tree is balanced & O(n) at worst case*/    
TreeItr TreeBegin(Tree* _tree);

/*returns tree stub - (first dummy node that the real root of tree will be its left son 
  complexity O(1)	*/ 
TreeItr TreeEnd(Tree* _tree);

/*returns tree real root 
  complexity O(1)	*/ 
TreeItr TreeRoot(Tree* _tree);

/*returns biggest item - will return stub if tree is empty
 complexity O(log n) if tree is balanced & O(n) at worst case*/ 
TreeItr TreeLargest(Tree* _tree);

/*returns iterator of next item (the successor) of _iter   
  complexity O(log n) if tree is balanced & O(n) at worst case*/ 
TreeItr TreeNext(TreeItr _iter);

/*returns iterator of previous item (the predcesor) of _iter   
  complexity O(log n) if tree is balanced & O(n) at worst case*/ 
TreeItr TreePrev(TreeItr _iter);

/*returns data of the iterator 
  complexity O(1)	*/   
Data TreeGetData(TreeItr _iter);

/*----------------------------------------------------------------------------------*/

/*Iterative preorder traverse using stack - complexity O(n)
  Algorithem in general lines:	
  while stack is not empty
  - if node != NULL : push node to stack and go to left
  - else: pop node from stack and execute func on it + go to right 	*/
int TreeIterInorderA(TreeItr _iter, DoFunc _DoFunc, Parm _parm);

/*Iterative preorder traverse using flags - complexity O(n) 
  Algorithem in general lines:	
  allways go to the most left
  when getting to leaf - turn goToLeftFlag off & go back up
  if got up from left son - do node & (if there is right son - go to right & turn goToLeftFlag on)
  if got up from right son - continue going up
 */
int TreeIterInorderB(TreeItr _iter, DoFunc _DoFunc, Parm _parm);

/*----------------------------------------------------------------------------------*/

void PrintTree(Tree* _tree);



#endif /* #ifndef __TREE_H__ */
