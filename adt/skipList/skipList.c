/**************************************************************************************
    Author : Tomer Dery
    Creation date :      25.02.14
    Date last modified : 25.02.14
    Description : skip list
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "list.h"
#include "skipList.h"


#define SUCCESS 1
enum {FALSE, TRUE}; 

#define RAND ( rand() % 2 )
#define RAND_IS_TRUE(RAND_RES)  (RAND_RES == 1) 

#define MIN_VAL (Data)INT_MIN
#define MAX_VAL (Data)INT_MAX

#define GET_MAIN_LIST(SKIP)   (SKIP->m_mainList)
#define GET_COMP(SKIP)   (SKIP->m_compFunc)

#define GET_MAIN_LIST_END(SKIP)  ( ListEnd( GET_MAIN_LIST(SKIP) ) )

#define GET_LOWEST_LIST(SKIP)  (  ListGetData( ListPrev( ListEnd( GET_MAIN_LIST(SKIP)   ) ) ) )

#define GET_ITR_DATA(ITR)     ( ( (DataWrapper*)ListGetData(ITR) )->m_data )
#define GET_ITR_DOWN(ITR)     ( ( (DataWrapper*)ListGetData(ITR) )->m_down )

#define LOWEST_LEVEL_DOWN_SIGN (NULL)
#define IS_ITR_LOWER_LEVEL(ITR)  (GET_ITR_DOWN(ITR) == LOWEST_LEVEL_DOWN_SIGN)

#define GET_UPPER_LIST(SKIP)  (  ListGetData( ListBegin( GET_MAIN_LIST(SKIP)   ) ) ) 
#define GET_SKIP_UPPER_BEGIN(SKIP)  (  ListBegin( GET_UPPER_LIST(SKIP) )  )  /* <=> upper min*/
#define GET_SKIP_UPPER_MAX(SKIP)   ( ListPrev( ListEnd( GET_UPPER_LIST(SKIP) ) ) ) 

#define GET_LIST_MAX(LIST)  ( ListPrev( ListEnd(LIST)) )

/*------------------------------------------------------------------------------------*/

typedef struct DataWrapper{
	Data m_data;
	ListItr m_down;
}DataWrapper;


struct SkipList{
	List* m_mainList;
	DataComp m_compFunc;
};

/*------------------------------------------------------------------------------------*/

static DataWrapper* CreateDataWrapper(Data _data, ListItr _down);
static List* CreateNewLevel(SkipList* _skipList, int _isLowerLevel);

static ListItr FindFirstGE(ListItr _from, DataComp _Comp, Data _data, int* _lastCompRes);

static int InsertRec( ListItr _from , DataComp _CompFunc, Data _data, ListItr* _itrInserted, ListItr* _itrLevelDown, int* _isExit);

/*------------------------------------------------------------------------------------*/

SkipList* SkipListCreate (DataComp _compFunc)
{      
	SkipList* skipList = NULL;	

	assert(_compFunc);
	
	skipList = calloc(1 , sizeof(SkipList));
	if(skipList == NULL){
		return NULL;
	}
	
	/*create main list*/
	skipList->m_mainList = ListCreate();
	if(skipList->m_mainList == NULL){
		free(skipList);
		return NULL;
	}
	
	/*create lower level list*/
	if( CreateNewLevel(skipList, TRUE) == NULL ){
		free(skipList->m_mainList);
		free(skipList);
	}
	
	skipList->m_compFunc = _compFunc;
		
	/*initialize rand seed*/
	srand (time(NULL));	
		
	return skipList;
}
/*------------------------------------------------------------------------------------*/

static List* CreateNewLevel(SkipList* _skipList, int _isLowerLevel)
{	
	List* newLevelList = NULL;	
	ListItr itrMinDown = LOWEST_LEVEL_DOWN_SIGN;
	ListItr itrMaxDown = LOWEST_LEVEL_DOWN_SIGN;		
	
	/*create new level list*/
	newLevelList = ListCreate();
	if(newLevelList == NULL){
		return NULL;
	}
	
	/*set connections to current highest level*/
	if(!_isLowerLevel){
		itrMinDown = GET_SKIP_UPPER_BEGIN(_skipList);
		itrMaxDown = GET_SKIP_UPPER_MAX(_skipList);
	}
	
	/*insert min val and max val to new level list*/
	ListPushHead(newLevelList , (Data)CreateDataWrapper(MIN_VAL, itrMinDown));
	ListPushTail(newLevelList , (Data)CreateDataWrapper(MAX_VAL, itrMaxDown));
	
	/*insert next level list to main list*/
	ListPushHead( GET_MAIN_LIST(_skipList), newLevelList);
	
	return newLevelList;
}

/*------------------------------------------------------------------------------------*/

static DataWrapper* CreateDataWrapper(Data _data, ListItr _down)
{
	DataWrapper* dataWrapper = NULL;
	
	dataWrapper = malloc( sizeof(DataWrapper) );
	if (dataWrapper == NULL){
		return NULL;
	}
	
	dataWrapper->m_data = _data;
	dataWrapper->m_down = _down;
	
	return dataWrapper;
}

/*------------------------------------------------------------------------------------*/

void SkipListDestroy(SkipList * _skipList)
{
	
}

/*------------------------------------------------------------------------------------*/

SkipListItr SkipListInsert (SkipList* _skipList, Data _data)
{
	int res, isExit = FALSE ;
	ListItr itrInserted;
	ListItr itrLevelDown;
	List* newLevelList = NULL;
	
	res = InsertRec( GET_SKIP_UPPER_BEGIN(_skipList), GET_COMP(_skipList), _data, &itrInserted, &itrLevelDown, &isExit); 
	
	/* check if need to add new top level */
	if( RAND_IS_TRUE(res) && !isExit ){
		newLevelList = CreateNewLevel(_skipList, FALSE);
		ListInsert(  GET_LIST_MAX(newLevelList) , (Data)CreateDataWrapper(_data, itrLevelDown));
	}
	
 	return itrInserted;

}

/*------------------------------------------------------------------------------------*/


/*returns rand : 1 => insert to next upper level as well*/
 
static int InsertRec( ListItr _from , DataComp _CompFunc, Data _data, ListItr* _itrInserted, ListItr* _itrLevelDown, int* _isExit) 
{
	int cmpRes, randRes;
	ListItr itr = _from;

	/*advance right while itr data is smaller then data*/
	while( (cmpRes = _CompFunc( GET_ITR_DATA(itr), _data) )  < 0 ){
		itr = ListNext(itr);
	}
	
	/*lower level - stop condition*/
	if(  IS_ITR_LOWER_LEVEL(itr)  ){  
		if ( cmpRes == 0){
			/*duplicate !! not suposse to happen*/
			return 0; 
		}	
		
		/*insert to lowest level and save inserted iterator*/ 			
		*_itrInserted = ListInsert (itr, (Data)CreateDataWrapper(_data, LOWEST_LEVEL_DOWN_SIGN));
		*_itrLevelDown = *_itrInserted;
			
		return RAND;
	}

	/* REC to next level => down from first smaller (prev iter) */ 
	randRes = InsertRec( GET_ITR_DOWN( ListPrev(itr) ), _CompFunc, _data, _itrInserted, _itrLevelDown, _isExit); 
		
	/*on the way back from rec*/
	if( RAND_IS_TRUE(randRes) && !(*_isExit) ){
		/*insert and rand again (itrLevelDown <= new inserted node)*/ 
		*_itrInserted = ListInsert (itr, (Data)CreateDataWrapper(_data, *_itrInserted));			
		
		return RAND;   
	}
	else{
		*_isExit = TRUE;
		return 0;         				    /*stop!!!*/
	}
}

/*------------------------------------------------------------------------------------*/

static ListItr FindFirstGE(ListItr _from, DataComp _Comp, Data _data, int* _lastCompRes)
{
	ListItr itr = _from;
	
	while(TRUE){
		/*advance right while next is smaller then data*/
		while( (*_lastCompRes = _Comp( GET_ITR_DATA( ListNext(itr) ), _data) ) < 0 ){
			itr = ListNext(itr);
		}
	
		/*data was found || lower level GE*/
		if(*_lastCompRes == 0 || IS_ITR_LOWER_LEVEL(itr) ){
			return ListNext(itr);
		}
		else 
		{
			/*advance down from closest smaller itr*/
			itr = GET_ITR_DOWN( itr );
		}	
	}
}

/*------------------------------------------------------------------------------------*/

SkipListItr SkipListFind(SkipList* _skipList, Data _data)
{
	int cmpResult = 0;
	ListItr itrFound;
	
	itrFound = FindFirstGE( GET_SKIP_UPPER_BEGIN(_skipList), GET_COMP(_skipList), _data, &cmpResult); 
	
	if(cmpResult != 0){
		/*not found => return skipList End*/
		return SkipListEnd(_skipList);
	}
	
	return itrFound;
}

/*------------------------------------------------------------------------------------*/

Data SkipListGetData(SkipListItr _itr)
{
	return GET_ITR_DATA(_itr);
}

/*------------------------------------------------------------------------------------*/
/*
Data SkipListRemove (SkipList * _skipList, SkipListItr _itr)
{
	FindFirstGE
	remove all the way down
}
*/
/*------------------------------------------------------------------------------------*/

SkipListItr SkipListEnd	 (SkipList * _skipList)
{
	return ListEnd(GET_LOWEST_LIST(_skipList));
}

/*------------------------------------------------------------------------------------*/

SkipListItr SkipListBegin(SkipList * _skipList)
{
	return ListBegin(GET_LOWEST_LIST(_skipList));
}

/*------------------------------------------------------------------------------------*/

size_t		SkipListCountItems  (SkipList * _skipList);




/*------------------------------------------------------------------------------------*/
/*
int	SkipListForEach(SkipListItr _from, SkipListItr _to, DoFunc _do, Param)
{

}
*/
/*------------------------------------------------------------------------------------*/
/*
void SkipListPrint(SkipList * _skipList, PrintFunc _printFunc)
{
	//TODO    ListForEach( GET_MAIN_LIST(SKIP) , _printFunc , NULL );
}
*
/*------------------------------------------------------------------------------------*/

/*for DEBUG only*/
void PrintSkip(SkipList * _skipList)
{	
	List* lowerList = GET_LOWEST_LIST(_skipList);
	ListItr itr;
	Data data;
	
	for ( itr = ListBegin(lowerList) ; itr != ListEnd(lowerList) ; itr = ListNext(itr)  ) {
		data = GET_ITR_DATA(itr);
		printf("%d\n", (int)data);
	}
	
}

/*------------------------------------------------------------------------------------*/

/*for DEBUG only*/
void PrintSkipAllLevels(SkipList * _skipList)
{	
	List* curList;
	ListItr curListItr; 
	ListItr itr;
	Data data;

	
	for (curListItr = ListBegin( GET_MAIN_LIST(_skipList)) ; curListItr != GET_MAIN_LIST_END(_skipList); curListItr = ListNext(curListItr ) ){ 
		curList = ListGetData(curListItr);
		for ( itr = ListBegin(curList) ; itr != ListEnd(curList) ; itr = ListNext(itr)  ) {
			data = GET_ITR_DATA(itr);
			printf("%d\n", (int)data);
		}
		printf("-------------------\n");
	}
	
}

/*------------------------------------------------------------------------------------*/

int GetNumOfLevels(SkipList * _skipList)
{
	return ListCountItems(GET_MAIN_LIST(_skipList));
}

/*------------------------------------------------------------------------------------*/

