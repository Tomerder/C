/*******************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-21    
    Last modified date:		2013-07-22
    Description: generic (double) linked list - test functions
********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ADTDefs.h"
#include "LListGenFunc.h"
#include "LListGenTest.h"


/*----------------------------------------------------------*/
/*			definitions & foreward declarations				*/
/*----------------------------------------------------------*/


struct Node_t
{
	ListData*	m_data;
	Node*		m_next;
	Node*		m_prev;
};

struct List_t
{
	Node	m_head;
	Node	m_tail;
};



struct City
{
	char  *m_name;
	int	  m_population;
};

struct Student
{
	char  *m_name;
	float  m_grade;
};



static City*	MakeCities		(size_t _num);
static City*	CityCreate		(char *_name, int _population);
static void 	CityFreeArr		(City** _citiesArr, int _num);

static Student*	MakeStudentsArr	(size_t _num);
static Student*	StudentCreate	(char *_name, float _grade);
static void 	StudentFreeArr	(Student* _studentsArr, int _num);

static int 		StudentCompare	(ListData* _student1, ListData* _student2);
static int 		StudentFactor	(ListData* _student, void* _factor);

extern void 	ListPrint		(List *_list, PrintFunc _printFunc);
#ifdef PRINTALL
static void 	PrintInt		(void *_item);
static void 	PrintCity		(void *_city);
static void 	PrintStudent	(void *_student);
#endif

/*----------------------------------------------------------*/






/*----------------------------------------------------------*/
/*				test functions - general					*/
/*----------------------------------------------------------*/


#ifdef CHECK_NULL

/* test behavior with NULL pointers */
/* to activate, compile with -D CHECK_NULL */
/* put the function to be check first */
void LlistTestNull()
{
	List* list = NULL;
	int item1 = 5, item2, *ptr;
	
	ptr = &item2;
	
	ListPushHead(list, &item);
	ListPushTail(list, &item);	
	ListPopHead(list, &ptr);
	ListPopTail(list, &ptr);
	ListCountItems(list);
	item2 = ListIsEmpty(list);
	ListDestroy(list);
}
#endif
/*----------------------------------------------------------*/


/* create & destroy a list */
int LlistTestCreateDestroy()
{
	List* list = NULL;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	ListDestroy(list);
	
	return 0;
}
/*----------------------------------------------------------*/


/* test pushHead for integers & cities */
int LlistTestPushHead(size_t _size)
{
	List* list 	= NULL;	
	City *city	= NULL;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* with integers */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		error += ListPushHead(list, (ListData*)&arrInt[i]);
	}
	if(error) {
		ListDestroy(list);
		return error;
	}
	
#ifdef PRINTALL
	ListPrint(list, PrintInt);
#endif
	ListDestroy(list);
	
	
	/* with cities */	

	list = ListCreate();
	if(!list) {
		return 1;
	}
	city = MakeCities(_size);
		
	for(i=0; i<_size; ++i) {
		error += ListPushHead(list, (ListData*)&city[i]);
	}

#ifdef PRINTALL
	ListPrint(list, PrintCity);
#endif
	
	ListDestroy(list);
	CityFreeArr(&city, _size);
	return error;
}
/*----------------------------------------------------------*/


/* test pushTail for integers & cities */
int LlistTestPushTail(size_t _size)
{
	List* list 	= NULL;	
	City* city	= NULL;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* with integers */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		error += ListPushTail(list, (ListData*)&arrInt[i]);
	}
	if(error) {
		ListDestroy(list);
		return error;
	}
	
#ifdef PRINTALL
	ListPrint(list, PrintInt);
#endif
	ListDestroy(list);
	
	
	/* with cities */	

	list = ListCreate();
	if(!list) {
		return 1;
	}
	city = MakeCities(_size);
		
	for(i=0; i<_size; ++i) {
		error += ListPushTail(list, (ListData*)&city[i]);
	}

#ifdef PRINTALL
	ListPrint(list, PrintCity);
#endif
	
	ListDestroy(list);
	CityFreeArr(&city, _size);
	return error;
}
/*----------------------------------------------------------*/



/* test popHead for integers & cities */
int LlistTestPopHead(size_t _size)
{
	List* list 	= NULL;	
	City* city	= NULL;
	int arrInt[LIST_SIZE];
	int i, errorInt;
	City* cityPopped = NULL;
	int intPopped=-1, *ptrIntPopped;
	ListData *error;
	
	ptrIntPopped = &intPopped;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* with integers */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushHead(list, (ListData*)&arrInt[i]);
	}
	
	error = ListPopHead(list, (ListData**)&ptrIntPopped);
	
	if( NULL == error || *ptrIntPopped != arrInt[_size-1] ) {
		ListDestroy(list);
		return 1;
	}

	
#ifdef PRINTALL
	printf("\n\nlist after:\n");
	ListPrint(list, PrintInt);
	printf("\nPopped item: %d\n", *(int*)ptrIntPopped);
#endif
	ListDestroy(list);
	
	
	/* with cities */	

	list = ListCreate();
	if(!list) {
		return 1;
	}
	city = MakeCities(_size);
		
	for(i=0; i<_size; ++i) {
		ListPushHead(list, (ListData*)&city[i]);
	}

	error = ListPopHead(list, (ListData**)&cityPopped);

#ifdef PRINTALL
	printf("\n\nlist after:\n");
	ListPrint(list, PrintCity);
	printf("Popped item: ");
	PrintCity((void*)cityPopped);
	printf("\n");
#endif
	
	errorInt = ( NULL == error || ((City*)cityPopped)->m_population != city[_size-1].m_population );
	ListDestroy(list);
	CityFreeArr(&city, _size);
	
	if( errorInt ) {
		return errorInt;
	}
	
	/* with empty list */	
	
	list = ListCreate();
	error = ListPopHead(list, (ListData**)&cityPopped);
	
	ListDestroy(list);
	return (error != NULL);
}
/*----------------------------------------------------------*/



/* test popTail for integers & cities */
int LlistTestPopTail(size_t _size)
{
	List* list 	= NULL;	
	City* city	= NULL;
	int arrInt[LIST_SIZE];
	int i, errorInt;
	City* cityPopped = NULL;
	int intPopped=-1, *ptrIntPopped;
	ListData *error;
	
	ptrIntPopped = &intPopped;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* with integers */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	
	error = ListPopTail(list, (ListData**)&ptrIntPopped);
	
	if( NULL == error || *ptrIntPopped != arrInt[_size-1] ) {
		ListDestroy(list);
		return 1;
	}

	
#ifdef PRINTALL
	printf("\n\nlist after:\n");
	ListPrint(list, PrintInt);
	printf("\nPopped item: %d\n", *(int*)ptrIntPopped);
#endif
	ListDestroy(list);
	
	
	/* with cities */	

	list = ListCreate();
	if(!list) {
		return 1;
	}
	city = MakeCities(_size);
		
	for(i=0; i<_size; ++i) {
		ListPushTail(list, (ListData*)&city[i]);
	}

	error = ListPopTail(list, (ListData**)&cityPopped);

#ifdef PRINTALL
	printf("\n\nlist after:\n");
	ListPrint(list, PrintCity);
	printf("Popped item: ");
	PrintCity((void*)cityPopped);
	printf("\n");
#endif
	
	errorInt = ( NULL == error || ((City*)cityPopped)->m_population != city[_size-1].m_population );
	ListDestroy(list);
	CityFreeArr(&city, _size);
	
	if( errorInt ) {
		return errorInt;
	}
	
	/* with empty list */	
	
	list = ListCreate();
	error = ListPopHead(list, (ListData**)&cityPopped);
	
	ListDestroy(list);
	return (error != NULL);
}
/*----------------------------------------------------------*/



/* test countItems */
int ListTestCountItems(size_t _size)
{
	List* list 	= NULL;	
	int arrInt[LIST_SIZE];
	int itemPopped, *ptrPopped;
	int i, error = 0;
	size_t numItems;
	
	ptrPopped = &itemPopped;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* check while adding items */
	
	for(i=0; i<_size/2; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
		numItems = ListCountItems(list);
		error += (numItems != i+1);
	}
	for(i=_size/2; i<_size; ++i) {
		arrInt[i] = i;
		ListPushHead(list, (ListData*)&arrInt[i]);
		numItems = ListCountItems(list);
		error += (numItems != i+1);
	}
	
	/* check while popping items */
	
	for(i=_size-1; i>_size/2; --i) {
		arrInt[i] = i;
		ListPopTail(list, (ListData**)&ptrPopped);
		numItems = ListCountItems(list);
		error += (numItems != i);
	}
	for(i=_size/2; i>0; --i) {
		arrInt[i] = i;
		ListPopHead(list, (ListData**)&ptrPopped);
		numItems = ListCountItems(list);
		error += (numItems != i);
	}


	/*check on an empty list */
	
	ListPopHead(list, (ListData**)&ptrPopped);
	numItems = ListCountItems(list);
	error += (numItems != 0);
	
	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/


/* test isEmpty */
int ListTestIsEmpty(size_t _size)
{
	List* list 	= NULL;	
	int arrInt[LIST_SIZE];
	int itemPopped, *ptrPopped;
	int i, isEmpty, error = 0;
	
	ptrPopped = &itemPopped;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* check for a new list */
	isEmpty = ListIsEmpty(list);
	error += (!isEmpty);
	
	/* check while adding items */
	
	for(i=0; i<2; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
		isEmpty = ListIsEmpty(list);
		error += isEmpty;
	}
	for(i=2; i<5; ++i) {
		arrInt[i] = i;
		ListPushHead(list, (ListData*)&arrInt[i]);
		isEmpty = ListIsEmpty(list);
		error += isEmpty;
	}
	
	/* check while popping items */
	
	for(i=4; i>2; --i) {
		arrInt[i] = i;
		ListPopTail(list, (ListData**)&ptrPopped);
		isEmpty = ListIsEmpty(list);
		error += isEmpty;
	}
	for(i=2; i>0; --i) {
		arrInt[i] = i;
		ListPopHead(list, (ListData**)&ptrPopped);
		isEmpty = ListIsEmpty(list);
		error += isEmpty;
	}


	/*check on an emptied list */
	
	ListPopHead(list, (ListData**)&ptrPopped);
	isEmpty = ListIsEmpty(list);
	error += (!isEmpty);
	
	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/





/*----------------------------------------------------------*/
/*				test functions - iterative					*/
/*----------------------------------------------------------*/


/* test begin & end */
int ListItrTestBeginEnd(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr, itr2;
	int arrInt[LIST_SIZE];
	int i, isEmpty = 0, error = 0;
	ListData *ptrPopped;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	
	/* start testing */
	
	itr = ListBegin(list);
	error += (itr != list->m_head.m_next);
	
	itr = ListEnd(list);
	error += (itr != &list->m_tail);
	
	/* after popping intems form head & tail */
	
	ListPopHead(list, &ptrPopped);
	ListPopTail(list, &ptrPopped);
	
	itr = ListBegin(list);
	error += (itr != list->m_head.m_next);
	
	itr = ListEnd(list);
	error += (itr != &list->m_tail);
	
	/* empty list */
	
	while(!isEmpty)
	{
		ListPopHead(list, &ptrPopped);
		isEmpty = ListIsEmpty(list);
	}
	
	itr = ListBegin(list);
	itr2 = ListEnd(list);
	
	error += (itr != itr2);
	
	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/


/* test get data */
int ListItrTestGet(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	ListData *data;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* start testing */
	
	/* new empty list */
	itr = ListBegin(list);
	data = ListGetData(itr);
	error += (data != NULL);
	
	/* fill list & test */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	itr = ListBegin(list);	
	data = ListGetData(itr);
	error += (*(int*)data != arrInt[0]);
	
	itr = list->m_head.m_next->m_next;
	data = ListGetData(itr);
	error += (*(int*)data != arrInt[1]);	
	
	itr = ListEnd(list);
	data = ListGetData(itr);
	error += (data != NULL);

	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/


/* test next */
int ListItrTestNext(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	ListData* data;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* start testing */	
	
	/* new empty list */
	itr = ListBegin(list);
	itr = ListNext(itr);
	error += (itr != NULL);
	
	/* fill list & test */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	
	itr = ListBegin(list);
	for(i=1; i<_size; ++i) {
		itr = ListNext(itr);
		data = ListGetData(itr);
		error += ( *(int*)data != arrInt[i] );
	}	
	
	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/


/* test prev */
int ListItrTestPrev(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	ListData* data;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	/* start testing */	
	
	/* new empty list */
	itr = ListEnd(list);
	itr = ListPrev(itr);
	error += (itr != NULL);
	
	/* fill list & test */
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushHead(list, (ListData*)&arrInt[i]);
	}
	
	itr = ListEnd(list);
	for(i=0; i<_size; --i) {
		itr = ListPrev(itr);
		data = ListGetData(itr);
		error += ( *(int*)data != arrInt[i] );
	}	
	
	ListDestroy(list);
	return error;
}
/*----------------------------------------------------------*/



/* test insert */
int ListItrTestInsert(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr;
	int arrInt[LIST_SIZE];
	int arrInt2[] = {11, 12, 13, 14, 15};
	int i, error = 0;
	ListData* data;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	
	/* start testing */	
	
	itr = ListBegin(list);
	
	for(i=0; i<4; ++i)
	{
		itr = ListInsertBefore(itr, (ListData*)&arrInt2[i]);
		data = ListGetData(itr);
		error += (*(int*)data != arrInt2[i]);
	
		itr = ListNext(itr);
		data = ListGetData(itr);
		error += (*(int*)data != arrInt[i]);
		itr = ListNext(itr);
	}
	
	ListDestroy(list);
	
	/* on empty list */
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	itr = ListBegin(list);
	itr = ListInsertBefore(itr, (ListData*)&arrInt2[5]);
	data = ListGetData(itr);
	error += (*(int*)data != arrInt2[5] || itr != ListBegin(list) );
	
	ListDestroy(list);	
	return error;
}
/*----------------------------------------------------------*/


/* test remove */
int ListItrTestRemove(size_t _size)
{
	List* list 	= NULL;	
	ListItr itr;
	int arrInt[LIST_SIZE];
	int i, error = 0;
	ListData* data;
		
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	for(i=0; i<_size; ++i) {
		arrInt[i] = i;
		ListPushTail(list, (ListData*)&arrInt[i]);
	}
	
	/* start testing */	
	
	itr = ListPrev(ListEnd(list));
	data = ListRemove(itr);
	error += ( *(int*)data != arrInt[_size-1] );
	
	itr = ListNext(ListBegin(list));
	data = ListRemove(itr);
	error += ( *(int*)data != arrInt[1] );
	
	ListDestroy(list);	
	
	/* on empty list */
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	itr = ListBegin(list);
	data = ListRemove(itr);
	error += ( data != NULL );
	
	ListDestroy(list);	
	return error;
}
/*----------------------------------------------------------*/



/* test ListFindFirst */
int ListTestFindFirst(size_t _size)
{
	List* list = NULL;	
	ListItr itr;
	ListData *data;
	Student* studentArr = NULL;
	Student* studentToNotFind;
	char* name = "Zz";
	int i, error = 0;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	studentArr = MakeStudentsArr(_size);
	if(!studentArr) {
		ListDestroy(list);
		return 1;
	}
	
	for(i=0; i<_size; ++i) {
		if(i%2) {
			ListPushTail(list, (ListData*)&studentArr[i]);
		} else {
			ListPushHead(list, (ListData*)&studentArr[i]);
		}
	}
	
	/* start testing */	
	
	/* search for existing data */
	
	itr = ListEnd(list);
	
	itr = ListPrev(ListPrev(itr));
	data = ListGetData(itr);
	
	itr = ListFindFirst(ListBegin(list), ListEnd(list), StudentCompare, data);
	
	error += ( ((Student*)(itr->m_data))->m_grade != ((Student*)(data))->m_grade && strcmp(((Student*)(data))->m_name, ((Student*)(itr->m_data))->m_name) );
	
	/* search for non-existing data */
	studentToNotFind = StudentCreate(name, 70.0);
	if(!studentToNotFind) {
		ListDestroy(list);
		StudentFreeArr(studentArr, _size);
		return 1;
	}
	itr = ListFindFirst(ListBegin(list), ListEnd(list), StudentCompare, (ListData*)&studentToNotFind);
	error += (itr != ListEnd(list));
	
	/* search with _from=_to */
	itr = ListNext(ListNext(ListBegin(list)));
	itr = ListFindFirst(itr, itr, StudentCompare, (ListData*)&studentToNotFind);
	error += (itr != ListNext(ListNext(ListBegin(list))));
	
	/* search with _from>_to */
	itr = ListFindFirst(ListNext(itr), ListPrev(itr), StudentCompare, (ListData*)&studentToNotFind);
	error += (itr != ListNext(ListBegin(list)));
	
	free(studentToNotFind);
	ListDestroy(list);
	StudentFreeArr(studentArr, _size);
	return error;
}
/*----------------------------------------------------------*/



/* test ListFindFirst */
int ListTestForEach(size_t _size)
{
	List* list = NULL;	
	ListItr itr, itr2, itr3;
	ListData *data1, *data2;
	Student* studentArr = NULL;
	float data1before, data2before, factor = 0.15;
	int i, error = 0;
	
	list = ListCreate();
	if(!list) {
		return 1;
	}
	
	studentArr = MakeStudentsArr(_size);
	if(!studentArr) {
		ListDestroy(list);
		return 1;
	}
	
	for(i=0; i<_size; ++i) {
		ListPushHead(list, (ListData*)&studentArr[i]);
	}
	
#ifdef PRINTALL
	printf("\n\n\tListForEach test:\n\nbefore:");
	ListPrint(list, PrintStudent);
#endif
	
	/* start testing */	
	/* data 1 is supposed to change, data2 supposed not to */
	
	itr = ListNext(ListBegin(list));
	data1 = ListGetData(itr);
	data1before = ((Student*)(data1))->m_grade;
	
	itr = ListPrev(ListPrev(ListEnd(list)));
	data2 = ListGetData(itr);
	data2before = ((Student*)(data2))->m_grade;
	
	/* apply the change */
	itr2 = ListBegin(list);
	itr3 = ListEnd(list);
	itr = ListForEach(itr2, itr3, StudentFactor, (void*)&factor);
	error += (itr != ListEnd(list));

	/* check data1 and data2 for change */
	
	itr = ListNext(ListBegin(list));
	data1 = ListGetData(itr);
	
	itr = ListPrev(ListPrev(ListEnd(list)));
	data2 = ListGetData(itr);
	
	error += ( data1before == ((Student*)(data1))->m_grade );
	error += ( data2before!= ((Student*)(data2))->m_grade );

#ifdef PRINTALL
	printf("after:\n");
	ListPrint(list, PrintStudent);
#endif
	
	ListDestroy(list);
	StudentFreeArr(studentArr, _size);
	return error;
}
/*----------------------------------------------------------*/







/*----------------------------------------------------------*/
/*					internal functions						*/
/*----------------------------------------------------------*/


/********************* print functions *********************/

/* print: integer */

void PrintInt(void *_item)
{
	printf("%d ", *(int*)_item);
}
/*---------------------------------------------------------*/

/* print: City */

void PrintCity(void *_city)
{
	City thisCity = *(City*)_city;
	printf("City name: %s\tpopulation: %d\n", thisCity.m_name, thisCity.m_population);
}
/*---------------------------------------------------------*/

/* print: Student */

void PrintStudent(void *_student)
{
	Student thisStudent = *(Student*)_student;
	printf("Student name: %s\tgrade: %2.2f\n", thisStudent.m_name, thisStudent.m_grade);
}
/*---------------------------------------------------------*/


/*************** functions for City array ****************/


/* make an array of cities to turn into a list */
City *MakeCities(size_t _num)
{
	City *cities;
	int i, j;
	char name[]={"A"};
	
	cities = malloc(sizeof(City)*_num);
	if(NULL==cities) {
		return NULL;
	}

	for(i=0; i<_num; ++i)
	{
		name[0] = 'A'+i%26;
		cities[i] = *CityCreate(name, (i+1)<<6);
		if(NULL == (void*)&cities[i]) {
			for(j=i-1; j>=0; --j) {
				free(cities[j].m_name);
				free(&cities[j]);
			}
			return NULL;
		}
	}
	return cities;
}
/*---------------------------------------------------------*/


/* create a City struct from data */
City *CityCreate(char *_name, int _population)
{
	City *city=NULL;

	city = malloc(sizeof(City));
	if(NULL == city) {
		return NULL;
	}

	city->m_name = malloc(sizeof(char) * strlen(_name));
	if(NULL == city->m_name) {
		free(city);
		return NULL;
	}

	strcpy(city->m_name, _name);
	city->m_population = _population;

	return city;
}
/*---------------------------------------------------------*/


/* free an array of cities */
void CityFreeArr (City** _citiesArr, int _num)
{
	int i;

	if(*_citiesArr) {
		for(i=_num-1; i>=0; --i) {
			free(_citiesArr[i]->m_name);
			free(_citiesArr[i]);
		}
	}
}
/*---------------------------------------------------------*/




/*************** functions for Student array ****************/


/* make an array of Students to turn into a list */
Student *MakeStudentsArr(size_t _num)
{
	Student *students;
	int i, j;
	char name[]={"A"};
	
	students = malloc(sizeof(Student)*_num);
	if(NULL==students) {
		return NULL;
	}

	for(i=0; i<_num; ++i)
	{
		name[0] = 'A'+i%26;
		students[i] = *StudentCreate(name, (i+1)*12 + 4.34*i );
		if(NULL == (void*)&students[i]) {
			for(j=i-1; j>=0; --j) {
				free(students[j].m_name);
				free(&students[j]);
			}
			return NULL;
		}
	}
	return students;
}
/*---------------------------------------------------------*/


/* create a Student struct from data */
Student *StudentCreate(char *_name, float _grade)
{
	Student *student=NULL;

	student = malloc(sizeof(Student));
	if(NULL == student) {
		return NULL;
	}

	student->m_name = malloc(sizeof(char) * strlen(_name));
	if(NULL == student->m_name) {
		free(student);
		return NULL;
	}

	strcpy(student->m_name, _name);
	student->m_grade = _grade;

	return student;
}
/*---------------------------------------------------------*/


/* free an array of students */
void StudentFreeArr (Student *_studentsArr, int _num)
{
	int i;

	if(_studentsArr) {
		for(i=_num-1; i>=0; --i) {
			free(_studentsArr[i].m_name);
			free(&_studentsArr[i]);
		}
	}
}
/*---------------------------------------------------------*/




/* compare student nodes, return 0 if equal, other number if not */
int StudentCompare(ListData* _student1, ListData* _student2)
{
	int nameDiff = strcmp( ((Student*)(_student1))->m_name , ((Student*)(_student2))->m_name );
	float gradeDiff = ((Student*)(_student1))->m_grade - ((Student*)(_student2))->m_grade;
	
	return (nameDiff || gradeDiff);
}


/* give factor of _factor (0<=_factor<=1) to students with grades from 50 and above */
/* return 0 for success, 1 for error */
int StudentFactor(ListData* _student, void* _factor)
{
	if(*(float*)_factor > 1.0 || *(float*)_factor < 0.0) {
		return 1;
	}
	
	if( ((Student*)(_student))->m_grade >= 50.0 )
	{
		((Student*)(_student))->m_grade *= ( 1 + *(float*)_factor );

		if( ((Student*)(_student))->m_grade > 100.0 ) {
			((Student*)(_student))->m_grade = 100.0;
		}
	}
	return 0;
}
/*----------------------------------------------------------*/

