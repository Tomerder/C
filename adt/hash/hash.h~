/**************************************************************************************
    Author : Tomer Dery
    Creation date :      08.01.14
    Date last modified : 09.01.14
     Description : HashMap
***************************************************************************************/
#ifndef HASH_H
#define HASH_H

#define _DEBUG

#include "GenData.h"

typedef struct HashMap HashMap;
typedef const void *	Key;
typedef int (*HashFunc)  (Key, size_t);
typedef int (*HashDoFunc)(Key, Data, Param);
typedef int (*CompFunc)	 (Key, Key);


/*----------------------------------------------------------------------*/
/* Creates an hash table of a given size.                               */
/*  - Given size must be biger then maximum data nodes number.          */
/*  - User must provide: hash function    - _hashFunc                   */
/*                       compare function - _isEqualFunc                */
/*  Implementation issues:                                              */
/*   - Size should be the closest prime number followed to choosen size.*/
/*   - Collisions shall be solved using LL.                             */
/*   - All lists shall be allocated while creation.                     */
/*----------------------------------------------------------------------*/
HashMap* 	HashCreate(size_t, HashFunc, CompFunc);
void		HashDestroy(HashMap*);

/*----------------------------------------------------------------------*/
/* HashInsert: Duplicated Key insertion is not allowed.                 */
/*             returns: data on success.                                */
/*                      0 Otherwise (also in case of duplication).      */
/*----------------------------------------------------------------------*/
Data		HashInsert	(HashMap*, Key, Data);

/*----------------------------------------------------------------------*/
/* HashFind: Returns the value for the found key.                       */
/*                   NULL - if key was not found                        */
/*----------------------------------------------------------------------*/
Data		HashFind	(HashMap*, Key);

/*----------------------------------------------------------------------*/
/* HashRemove: Returns the value of the removed key.                    */
/*                   NULL - if key was not found                        */
/*----------------------------------------------------------------------*/
Data		HashRemove	(HashMap*, Key);

/*----------------------------------------------------------------------*/
/* HashCountAcupiedBuckets:                                             */
/*  Counts the all items - O(n)  										*/
/*----------------------------------------------------------------------*/
size_t		HashCountItems(HashMap*);

/*----------------------------------------------------------------------*/
/* HashCountBuckets: 		                                            */
/*  Counts all non empty buckets of hash table - O(size of table)       */
/*----------------------------------------------------------------------*/
size_t		HashCountBuckets(HashMap*);


/*----------------------------------------------------------------------*/
/* HashForEachunt:	 		                                            */
/*  Perform function for each element in  of hash table				    */
/*	returns num of buckets that has successfully passed                 */
/*----------------------------------------------------------------------*/
int			HashForEach(HashMap* _map, HashDoFunc, Param);

/*----------------------------------------------------------------------*/

#endif





