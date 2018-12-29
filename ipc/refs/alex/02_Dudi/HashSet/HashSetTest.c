/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-24
    Date last modified :	2013-07-24
    Description : Hash set - for integers only
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "ADTDefs.h"
#include "HashSet.h"
#include "HashSetTest.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define NUM_ITEMS	100
#define FACTOR		2
#define N_TRIES		(int)NUM_ITEMS/4	
#define EFFECTIVE	0.7

#define ITEM(X,Y)	(X)->m_array[(Y)]

struct Hash
{
	int*	m_array;
	size_t	m_nItems;
	size_t	m_size;
};
/*----------------------------------------------------------*/




/*----------------------------------------------------------*/
/*						test functions						*/
/*----------------------------------------------------------*/




/* create & destroy a hash */
int HashTestCreateDestroy()
{
	Hash* hash = NULL;
	
	hash = HashCreate(NUM_ITEMS*FACTOR);
	if(!hash) {
		return 1;
	}
	
	HashDestroy(hash);
	
	return 0;
}
/*----------------------------------------------------------*/


/* test HashInsert */
int HashTestInsert()
{
	Hash* hash = NULL;
	size_t hashSize = NUM_ITEMS*FACTOR - 1;
	int arr[NUM_ITEMS];
	int i, index, error = 0;
	
	hash = HashCreate(hashSize);
	if(!hash) {
		return -1;
	}
	
	srand(time(NULL));
	
	/* insert random items, high chance for collisions */
	for(i=0; i<NUM_ITEMS/2; ++i) {
		arr[i] = (rand()%100)*100 + rand()%100;
		error += HashInsert(hash, arr[i]);
	}
	
	/* search for some of them */
	
	index = rand()%20;
	for(i=0; i<hashSize; ++i) {
		if(hash->m_array[i] == arr[index]) {
			break;
		}
	}
	error += (hash->m_array[i] != arr[index]);

	index += rand()%15;
	for(i=0; i<hashSize; ++i) {
		if(hash->m_array[i] == arr[index]) {
			break;
		}
	}
	error += (hash->m_array[i] != arr[index]);

	index += rand()%10;
	for(i=0; i<hashSize; ++i) {
		if(hash->m_array[i] == arr[index]) {
			break;
		}
	}
	error += (hash->m_array[i] != arr[index]);

	HashDestroy(hash);
	return error;
}
/*----------------------------------------------------------*/





/* test HashIsFound */
int HashTestIsFound()
{
	Hash* hash = NULL;
	size_t hashSize = NUM_ITEMS*FACTOR - 1;
	int arr[NUM_ITEMS];
	int item1, item2, item3;
	int i, index, error = 0;
	
	hash = HashCreate(hashSize);
	if(!hash) {
		return -1;
	}
	
	srand(time(NULL));
	
	
	/* insert specific items & search for them */
	
	item1 = 5673;
	item2 = 2900;
	item3 = 512;
	
	HashInsert(hash, item1);
	HashInsert(hash, item2);
	HashInsert(hash, item3);
	
	index = HashIsFound(hash, item1);
	index = (index==hashSize) ? 0 : index;
	error += ( hash->m_array[index] != item1 );
	
	index = HashIsFound(hash, item2);
	index = (index==hashSize) ? 0 : index;
	error += ( hash->m_array[index] != item2 );
	
	index = HashIsFound(hash, item3);
	index = (index==hashSize) ? 0 : index;
	error += ( hash->m_array[index] != item3 );
	
	
	/* search for non-existent items */
	
	index = HashIsFound(hash, item1+1);
	error += (index != 0);
	
	index = HashIsFound(hash, item2+1);
	error += (index != 0);

	index = HashIsFound(hash, item3+1);
	error += (index != 0);

	
	/* insert random items, high chance for collisions, & search for some of them */
	
	for(i=0; i<NUM_ITEMS; ++i) {
		arr[i] = (rand()%100)*100 + rand()%99 + 1;
		HashInsert(hash, arr[i]);
	}
	
	i = rand()%30;
	index = HashIsFound(hash, arr[i]);
	error += ( hash->m_array[index] != arr[i] );
	
	i += rand()%15 + 2;
	index = HashIsFound(hash, arr[i]);
	error += ( hash->m_array[index] != arr[i] );
	
	i += rand()%40 + 7;
	index = HashIsFound(hash, arr[i]);
	error += ( hash->m_array[index] != arr[i] );
	
	
	HashDestroy(hash);
	return error;
}
/*----------------------------------------------------------*/



/* test HashRemove */
int HashTestRemove()
{
	Hash* hash = NULL;
	size_t hashSize = NUM_ITEMS*FACTOR - 1;
	int arr[NUM_ITEMS];
	int i, index, error = 0;
	
	hash = HashCreate(hashSize);
	if(!hash) {
		return -1;
	}
	
	srand(time(NULL));
	
	/* insert random items */
	for(i=0; i<NUM_ITEMS/2; ++i) {
		arr[i] = (rand()%100)*100 + rand()%100;
		HashInsert(hash, arr[i]);
	}
	
	/* remove items, then search for them */
	
	index = rand()%10;
	error += HashRemove(hash, arr[index]);
	error += HashIsFound(hash, arr[index]);
	
	index += rand()%15 + 5;
	error += HashRemove(hash, arr[index]);
	error += HashIsFound(hash, arr[index]);
	
	index += rand()%20 + 3;
	error += HashRemove(hash, arr[index]);
	error += HashIsFound(hash, arr[index]);
	
	/* print */
	printf("\n\tthis is the hash:\n\n");
	HashPrint(hash);
	
	/* re-insert last item */
	error += HashInsert(hash, arr[index]);
	error += (!HashIsFound(hash, arr[index]));
	
	HashDestroy(hash);
	return error;
}
/*----------------------------------------------------------*/



/* test IsEffective & NumItems */
int HashTestNumEffect()
{
	Hash* hash = NULL;
	size_t hashSize = NUM_ITEMS*FACTOR - 1;
	int arr[NUM_ITEMS];
	int i, index, error = 0;
	
	hash = HashCreate(hashSize);
	if(!hash) {
		return -1;
	}
	
	srand(time(NULL));
	
	/* insert random items & test NumItems */
	for(i=0; i<NUM_ITEMS/2; ++i) {
		arr[i] = (rand()%100)*100 + rand()%100;
		HashInsert(hash, arr[i]);
	}
	
	error += ( HashNumItems(hash) != i );
	
	/* test isEffective */
	
	error += ( !HashIsEffective(hash) );
	
	for( ; i<hashSize; ++i) {
		HashInsert(hash, (rand()%100)*100 + rand()%100);
		if( !HashIsEffective(hash) ) {
			break;
		}
	}
	
	index = (int) (hash->m_size * EFFECTIVE);
	
	error += ( i-index > 2 || i-index < -2 );
	
	/* print */
	printf("\n\tthis is an ineffective hash:\n\n");
	HashPrint(hash);

	HashDestroy(hash);
	return error;
}
/*----------------------------------------------------------*/



