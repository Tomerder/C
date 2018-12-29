/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-30
    Date last modified :	2013-08-02
    Description : Hash Map
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "DLLGen.h"
#include "HashMap.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define COMPARE(H,X,Y)	(H)->m_cmpFunc((X),(Y))
#define HASH(H,X,Y)		(H)->m_hashFunc((X),(Y))
#define TABLE(H)		((H)->m_hashTable)
#define SIZE(H)			((H)->m_size)
#define THRESHOLD(H)	(H)->m_threshold
#define STATS(X)		_hashMap->m_stats.X

/* for statistics */
typedef struct HashStats
{
	size_t		*m_bucketNumItems;	/* array of # of items in each bucket		*/
	size_t		m_nInsert;			/* # of insertions performed				*/
	size_t		m_nRemove;			/* # of removals performed					*/
} HashStats;


struct HashMap
{
	List*		*m_hashTable;	/* array of ptrs to Linked lists - the buckets	*/
	size_t		m_size;			/* # of buckets									*/
	size_t		m_threshold;	/* max # of items in bucket						*/
	
	CmpFunc		m_cmpFunc;		/* comparing keys								*/
	HashFunc	m_hashFunc;		/* assign index (bucket) by key					*/
	
	HashStats	m_stats;		/* statistics struct							*/
};

/* used to insert data+key as a single entry to hash table */
typedef struct Pair
{
	HashData	m_data;
	HashKey		m_key;
} Pair;

/* to send as a single paramter to doFunc of list */
typedef struct AuxTool
{
	DoFuncHash	m_doFuncHash;
	void*		m_params;
} AuxTool;


/* internal functions */

/* for searching */
static Itr		HashFindItr		(HashMap* _hashMap, HashKey _key, int *_index);
/* pair create & destroy */
static Pair* 	PairCreate		(HashData _data, HashKey _key);
static void 	PairDestroy		(Pair* _pair);
/* auxtool create & destroy */
static AuxTool* 	AuxCreate	(DoFuncHash _doFuncHash, void* _params);
static void			AuxDestroy	(AuxTool* _tool);
/* comparison function */
static int 		compPairFunc	(HashData* _data1, HashData* _data2);
/* for use in HashForEach */
static int		ListFunc		(HashData* _data, void* _params);

/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*						API functions						*/
/*----------------------------------------------------------*/


/* create hashmap, return NULL for failure */
HashMap*	HashCreate		(size_t _size, CmpFunc _cmpFunc, HashFunc _hashFunc)
{
	HashMap* map;
	int threshold;
	
	assert(_cmpFunc);
	assert(_hashFunc);
	
	/* if(0==_size) */
	assert(_size);
	
	map = (HashMap*)malloc(sizeof(HashMap));
	if(!map)
		goto HASH_FAILED;
	
	map->m_hashTable = (List**)calloc(_size,sizeof(List*));
	if( !map->m_hashTable )
		goto TABLE_ARRAY_FAILED;
	
	map->m_stats.m_bucketNumItems = (size_t*)calloc(_size, sizeof(size_t));
	if(!map->m_stats.m_bucketNumItems)
		goto BUCKET_ARRAY_FAILED;
	
	SIZE(map) = _size;
	map->m_cmpFunc = _cmpFunc;
	map->m_hashFunc = _hashFunc;
	map->m_stats.m_nInsert = 0;
	map->m_stats.m_nRemove = 0;
		
	/* set default threshold */
	threshold = (_size>20) ? _size/4 : 5;
	THRESHOLD(map) = HashSetThreshold(map, CONST, &threshold);
	
	return map;
	
/* in case of failure */
BUCKET_ARRAY_FAILED:
	free(map->m_hashTable);
	
TABLE_ARRAY_FAILED:
	free(map);
	
HASH_FAILED:
	return NULL;
}
/*----------------------------------------------------------*/


/* destroy hashmap, free memory */
void		HashDestroy		(HashMap* _hashMap)
{
	int i;
	Pair* pair = NULL;
	List* list = NULL;
	
	assert(_hashMap);

	/* for each list */
	for(i=0; i<SIZE(_hashMap); ++i)
	{
		list = TABLE(_hashMap)[i];
		if(list)
		{	/* free all list items */
			while( !ListIsEmpty(list) )
			{
				pair = (Pair*)ListPopTail(list);
				PairDestroy(pair); 
			} 
			ListDestroy(list);
		}
	} /* end for */
	
	free(_hashMap->m_stats.m_bucketNumItems);
	free(_hashMap->m_hashTable);
	free(_hashMap);
}
/*----------------------------------------------------------*/


/* insert a pair of data&key to hash table. return success/error messege */
/* doesn't support duplication of keys */
/* calls internal function HashFindItr */
ADTErr		HashInsert		(HashMap* _hashMap, HashKey _key, HashData _data)
{
	Pair *pair = NULL;
	Itr itr;
	int index, error;
	
	assert(_hashMap);
	assert(_key);
	assert(_data);
	
	/* apply hash function to get index */
	index = HASH(_hashMap, SIZE(_hashMap), _key);
	
	/* if nothing at the index - create list */
	if( !TABLE(_hashMap)[index] )
	{
		TABLE(_hashMap)[index] = ListCreate();
		if( !TABLE(_hashMap)[index] )
		{
			return ERR_ALLOCATION_FAILED;
		}
	}
	else /* find if # of items is above threshold or if identical item is already there */
	{
		if( STATS( m_bucketNumItems[index] ) >= THRESHOLD(_hashMap) )
		{
			return ERR_NOT_EFFECTIVE;
		}
		
		itr = HashFindItr(_hashMap, _key, &index);
		if( NULL != itr )
		{
			return ERR_ALREADY_EXISTS;
		}
	}
	
	/* create pair & insert into list */
	pair = PairCreate(_data, _key);
	error = ListPushTail( TABLE(_hashMap)[index], (HashData)pair );
	if(error) {
		PairDestroy(pair);
	} else {
		++STATS(m_nInsert);
		++STATS( m_bucketNumItems[index] );
	}
	return error;
}
/*----------------------------------------------------------*/


/* find & return data of _key, if not found return NULL */
/* calls internal function HashFindItr */
HashData	HashFind		(HashMap* _hashMap, HashKey _key)
{
	Itr itr = NULL;
	int index;
	Pair* pair;
	
	assert(_hashMap);
	assert(_key);
	
	itr = HashFindItr(_hashMap, _key, &index);
	if(!itr)
	{
		return NULL;
	}
	
	pair = (Pair*)ListGetData(itr);
	
	return pair->m_data;
}
/*----------------------------------------------------------*/


/* remove item from hash table, return its data, or NULL for error */
/* calls internal function HashFindItr */
HashData	HashRemove		(HashMap* _hashMap, HashKey _key)
{
	Itr itrRemove= NULL;
	int index;
	Pair *dataPair = NULL;
	HashData data = NULL;
	
	assert(_hashMap);
	assert(_key);
	
	itrRemove = HashFindItr(_hashMap, _key, &index);
	if(!itrRemove)
	{
		return NULL;
	}
	
	dataPair = (Pair*)ListRemove(itrRemove);
	data = dataPair->m_data;
	
	PairDestroy(dataPair);
	
	--STATS( m_bucketNumItems[index] );
	++STATS(m_nRemove);
	
	return data;
}
/*----------------------------------------------------------*/


/* return total # of items */
size_t		HashCountItems	(HashMap* _hashMap)
{
	size_t num = 0;
	int i;
	
	assert(_hashMap);
	
	for(i=0; i<SIZE(_hashMap); ++i)
	{
		if( TABLE(_hashMap)[i] )
		{
			num += ListCountItems( TABLE(_hashMap)[i] );
		}
	}
	
	return num;
}
/*----------------------------------------------------------*/


/* apply _doFunc to all items */
/* calls internal function ListFunc */
void		HashForEach		(HashMap* _hashMap, DoFuncHash _doFuncHash, void* _params)
{
	int i, isPrint = 0;
	List* list;
	AuxTool* tool;
	
	assert(_hashMap);
	assert(_doFuncHash);
	
	/* wrap doFunc & params to send as one parameter into ListForEach */
	tool = AuxCreate( _doFuncHash, _params);
	if(!tool) {
		return;
	}
	
	/* in case function is called for printing */
	isPrint = ( 0 == strcmp((char*)_params , "print") );
	
	if(isPrint) {
		printf("\nHashTable:\n");
	}
	
	/* for each list */
	for(i=0; i<SIZE(_hashMap); ++i)
	{
		list = TABLE(_hashMap)[i];
		if(list && !ListIsEmpty(list) )
		{
			ListForEach( ListBegin(list), ListEnd(list), ListFunc, (void*)tool);
			if(isPrint) {
				printf("\n");
			}
		}
		else if(isPrint) {
			printf("-\n");
		}
	} /* end for */
	
	if(isPrint) {
		printf("\n");
	}
	AuxDestroy(tool);
}
/*----------------------------------------------------------*/


/* set threshold for # of items in bucket - either a constant number (_mod=CONST, _param is (int)threshold ) ,
or as a percent of hashMap size (_mod=FACTOR_SIZE, 0 < (float)_param < 1  */
size_t HashSetThreshold(HashMap* _hashMap, ThresholdMod _mod, void* _param)
{
	size_t thresh, min = 3;
	
	switch(_mod)
	{
		case CONST:
			return *(int*)_param;
			
		case FACTOR_SIZE:
			thresh = *(float*)_param * SIZE(_hashMap);
			return (thresh >= min) ? thresh : min;
			
		default:
			return min;
	}
}
/*----------------------------------------------------------*/


/* print hash table - calls HashForEach */
void HashPrint(HashMap* _hashMap, DoFuncHash _printFunc)
{
#ifdef DEBUG
	char param[] = "print";
	HashForEach(_hashMap, _printFunc, param);
#endif
}
/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* general purpose search - return Itr to the pair with _key */
Itr	HashFindItr(HashMap* _hashMap, HashKey _key, int *_index)
{
	Pair *pair = NULL;
	List *list;
	Itr itr;
	int dummy;
	
	/* apply hash function to get index */
	*_index = HASH(_hashMap, SIZE(_hashMap), _key);
	
	list = TABLE(_hashMap)[*_index];
	
	/* if nothing at the index */
	if(!list)
	{
		return NULL;
	}
	
	/* search using ListFindFirst */
	pair = PairCreate(&dummy, _key);
	itr = ListFindFirst( ListBegin(list), ListEnd(list), compPairFunc, pair);
	PairDestroy(pair);
	
	/* if item not found */
	if( ListEnd(list) == itr)
	{
		return NULL;
	}
	return itr;
}
/*----------------------------------------------------------*/


/* create a Pair struct */
Pair* PairCreate(HashData _data, HashKey _key)
{
	Pair* newPair;

	newPair = (Pair*)malloc(sizeof(Pair));
	if(!newPair) {
		return NULL;
	}
	
	newPair->m_data = _data;
	newPair->m_key = _key;
	return newPair;
}
/*----------------------------------------------------------*/

/* free a Pair struct */
void PairDestroy(Pair* _pair)
{
	if(_pair) {
		free(_pair);
	}
}
/*----------------------------------------------------------*/


/* create an AuxTool struct */
AuxTool* AuxCreate(DoFuncHash _doFuncHash, void* _params)
{
	AuxTool* tool;
	
	tool = (AuxTool*)malloc(sizeof(AuxTool));
	if(!tool) {
		return NULL;
	}
	
	tool->m_doFuncHash = _doFuncHash;
	tool->m_params = _params;
	return tool;
}
/*----------------------------------------------------------*/

/* free an AuxTool struct */
void AuxDestroy(AuxTool* _tool)
{
	if(_tool) {
		free(_tool);
	}
}
/*----------------------------------------------------------*/


/* compare a pair of Pairs */
int compPairFunc(HashData* _data1, HashData* _data2)
{
	HashKey key1 = ((Pair*)_data1)->m_key, key2 = ((Pair*)_data2)->m_key;
	
	return *(int*)key1 - *(int*)key2;
}
/*----------------------------------------------------------*/


/* DoFunc for lists, for use in HashForEach. _params is AuxTool and _data is a Pair */
int ListFunc(HashData* _data, void* _params)
{
	AuxTool* tool = (AuxTool*)_params;
	Pair* pair = (Pair*)_data;
	void* realParams;
	
	realParams = tool->m_params;
	
	return tool->m_doFuncHash((HashKey)pair->m_key, (HashData)pair->m_data, realParams);
}
/*----------------------------------------------------------*/


