/**************************************************************************************
    Author : Tomer Dery
    Creation date :      14.01.14
    Date last modified : 14.01.14
     Description : HashMap
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash.h"
#include "list.h"

/*------------------------------------------------------------------------------------*/

#define HASH_CELL(HASH, INDEX) HASH->m_hashTable[INDEX]
#define GET_INDEX_FRM_KEY(HASH,KEY) HASH->m_HashFunc(KEY, HASH->m_hashCapacity);

/*------------------------------------------------------------------------------------*/

enum {FALSE, TRUE}; 

/*------------------------------------------------------------------------------------*/

struct HashMap{
	List** m_hashTable;  /*array of pointers to list*/
	
	size_t m_hashCapacity;
	
	HashFunc m_HashFunc;
	CompFunc m_CompFunc;
};

typedef struct Pair{
	Key m_key;
	Data m_data;
}Pair;

/*------------------------------------------------------------------------------------*/
/*static funcs*/
int DoFuncDestroy(Data _data, Param _parm);
static List* GetBucketListOfKey(HashMap* _hash, Key _key);
static ListItr GetItrOfKeyInList(List* bucketList, Key _key, CompFunc _CompFunc);
static Pair* PairCreate(Key _key, Data _data);

/*------------------------------------------------------------------------------------*/

HashMap* HashCreate(size_t _size, HashFunc _HashFunc, CompFunc _CompFunc)
{   
	int i;
	HashMap* hash = NULL;		
	
	assert(_HashFunc);
	assert(_CompFunc);

	/*alloc hash*/
	hash = (HashMap*)calloc(1, sizeof(HashMap)); 
	if (hash == NULL){
		return NULL;
	}	

	/*alloc array of pointers to list*/
	hash->m_hashTable = (List**)calloc(_size , sizeof(List*)); 
	if (hash->m_hashTable  == NULL){
		free(hash);
		return NULL;
	}	

	/*initialize lists*/
	for(i=0; i < _size; i++){
		HASH_CELL(hash, i) = ListCreate();
		/*free all allocated lists if fail*/
		if(!HASH_CELL(hash, i)){
			while(i>0){
				--i;
				ListDestroy(HASH_CELL(hash, i));
			}
		}
	} 

	hash->m_hashCapacity = _size;  
	hash->m_HashFunc = _HashFunc; 
	hash->m_CompFunc = _CompFunc;    

	return hash;
}

/*------------------------------------------------------------------------------------*/

void HashDestroy(HashMap* _hash)
{
	List* curListBucket = NULL;
	DoFunc doFuncDestroy = DoFuncDestroy;
	int i;
	
	assert(_hash);
	
	for(i=0; i < _hash->m_hashCapacity; i++){
		curListBucket = HASH_CELL(_hash, i);
		/*free all pairs on list*/
		ListForEach(ListBegin(curListBucket), ListEnd(curListBucket) , doFuncDestroy, NULL);
		/*destroy list*/
		ListDestroy(curListBucket);
	}
	
	free(_hash->m_hashTable);
	free(_hash);
}

int DoFuncDestroy(Data _data, Param _parm)
{
	free(_data);
	return TRUE;
}

/*------------------------------------------------------------------------------------*/

Data HashInsert	(HashMap* _hash, Key _key, Data _data)
{
	Pair* pair = NULL;
	List* bucketList = NULL;
	
	assert(_hash);
	assert(_key);
	
	bucketList = GetBucketListOfKey(_hash, _key); 

	if(GetItrOfKeyInList(bucketList, _key, _hash->m_CompFunc) != NULL){
		return 0;
	}else{
		/*insert pair to list*/
		pair = PairCreate(_key, _data);
		if(pair == NULL){
			return 0;
		}
		
		ListPushTail(bucketList, pair);
	}

	return _data;
}

/*------------------------------------------------------------------------------------*/


typedef struct WraperMatchFunc{	
	CompFunc m_HashCompFunc;
    Data m_KeyToComp;
}WraperMatchFunc;


int MatchFuncPairs(Data _data1, Data _data2) /*data1 = hash pair , data2 = wraper*/ 
{
	Pair* pair = (Pair*)_data1;
	WraperMatchFunc* wraperMatchFunc = (WraperMatchFunc*)_data2;
	CompFunc compFunc = wraperMatchFunc->m_HashCompFunc;
	Key key = wraperMatchFunc->m_KeyToComp;

	return ( compFunc( pair->m_key , key ) == 0  );
}

static ListItr GetItrOfKeyInList(List* bucketList, Key _key, CompFunc _CompFunc)
{
	ListItr itrToRet;
	MatchFunc matchFuncPairs = MatchFuncPairs; 
	
	WraperMatchFunc* wraperMatchFunc = malloc(sizeof(WraperMatchFunc));	
	wraperMatchFunc->m_HashCompFunc = _CompFunc;
	wraperMatchFunc->m_KeyToComp = (Data)_key;
	
	itrToRet = ListFindFirst( ListBegin(bucketList), ListEnd(bucketList) , matchFuncPairs, wraperMatchFunc );	

	free(wraperMatchFunc);

	if(itrToRet != ListEnd(bucketList)){
		return itrToRet;
	}else{
		return NULL;
	}

	/***  ALTERNATIVE - WITHOUT WRAPER ***/
	/*	ListItr itr = NULL;
		Pair* pair = NULL;
		for(itr = ListBegin(bucketList); itr != ListEnd(bucketList); itr = ListNext(itr)){
			pair = (Pair*)ListGetData(itr);
			if(  _CompFunc( pair->m_key , _key) == 0 ){
				return itr;
			} 
		}
	*/	
	return NULL;
}

/*------------------------------------------------------------------------------------*/

static Pair* PairCreate(Key _key, Data _data)
{
	Pair* pair = (Pair*) malloc ( sizeof(Pair));
	if(pair == NULL){
		return NULL;
	}
	
	pair->m_key = _key;
	pair->m_data = _data;
	
	return pair;
}


/*------------------------------------------------------------------------------------*/

Data HashFind (HashMap* _hash, Key _key)
{
	ListItr itr;
	Pair* pair = NULL;
	Data dataToRet;
	List* bucketList = NULL;
	
	assert(_hash);
	assert(_key);

	bucketList = GetBucketListOfKey(_hash, _key); 

	itr = GetItrOfKeyInList(bucketList, _key, _hash->m_CompFunc);  
	pair = (Pair*)ListGetData(itr);
	dataToRet = pair->m_data; 

	return dataToRet;
}

/*------------------------------------------------------------------------------------*/

static List* GetBucketListOfKey(HashMap* _hash, Key _key)
{
	int indexToInsert; 
	List* bucketList = NULL; 
	
	indexToInsert = GET_INDEX_FRM_KEY(_hash, _key);
	bucketList = HASH_CELL(_hash, indexToInsert);  
	
	return bucketList;
}

/*------------------------------------------------------------------------------------*/

Data HashRemove	(HashMap* _hash, Key _key)
{
	ListItr itr;
	Pair* pair = NULL;
	Data* dataToRet = NULL;
	List* bucketList = NULL;
			
	assert(_hash);
	assert(_key);		
		
	bucketList = GetBucketListOfKey(_hash, _key); 	
			
	itr = GetItrOfKeyInList(bucketList, _key, _hash->m_CompFunc);
	if(!itr){  /*key wasnt found*/
		return NULL;
	}
	
	pair = (Pair*)ListGetData(itr); 
	dataToRet = pair->m_data;
	
	ListRemove(itr);
	
	free(pair);
	
	return dataToRet;
}

/*------------------------------------------------------------------------------------*/

typedef struct WraperHashDoFunc{	
	HashDoFunc m_HashDoFunc;
	Param m_param;
}WraperHashDoFunc;

int DoFuncHashWraper(Data _data, Param _param)  /*data is pair on list , parm is wraper*/
{
	WraperHashDoFunc* wraper = (WraperHashDoFunc*)_param;
	
	HashDoFunc hashDoFunc = wraper->m_HashDoFunc;	
	Param param = wraper->m_param;	

	Pair* pair = (Pair*)_data;
		
	return hashDoFunc(pair->m_key, pair->m_data, param);
}

int	HashForEach(HashMap* _hash, HashDoFunc _HashDoFunc, Param _param)
{
	int i, toRet=0;
	List* listBucket = NULL;
	WraperHashDoFunc* wraper = NULL;
	DoFunc doFuncHashWraper = DoFuncHashWraper;
	ListItr itr;

	assert(_hash);

	wraper = malloc(sizeof(WraperHashDoFunc));

	wraper->m_HashDoFunc = _HashDoFunc;
	wraper->m_param = _param;
	
	for(i=0; i < _hash->m_hashCapacity; i++){
		listBucket = HASH_CELL(_hash, i);
		itr = ListForEach(ListBegin(listBucket), ListEnd(listBucket) , doFuncHashWraper, wraper); 
		if (itr == ListEnd(listBucket)){
			++toRet;
		}
	}	

	free(wraper);	
	
	return toRet;
}

/*------------------------------------------------------------------------------------*/

int CountHashDoFunc(Key _key, Data _data, Param _param)
{
	++(*(int*)_param);
	return TRUE;
}

size_t HashCountItems(HashMap* _hash)
{
	int count=0;
	HashDoFunc countHashDoFunc = CountHashDoFunc;

	assert(_hash);

	HashForEach(_hash, countHashDoFunc, &count);
	
	return count;
}

/*------------------------------------------------------------------------------------*/

size_t HashCountBuckets(HashMap* _hash)
{
	int i,count=0;
	List* listBucket = NULL;
	
	assert(_hash);
	
	for(i=0; i < _hash->m_hashCapacity; i++){
		listBucket = HASH_CELL(_hash, i);
		if ( !ListIsEmpty(listBucket) ){
			++count;
		}
	}
	
	return count;
}

/*------------------------------------------------------------------------------------*/



