/**************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-29    
    Last modified date:		2013-07-29
    Description: generic heap - test functions
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "HeapGeneric.h"
#include "HeapGenericTest.h"

/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/

#define VEC_SIZE 10

#define LEFT(I)			((I)+(I))
#define RIGHT(I)		((I)+(I)+1)
#define PARENT(I)		((I)/2)


typedef struct City
{
	char  *m_name;
	int	  m_population;
} City;

/* handling of intermediate errors, printing error messeges in debug mode */
static void 	ErrorCheckPrint	(int *_errTemp, int *_errTot, char *_text, int _number);
static void 	PrintDebug		(char *_text, int _number); 
 
/* comparison & print functions - integers and cities */
static int		CompInt			(HeapData _data1, HeapData _data2);
static void 	PrintFuncInt	(HeapData _data, int dummy);
static int 		CompCityPop		(HeapData _data1, HeapData _data2);
static int 		CompCityName	(HeapData _data1, HeapData _data2);
static void 	PrintFuncCity	(void *_city, int dummy);
static void 	AddInt			(HeapData _data, void* _params);	/* doFunc for HeapForEach */


/* allocate & free city structs */
static City* 	CityCreate		(char *_name, int _population);
static void 	CityFreeArr 	(City **_citiesArr, int _num);

/*----------------------------------------------------------*/


/*----------------------------------------------------------*/
/*						TEST FUNCTIONS						*/
/*															*/
/* 		return 0 for success, positive # for failure,		*/
/* 		-1 for error unrelated to tested function(s)		*/
/*----------------------------------------------------------*/


/* build & destroy a heap: integers & cities */
int HeapTestBuildDestroy()
{
	Heap* heap = NULL;
	Vector *vector = NULL;
	size_t size = VEC_SIZE;
	int arrInt [VEC_SIZE];
	City *arrCity [VEC_SIZE]; 
	char name[]={"A"};
	int i, errorTemp = 0, errorTot = 0, errGet;
	HeapData data1, data2, data3;
	
	srand(time(NULL));
		
	/*** integers ***/
	
	vector = VectorCreate(size, 2);
	if(!vector) {
		return -1;
	}
	for(i=0; i<size; ++i)
	{
		arrInt[i] = rand()%100;
		VectorAdd( vector, &arrInt[i] );
	}
	
	PrintDebug("\noriginal Vector:",-1);
	VectorPrint(vector, PrintFuncInt);
	
	heap = HeapBuild(vector, CompInt);
	if(!heap) {
		return 1;
	}
	PrintDebug("\nheapified Vector:", -1);
	VectorPrint(vector, PrintFuncInt);
	PrintDebug("\n", -1);
	
	for(i=1; i<=size/2; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, LEFT(i), &data2);
		errGet = VectorGet(vector, RIGHT(i), &data3);
		
		errorTemp += ( CompInt(data1, data2)<0 && ( !errGet && CompInt(data1, data3)<0 ) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapBuild, line ", __LINE__-16);
	
	HeapDestroy(heap);
	PrintDebug("Heap Destroyed.\n", -1);
	VectorDestroy(vector);
	
	/*** cities ***/

	vector = VectorCreate(size, 2);
	if(!vector) {
		return -1;
	}
	
	for(i=0; i<size; ++i)
	{
		name[0] = 'A'+i%26;
		arrCity[i] = CityCreate(&name[0], rand()%5000+500 );
		if(!arrCity[i])
		{
			CityFreeArr(arrCity, i-1);
			VectorDestroy(vector);
			return -1;
		}
		VectorAdd(vector, (void*)arrCity[i]);
	}
	
	PrintDebug("\noriginal Vector:",-1);
	VectorPrint(vector, PrintFuncCity);
	
	heap = HeapBuild(vector, CompCityPop);
	if(!heap) {
		return 1;
	}
	PrintDebug("\nheapified Vecto(by population):", -1);
	VectorPrint(vector, PrintFuncCity);
	
	for(i=1; i<=size/2; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, LEFT(i), &data2);
		errGet = VectorGet(vector, RIGHT(i), &data3);
		
		errorTemp += ( CompCityPop(data1, data2)<0 && ( !errGet && CompCityPop(data1, data3)<0 ) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapBuild, line ", __LINE__-15);
	
	PrintDebug(" ", -1);
	HeapDestroy(heap);
	PrintDebug("Heap Destroyed.\n", -1);
	VectorDestroy(vector);
	CityFreeArr(arrCity, size);

	return errorTot;
}
/*----------------------------------------------------------*/



/* extract max and heap sort */
int HeapTestMaxSort()
{
	Heap* heap = NULL;
	Vector *vector = NULL;
	size_t size = VEC_SIZE;
	City *arrCity [VEC_SIZE]; 
	char name[]={"A"};
	int i, errorTemp = 0, errorTot = 0;
	HeapData data1, data2;
	
	PrintDebug("\n", -1);
	srand(time(NULL));
	
	vector = VectorCreate(size, 2);
	if(!vector) {
		return -1;
	}
	for(i=0; i<size; ++i)
	{
		name[0] = 'A'+i%26;
		arrCity[i] = CityCreate(&name[0], rand()%5000+500 );
		if(!arrCity[i])
		{
			CityFreeArr(arrCity, i-1);
			VectorDestroy(vector);
			return -1;
		}
		VectorAdd(vector, (void*)arrCity[i]);
	}
	
	PrintDebug("\noriginal Vector:",-1);
	VectorPrint(vector, PrintFuncCity);
	
	HeapSort(vector, CompCityPop);
	PrintDebug("\nsorted by population:", -1);
	VectorPrint(vector, PrintFuncCity);
	
	for(i=1; i<size; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, i+1, &data2);
		errorTemp += ( CompCityPop(data1, data2)>0 );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapSort (by population), line ", __LINE__-10);
	
	HeapSort(vector, CompCityName);
	PrintDebug("\nnow re-sorted by name:", -1);
	VectorPrint(vector, PrintFuncCity);

	for(i=1; i<size; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, i+1, &data2);
		errorTemp += ( CompCityName(data1, data2)>0 );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapSort (by name), line ", __LINE__-10);
	
	PrintDebug(" ", -1);
	HeapDestroy(heap);
	VectorDestroy(vector);
	CityFreeArr(arrCity, size);
	return errorTot;
}
/*----------------------------------------------------------*/


/* testing insert, extract, numItems */
int HeapTestInsertExtractNum()
{
	Heap* heap = NULL;
	Vector *vector = NULL;
	size_t size = VEC_SIZE;
	int arrInt [VEC_SIZE+2];
	int i, errorTemp = 0, errorTot = 0, errGet = 0, num;
	HeapData data1, data2, data3;
	
	srand(time(NULL));
	
	vector = VectorCreate(size, 2);
	if(!vector) {
		return -1;
	}
	for(i=0; i<size; ++i)
	{
		arrInt[i] = rand()%100;
		VectorAdd( vector, &arrInt[i] );
	}
	
	PrintDebug("\noriginal Vector:",-1);
	VectorPrint(vector, PrintFuncInt);
	
	heap = HeapBuild(vector, CompInt);
	if(!heap) {
		return -1;
	}
	
	num = HeapNumItems(heap);
	errorTemp += (num != size);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapNumItems, line ", __LINE__-2);
	
	/* ExtractMax */
	HeapExtractMax(heap);
	HeapExtractMax(heap);
	
	num = HeapNumItems(heap);
	errorTemp += (num != size-2);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapNumItems after extraction, line ", __LINE__-2);
	
	PrintDebug("\nheap after two extractions:", -1);
	VectorPrint(vector, PrintFuncInt);
	
	for(i=1; i<=size/2; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, LEFT(i), &data2);
		errGet = VectorGet(vector, RIGHT(i), &data3);
		
		errorTemp += ( CompInt(data1, data2)<0 && ( !errGet && CompInt(data1, data3)<0 ) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapExtractMax, line ", __LINE__-18);
	errGet = 0;
	
	/* Insert */
	arrInt[size] = 120;
	arrInt[size+1] = 12;
	
	errGet += HeapInsert(heap, (HeapData)&arrInt[size]);
	errGet += HeapInsert(heap, (HeapData)&arrInt[size+1]);
	errorTemp += errGet;
	
	num = HeapNumItems(heap);
	errorTemp += (num != size);
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapNumItems after insertion, line ", __LINE__-2);
	
	for(i=1; i<=size/2; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, LEFT(i), &data2);
		errGet = VectorGet(vector, RIGHT(i), &data3);
		
		errorTemp += ( CompInt(data1, data2)<0 && ( !errGet && CompInt(data1, data3)<0 ) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapInsert, line ", __LINE__-15);
	
	PrintDebug("\nheap after two insertions:", -1);
	VectorPrint(vector, PrintFuncInt);
	PrintDebug("\n", -1);
	
	HeapDestroy(heap);
	VectorDestroy(vector);
	return errorTot;
}
/*----------------------------------------------------------*/


/* testing ForEach */
int HeapTestForEach()
{
	Heap* heap = NULL;
	Vector *vector = NULL;
	size_t size = VEC_SIZE;
	int arrInt [VEC_SIZE];
	int i, errorTemp = 0, errorTot = 0, errGet = 0;
	int add = 100;
	HeapData data1, data2, data3;
	
	srand(time(NULL));
	
	vector = VectorCreate(size, 2);
	if(!vector) {
		return -1;
	}
	for(i=0; i<size; ++i)
	{
		arrInt[i] = rand()%100;
		VectorAdd( vector, &arrInt[i] );
	}
	
	PrintDebug("\noriginal Vector:",-1);
	VectorPrint(vector, PrintFuncInt);
	
	heap = HeapBuild(vector, CompInt);
	if(!heap) {
		return -1;
	}
	PrintDebug("\nheapified:", -1);
	VectorPrint(vector, PrintFuncInt);
	
	/* HeapForEach */
	HeapForEach(heap, AddInt, (void*)&add);

	for(i=1; i<=size/2; ++i)
	{
		VectorGet(vector, i, &data1);
		VectorGet(vector, LEFT(i), &data2);
		errGet = VectorGet(vector, RIGHT(i), &data3);
		
		errorTemp += ( CompInt(data1, data2)<0 && ( !errGet && CompInt(data1, data3)<0 ) );
	}
	ErrorCheckPrint(&errorTemp, &errorTot, "Error: HeapInsert, line ", __LINE__-15);

	PrintDebug("\nafter HeapForEach adding 100 to all items <50:",-1);
	VectorPrint(vector, PrintFuncInt);
	PrintDebug("\n", -1);
	
	HeapDestroy(heap);
	VectorDestroy(vector);
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


/*** for data types ***/


/* compare integers */	
static int CompInt(HeapData _data1, HeapData _data2)
{
	return *(int*)_data1 - *(int*)_data2;
}
/*----------------------------------------------------------*/


/* print integer data */
static void PrintFuncInt(HeapData _data, int dummy)
{
#ifdef DEBUG
	printf("%d ", *(int*)_data);
#endif
}
/*----------------------------------------------------------*/


/* compare cities by population */	
static int CompCityPop(HeapData _data1, HeapData _data2)
{
	int pop1, pop2;
	
	pop1 = ((City*)_data1)->m_population;
	pop2 = ((City*)_data2)->m_population;
	
	return pop1 - pop2;
}
/*---------------------------------------------------------*/

/* compare cities by name */	
static int CompCityName(HeapData _data1, HeapData _data2)
{
	char *name1, *name2;
	
	name1 = ((City*)_data1)->m_name;
	name2 = ((City*)_data2)->m_name;
	
	return name2-name1;
}
/*---------------------------------------------------------*/

/* print: City */
static void PrintFuncCity(void *_city, int dummy)
{
#ifdef DEBUG
	City thisCity = *(City*)_city;
	printf("City name: %s\tpopulation: %d\n", thisCity.m_name, thisCity.m_population);
#endif
}
/*---------------------------------------------------------*/

/* doFunc for HeapForEach: add *(int*)_params to all items < 50 */
static void AddInt(HeapData _data, void* _params)
{
	int dataInt = *(int*)_data;
	int add = *(int*)_params;
	
	*(int*)_data = (dataInt<50) ? dataInt+add : dataInt;
}
/*---------------------------------------------------------*/


/* create a City struct from data */
static City* CityCreate(char *_name, int _population)
{
	City* city = NULL; 
	
	city = malloc(sizeof(City));
	if(!city) {
		return NULL;
	}
	city->m_name = malloc(sizeof(char) * strlen(_name));
	if(!city->m_name) {
		return NULL;
	}

	strcpy(city->m_name, _name);
	city->m_population = _population;

	return city;
}
/*---------------------------------------------------------*/


/* free an array of cities */
static void CityFreeArr (City **_citiesArr, int _num)
{
	int i;

	if(_citiesArr) {
		for(i=0; i<_num; ++i) {
			free(_citiesArr[i]->m_name);
			free(_citiesArr[i]);
		}
/*		free(_citiesArr); */
	}
}
/*---------------------------------------------------------*/

