/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Hash set - for integers only
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ADTDefs.h"
#include "HashSet.h"



/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define NUM_ITEMS	100
#define N_TRIES		(int)NUM_ITEMS	
#define EFFECTIVE	0.7

#define ITEM(X,Y)	(X)->m_array[(Y)]

struct Hash
{
	int*	m_array;
	size_t	m_nItems;
	size_t	m_size;
};


static int		HashFunc	(Hash* _hash, int _key);
static int		ReHash		(Hash* _hash, int _key);


/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*						API functions						*/
/*----------------------------------------------------------*/


/* create hash, return NULL for failure */
Hash*	HashCreate		(size_t _size)
{
	Hash* newHash = NULL;
	int*  hashArr = NULL;
	
	/* create items array & hash struct */
	
	hashArr = (int*)calloc( _size, sizeof(int) );
	if(!hashArr) {
		return NULL;
	}
	
	newHash = (Hash*)malloc(sizeof(Hash));
	if(!newHash) {
		free(hashArr);
		return NULL;
	}
	
	/* initialize hash */
	newHash->m_array = hashArr;
	newHash->m_nItems = 0;
	newHash->m_size = _size;
	
	return newHash;
}
/*----------------------------------------------------------*/


/* destroy hash */
void	HashDestroy		(Hash* _hash)
{
	assert(_hash);
	if(_hash->m_array) {
		free(_hash->m_array);
	}
	free(_hash);
}
/*----------------------------------------------------------*/



/* return the index for found, 0 for not found */
/* if the index is 0, returns array size */
int		HashIsFound		(Hash* _hash, int _key)
{
	int index, isFound = 0;
	
	assert(_hash);
	
	/* find index */
	index = HashFunc(_hash, _key);
	
	do {
	
		/* return 1 if found, 0 if index is empty */
		if( ITEM(_hash, index) == _key ) {
			isFound = 1;
			break;
		}
		if( !ITEM(_hash, index) ) {
			break;
		}

		/* if not either, try again by calling rehash */
		index = ReHash(_hash, index);
	
	} while( !isFound );
	
	if(isFound && index==0) {
		index = _hash->m_size;
	}
	
	return isFound * index;
}
/*----------------------------------------------------------*/



/* insert in hash, return success/error messege */
/* try at most N_TRIES times */
ADTErr	HashInsert		(Hash* _hash, int _key)
{
	int index, count=0, nTries = N_TRIES;

	assert(_hash);
	
	/* find index */
	index = HashFunc(_hash, _key);
	
	do {
		/* insert & return OK if index is empty (contains 0 or -1) */
		if( 0 == ITEM(_hash, index) || -1 == ITEM(_hash, index) ) {
			ITEM(_hash, index) = _key;
			++_hash->m_nItems;
			return ERR_OK;
		}

		/* else, try again by calling rehash */
		index = ReHash(_hash, index);
		++count;
	
	} while( count < nTries );
	
	return ERR_NOT_EFFECTIVE;
}
/*----------------------------------------------------------*/



/* remove from hash */
ADTErr	HashRemove		(Hash* _hash, int _key)
{
	int index, isThere=0;
	
	assert(_hash);
	
	if( 0 == _hash->m_nItems ) {
		return ERR_EMPTY;
	}
	
	/* find index */
	index = HashFunc(_hash, _key);
	
	do {
		/* mark if item is at index */
		if( _key == ITEM(_hash, index) ) {
			isThere = 1;
			break;
		}
		/* return error if index was always empty */
		if(!ITEM(_hash, index)) {
			return ERR_NOT_THERE;
		}
		
		/* else, try again by calling rehash */
		index = ReHash(_hash, index);
	
	} while(!isThere);
	
	/* remove & put -1 in index */
	if(isThere) {
		ITEM(_hash, index) = -1;
		--_hash->m_nItems;
	}
	
	return (isThere) ? ERR_OK : ERR_NOT_EFFECTIVE;
}
/*----------------------------------------------------------*/



/* return # of items */
int		HashNumItems	(Hash* _hash)
{
	assert(_hash);
	return _hash->m_nItems;
}
/*----------------------------------------------------------*/



/* is hash too full to be effective? 1 for yes, 0 for no */
int		HashIsEffective	(Hash* _hash)
{
	float effective;
	
	assert(_hash);
	
	effective = (float)_hash->m_nItems / _hash->m_size;
	
	return (effective < EFFECTIVE);
}
/*----------------------------------------------------------*/


/* print - for testing */
void	HashPrint		(Hash* _hash)
{
	int i;
	
	assert(_hash);
	
	printf("Number of items in hash: %d\nHash size: %d\n\n", _hash->m_nItems, _hash->m_size);
	
	for(i=0; i<_hash->m_size; ++i) {
		printf("%d ", ITEM(_hash, i) );
	}
	printf("\n\n");
}
/*----------------------------------------------------------*/





/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* hash function - rule for assigning. returns index */
int		HashFunc	(Hash* _hash, int _key)
{
	int index;
	
	index = _key % _hash->m_size;
	return index;
}
/*----------------------------------------------------------*/


/* rehash function - find new place if hash failed */
/* return index, or -1 if didn't find */
int		ReHash		(Hash* _hash, int _key)
{
	int index;
	
	index = ++_key % _hash->m_size;
	return index;
}
/*----------------------------------------------------------*/




