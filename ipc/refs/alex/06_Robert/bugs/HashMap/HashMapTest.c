/**************************************************************************************
    Author :	Stav Ofer
    Creation date :      	2013-07-31
    Date last modified :	2013-08-02
    Description : Hash Map - test functions
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "DLLGen.h"
#include "HashMap.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define HASHSIZE	11

/* test functions */
int HashTestCreateDestroy();
int HashTestInsertRemoveFind1();
int HashTestInsertRemoveFind2();
int HashTestCountForEach();

/* internal functions */
static int		HashModInt		(size_t _size, HashKey _key);	/* hash func */
static int 		CompFuncInt		(HashData* _data1, HashData* _data2); /* comp func */
static int		HashDoAdd		(HashKey _key, HashData _data, void *_params); /* do func */
static int 		printFuncInt	(HashKey _key, HashData _data, void *_params); /* print func */

/* handling of intermediate errors, printing error messeges in debug mode */
static void 	ErrorCheckPrint	(int *_errTemp, int *_errTot, char *_text, int _number);
static void 	PrintDebug		(char *_text, int _number); 

/*----------------------------------------------------------*/


/*----------------------------------------------------------*/
/*						TEST FUNCTIONS						*/
/*															*/
/* 		return 0 for success, positive # for failure,		*/
/* 		-1 for error unrelated to tested function(s)		*/
/*----------------------------------------------------------*/


/* create & destroy */
int HashTestCreateDestroy()
{
	HashMap *myHash;
	size_t size = HASHSIZE;
	
	myHash = HashCreate(size, CompFuncInt, HashModInt);
	if(!myHash) {
		PrintDebug("\nError: HashCreate failed, line ",__LINE__-2);
		return 1;
	}
	
	HashDestroy(myHash);
	return 0;
}
/*----------------------------------------------------------*/


/* insert, remove, find - no collisions */
int HashTestInsertRemoveFind1()
{
	HashMap *myHash;
	size_t size = HASHSIZE, numItems = HASHSIZE/2;
	int arrInt[HASHSIZE/2], arrKey[HASHSIZE/2];
	int i, errorTemp = 0, errorTot = 0;
	int tempInt, tempKey;
	HashData dataTemp = NULL;

	myHash = HashCreate(size, CompFuncInt, HashModInt);
	if(!myHash) {
		PrintDebug("\nError: HashCreate failed, line ",__LINE__-2);
		return -1;
	}
	
	/* insert, start from empty */
	srand(time(NULL));
	for(i=0; i<numItems; ++i)
	{
		arrInt[i] = i*100 + rand()%100;
		arrKey[i] = 2*i+1;
		errorTemp += HashInsert(myHash, &arrKey[i], &arrInt[i]);
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert, line ", __LINE__-2);
	
	for(i=0; i<numItems; ++i)
	{
		dataTemp = HashFind(myHash, &arrKey[i]);
		errorTemp += ( *(int*)dataTemp != arrInt[i] );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert, line ", __LINE__-9);
	
	/* insert with existing key */
	tempInt = arrInt[2] + 1;
	tempKey = arrKey[2];
	errorTemp = HashInsert(myHash, &tempKey, &tempInt);
	errorTemp = (errorTemp != ERR_ALREADY_EXISTS);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert with existing key, line ", __LINE__-2);
	
	/* remove until empty */
	
	dataTemp = HashRemove(myHash, &arrKey[0]);
	errorTemp += ( *(int*)dataTemp != arrInt[0] );
	dataTemp = HashFind(myHash, &arrKey[0]);
	errorTemp += ( dataTemp != NULL );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashRemove, line ", __LINE__-4);
	
	HashPrint(myHash, printFuncInt);
	
	for(i=1; i<numItems; ++i)
	{
		dataTemp = HashRemove(myHash, &arrKey[i]);
		errorTemp += ( *(int*)dataTemp != arrInt[i] );
		dataTemp = HashFind(myHash, &arrKey[i]);
		errorTemp += ( dataTemp != NULL );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashRemove, line ", __LINE__-5);
	
	/* find, remove nonexistent item */
	dataTemp = HashFind(myHash, &arrKey[3]);
	errorTemp += (dataTemp != NULL );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashFind of non-existent item, line ", __LINE__-2);
	dataTemp = HashRemove(myHash, &arrKey[3]);
	errorTemp += (dataTemp != NULL );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashRemove of non-existent item, line ", __LINE__-2);
	
	/* re-insert */
	
	errorTemp += HashInsert(myHash, &arrKey[3], &arrInt[3]);
	dataTemp = HashFind(myHash, &arrKey[3]);
	errorTemp += ( *(int*)dataTemp != arrInt[3] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert, line ", __LINE__-3);
	
	errorTemp += HashInsert(myHash, &arrKey[1], &arrInt[1]);
	dataTemp = HashFind(myHash, &arrKey[1]);
	errorTemp += ( *(int*)dataTemp != arrInt[1] );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert, line ", __LINE__-3);
	
	HashDestroy(myHash);
	return errorTot;
}
/*----------------------------------------------------------*/


/* insert, remove, find - with collisions */
int HashTestInsertRemoveFind2()
{
	HashMap *myHash;
	size_t size = HASHSIZE, numItems = HASHSIZE*1.5;
	int arrInt[HASHSIZE*2 - HASHSIZE/2], arrKey[HASHSIZE*2 - HASHSIZE/2];
	int arrIntLarge[100], arrKeyLarge[100];
	int i, errorTemp = 0, errorTot = 0;
	HashData dataTemp = NULL;

	myHash = HashCreate(size, CompFuncInt, HashModInt);
	if(!myHash) {
		PrintDebug("\nError: HashCreate failed, line ",__LINE__-2);
		return -1;
	}
	
	/* insert */
	srand(time(NULL));
	for(i=0; i<numItems; ++i)
	{
		arrInt[i] = i*100 + rand()%100;
		arrKey[i] = 2*i+1;
		errorTemp += HashInsert(myHash, &arrKey[i], &arrInt[i]);
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashInsert, line ", __LINE__-2);
	
	/* remove */
	for(i=1; i<numItems; i+=3)
	{
		dataTemp = HashRemove(myHash, &arrKey[i]);
		errorTemp += ( *(int*)dataTemp != arrInt[i] );
		dataTemp = HashFind(myHash, &arrKey[i]);
		errorTemp += ( dataTemp != NULL );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashRemove, line ", __LINE__-5);
	
	HashPrint(myHash, printFuncInt);
	
	HashDestroy(myHash);
	
	/* insert until not effective */
	
	myHash = HashCreate(10, CompFuncInt, HashModInt);
	if(!myHash) {
		PrintDebug("\nError: HashCreate failed, line ",__LINE__-2);
		return -1;
	}
	
	for(i=0; errorTemp != ERR_NOT_EFFECTIVE; ++i)
	{
		arrIntLarge[i] = i*100 + rand()%100;
		arrKeyLarge[i] = 2*i+1;
		errorTemp = HashInsert(myHash, &arrKeyLarge[i], &arrIntLarge[i]);
	}
	
	HashPrint(myHash, printFuncInt);
	
	HashDestroy(myHash);
	return errorTot;
}
/*----------------------------------------------------------*/


/* test countItems, ForEach */
int HashTestCountForEach()
{
	HashMap *myHash;
	size_t size = HASHSIZE, numItems = HASHSIZE*1.5;
	int arrInt[HASHSIZE*2 - HASHSIZE/2], arrKey[HASHSIZE*2 - HASHSIZE/2];
	int arrIntTemp[HASHSIZE*2 - HASHSIZE/2];
	int i, j, errorTemp = 0, errorTot = 0;
	int add = 10000;
	int num;

	myHash = HashCreate(size, CompFuncInt, HashModInt);
	if(!myHash) {
		PrintDebug("\nError: HashCreate failed, line ",__LINE__-2);
		return -1;
	}
	
	
	/* count - empty hash */
	num = HashCountItems(myHash);
	errorTemp = ( num != 0 );
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashCountItems for empty table, line ", __LINE__-2);
	
	/* filling hash */
	srand(time(NULL));
	for(i=0; i<numItems; ++i)
	{
		arrInt[i] = arrIntTemp[i] = i*100 + rand()%100;
		arrKey[i] = 2*i+1;
		HashInsert(myHash, &arrKey[i], &arrInt[i]);
		num = HashCountItems(myHash);
		errorTemp += ( num != i+1 );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashCountItems, line ", __LINE__-3);

	HashPrint(myHash, printFuncInt);
	PrintDebug("# of items is: ", num);
	
	/* ForEach */
	HashForEach(myHash, HashDoAdd, &add);
	for(i=0; i<numItems; ++i)
	{
		errorTemp += ( arrInt[i] != arrIntTemp[i] + add );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashForEach, line ", __LINE__-5);
	
	PrintDebug("\nAfter ForEach:", -1);
	HashPrint(myHash, printFuncInt);
	
	/* count - removing from hash */
	for(i=1, j=1; i<numItems; i+=3, ++j)
	{
		HashRemove(myHash, &arrKey[i]);
		num = HashCountItems(myHash);
		errorTemp += ( num != numItems - j );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HashCountItems, line ", __LINE__-4);
	
	HashDestroy(myHash);
	return errorTot;
}
/*----------------------------------------------------------*/



/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/* for printing in debug mode */
static void PrintDebug(char *_text, int _number)
{
#ifdef DEBUG
	printf("%s", _text);
	if( _number != -1 ) {
		printf("%d", _number);
	}
	printf("\n");
#endif
}
/*----------------------------------------------------------*/


/* error handling for test functions */
static void ErrorCheckPrint(int *_errTemp, int *_errTot, char *_text, int _number)
{
	if(*_errTemp)
	{
		PrintDebug(_text, _number);
		*_errTot += *_errTemp;
		*_errTemp = 0;
	}
}
/*----------------------------------------------------------*/


/* hash function - rule for assigning. returns index */
int		HashModInt	(size_t _size, HashKey _key)
{
	int index;
	
	index = *(int*)_key % _size;
	return index;
}
/*----------------------------------------------------------*/


/* compare 2 data items - integer, return a poisive # for 1 > 2, 0 for equal, negative for 1 < 2 */
int CompFuncInt(HashData* _data1, HashData* _data2)
{
	return **(int**)_data1 - **(int**)_data2;
}
/*----------------------------------------------------------*/


/* print integer data - for HashPrint */
int printFuncInt(HashKey _key, HashData _data, void *_params)
{
	printf("%d ", *(int*)_data);
	return 1;
}
/*----------------------------------------------------------*/


/* DoFunc for HashForEach: for integers: data = data + params */
int		HashDoAdd(HashKey _key, HashData _data, void *_params)
{	
	*(int*)_data += *(int*)_params;
	return 1;
}
/*----------------------------------------------------------*/


