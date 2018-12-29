/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-21    
    Last modified date:		2013-07-21
    Description: creating & testing IsTreePerfect function
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "TreeFunc.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

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

static void 	TreePerfectImp		(TreeNode *_root, int* _isPerfect, int* _treeHeight, int* _currentLevel);
static void 	IsTreePerfect	(TreeNode *_root, int* _isPerfect, int* _treeHeight);

static Tree*	TreeMakePerfect();
static Tree*	TreeMakePerfectSmall();

static Tree*	TreeMakeNotPerfect1();
static Tree*	TreeMakeNotPerfect2();
static Tree*	TreeMakeNotPerfectSmall();


/*----------------------------------------------------------*/


/*** main ***/
int main()
{	
	int isPerfect=1, height=0;
	Tree *tree = NULL;
	char *msg, *msgS="Success", *msgF="Failure";


	printf("\n\t****** IsTreePerfect: Testing Started ******\n\n");


	/****** Perfect trees ******/

	printf("Testing on Perfect trees\n\n");


	tree = TreeMakePerfect();
	if(NULL==tree) {
		fprintf(stderr, "Error: allocation failed\n");
		return 1;
	}

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (isPerfect) ? msgS : msgF;

	printf("Perfect tree:\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;

		/*** small Perfect tree ***/

	tree = TreeMakePerfectSmall();
	if(NULL==tree) {
		fprintf(stderr, "Error: allocation failed\n");
		return 1;
	}

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (isPerfect) ? msgS : msgF;

	printf("Small Perfect tree (just root and 2 children):\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;

		/*** Perfect tree - just root ***/

	tree = TreeCreate();
	if(NULL==tree) {
		fprintf(stderr, "Error: allocation failed\n");
		return 1;
	}
	TreeInsert(tree, 10);

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (isPerfect) ? msgS : msgF;

	printf("Tree with just a root:\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;

	/****** not-Perfect trees ******/

	printf("\n\nTesting on not-Perfect trees\n\n");

		/*** not-Perfect #1 ***/

	tree = TreeMakeNotPerfect1();
	if(NULL==tree) {
		fprintf(stderr, "Error: allocation failed\n");
		return 1;
	}

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (!isPerfect) ? msgS : msgF;

	printf("Not-Perfect tree #1:\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;

		/*** not-Perfect #2 ***/
	
	tree = TreeMakeNotPerfect2();
	if(NULL==tree) {
		fprintf(stderr, "Error:\tallocation failed\n");
		return 1;
	}

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (!isPerfect) ? msgS : msgF;

	printf("Not-Perfect tree #2:\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;

		/*** small not-Perfect ***/

	tree = TreeMakeNotPerfectSmall();
	if(NULL==tree) {
		fprintf(stderr, "Error: allocation failed\n");
		return 1;
	}

	IsTreePerfect(tree->m_root, &isPerfect, &height);
	msg = (!isPerfect) ? msgS : msgF;

	printf("Small not-Perfect tree (just root and 1 node):\t%s\n\t# of levels: %d\n", msg, height);
	TreeDestroy(tree);
	tree = NULL;


	printf("\n\n\t****** Testing Finished ******\n\n");
	return 0;
	
	
	return 0;
}
/*----------------------------------------------------------*/


/********** IsTreePerfect functions ********/

/*** wrapper for recursive function ***/
void IsTreePerfect(TreeNode *_root, int* _isPerfect, int* _treeHeight)
{	
	int level=0;

	assert(_root);	
	*_isPerfect = 1;
	*_treeHeight = 0;
	
	TreePerfectImp(_root, _isPerfect, _treeHeight, &level);
}
/*----------------------------------------------------------*/

/* internal recursive function */
void TreePerfectImp(TreeNode *_root, int* _isPerfect, int* _treeHeight, int* _currentLevel)
{	
	/* stopping conditions */
	
	if(!_root) {
		*_isPerfect = 1;
		*_treeHeight = -1;
		return;
	}
	
	if(!*_isPerfect) {
		return;
	}
	
	if( !_root->m_right )
	{
		if( !_root->m_left && *_currentLevel == *_treeHeight ) {
			*_isPerfect = 1;
			return;
		}
		else {
			*_isPerfect = 0;
			++*_treeHeight;
			return;
		}
	}
	
	++*_currentLevel;
	
	if( *_treeHeight < *_currentLevel ) {
		++*_treeHeight;
	}
	
	TreePerfectImp(_root->m_left, _isPerfect, _treeHeight, _currentLevel);
	TreePerfectImp(_root->m_right, _isPerfect, _treeHeight, _currentLevel);
	
	--*_currentLevel;
}
/*----------------------------------------------------------*/



/********** tree creating functions for testing ********/



/* create a Perfect tree */
Tree *TreeMakePerfect()
{
	Tree *tree;

	tree = TreeCreate();
	if(NULL == tree) {
		return NULL;
	}

	TreeInsert(tree, 10);	/* root */

	TreeInsert(tree, 15);
	TreeInsert(tree, 5);

	TreeInsert(tree, 7);	/* children of 5 */
	TreeInsert(tree, 2);

	TreeInsert(tree, 12);	/* children of 15 */
	TreeInsert(tree, 18);

	TreeInsert(tree, 3);	/* children of 2 */
	TreeInsert(tree, 1);
	
	TreeInsert(tree, 6);	/* children of 7 */
	TreeInsert(tree, 8);

	TreeInsert(tree, 11);	/* children of 12 */
	TreeInsert(tree, 13);

	TreeInsert(tree, 17);	/* children of 18 */
	TreeInsert(tree, 19);

	return tree;
}
/*----------------------------------------------------------*/

/* create a small Perfect tree - just root & 2 children */
Tree *TreeMakePerfectSmall()
{
	Tree *tree;

	tree = TreeCreate();
	if(NULL == tree) {
		return NULL;
	}

	TreeInsert(tree, 10);	/* root */

	TreeInsert(tree, 15);
	TreeInsert(tree, 5);

	return tree;
}
/*----------------------------------------------------------*/

/* create a not-Perfect tree */
Tree *TreeMakeNotPerfect1()
{
	Tree *tree;

	tree = TreeCreate();
	if(NULL == tree) {
		return NULL;
	}

	TreeInsert(tree, 10);	/* root */

	TreeInsert(tree, 15);
	TreeInsert(tree, 5);

	TreeInsert(tree, 7);	/* children of 5 */
	TreeInsert(tree, 2);

	TreeInsert(tree, 11);	/* children of 15 */
	TreeInsert(tree, 18);

	TreeInsert(tree, 1);	/* children of 2 */
	TreeInsert(tree, 3);

	TreeInsert(tree, 6);	/* children of 7 */
	TreeInsert(tree, 9);

	return tree;
}
/*----------------------------------------------------------*/

/* create another not-Perfect tree */
Tree *TreeMakeNotPerfect2()
{
	Tree *tree;

	tree = TreeCreate();
	if(NULL == tree) {
		return NULL;
	}

	TreeInsert(tree, 10);	/* root */

	TreeInsert(tree, 15);
	TreeInsert(tree, 5);

	TreeInsert(tree, 7);	/* children of 5 */
	TreeInsert(tree, 2);

	TreeInsert(tree, 12);	/* children of 15 */
	TreeInsert(tree, 18);

	TreeInsert(tree, 1);	/* children of 2 */
	TreeInsert(tree, 3);

	TreeInsert(tree, 6);	/* children of 7 */
	TreeInsert(tree, 9);

	TreeInsert(tree, 11);	/* children of 12 */
	TreeInsert(tree, 13);

	return tree;
}
/*----------------------------------------------------------*/

/* create a small not-Perfect tree - just root & 1 child */
Tree *TreeMakeNotPerfectSmall()
{
	Tree *tree;

	tree = TreeCreate();
	if(NULL == tree) {
		return NULL;
	}

	TreeInsert(tree, 10);	/* root */
	TreeInsert(tree, 5);

	return tree;
}
/*----------------------------------------------------------*/



