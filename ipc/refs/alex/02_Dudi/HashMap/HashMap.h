/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-30
    Date last modified :	2013-08-02
    Description : Hash Map
***************************************************************************************/

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "ADTDefs.h"
#include "DLLGen.h"

typedef struct HashMap HashMap;
typedef void*	HashData;
typedef void*	HashKey;

/* hash function */
typedef int		(*HashFunc)	(size_t _size, HashKey _key);
/* modify data */
typedef int 	(*DoFuncHash)	(HashKey _key, HashData _data, void *_params);

/* mod for settting threshold, see ThresholdSet */
typedef enum ThresholdMod
{
	CONST,
	FACTOR_SIZE
} ThresholdMod;


/* create hashmap, return NULL for failure */
HashMap*	HashCreate			(size_t _size, CmpFunc _cmpFunc, HashFunc _hashFunc);

/* destroy hashmap, free memory */
void		HashDestroy			(HashMap* _hashMap);

/* insert a pair of data&key to hash table. return success/error messege */
/* doesn't support duplication of keys */
ADTErr		HashInsert			(HashMap* _hashMap, HashKey _key, HashData _data);

/* find & return data of _key, if not found return NULL */
HashData	HashFind			(HashMap* _hashMap, HashKey _key);

/* remove item from hash table, return its data, or NULL for error */
HashData	HashRemove			(HashMap* _hashMap, HashKey _key);

/* return total # of items */
size_t		HashCountItems		(HashMap* _hashMap);

/* apply _doFuncHash to all items */
void		HashForEach			(HashMap* _hashMap, DoFuncHash _doFuncHash, void* _params);

/* set threshold for # of items in bucket - either a constant number (_mod=CONST, _param is (int)threshold ) ,
or as a percent of hashMap size (_mod=FACTOR_SIZE, 0 < (float)_param < 1  */
size_t 		HashSetThreshold	(HashMap* _hashMap, ThresholdMod _mod, void* _param);

/* print function for testing - call HashForEach */
void 		HashPrint			(HashMap* _hashMap, DoFuncHash _printFunc);


#endif /* __HASHMAP_H__ */
