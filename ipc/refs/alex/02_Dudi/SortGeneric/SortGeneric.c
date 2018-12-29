/**************************************************************************************
    Author: Stav Ofer
    Creation date:  		2013-07-29    
    Last modified date:		2013-07-29
    Description: generic sort function - bubble
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE 10

#define SWAP(X,Y,TYPE)		{TYPE temp = (X);\
							(X) = (Y);\
							(Y) = temp; }
							
typedef	int		(*CompFunc)		(void *_data, int _index, size_t _itemSize);

static int  	CompFuncInt		(void *_data, int _index, size_t _itemSize);
static void		BubbleSort		(void *_data, size_t _nItems, size_t _itemSize, CompFunc _cmpFunc);
void 			GenericSwapPtrs	(void **ptr1, void **ptr2);


/*** MAIN ***/
int main()
{
	size_t size=SIZE;
	int array[SIZE];
	int i; 
/*	char *str = "gwuzbfqahm";*/

	
	srand(time(NULL));
	/* fill array */
	for(i=0; i<size; ++i) {
		array[i] = rand()%(size*10);
	}
	printf("before:\n");
/*	printf("%s\n", str);
*/
	for(i=0; i<size; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
	
	/* sort */

	BubbleSort( (void*)&array, size, sizeof(int*), CompFuncInt );
/*	BubbleSort( (void*)&str, strlen(str), sizeof(char), CompFuncInt ); */

	printf("after:\n");
/*	printf("%s\n", str);
*/	
	for(i=0; i<size; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
	
	return 0;
}
/*--------------------------------------*/



/* generic bubble sort */
void BubbleSort(void *_data, size_t _nItems, size_t _itemSize, CompFunc _cmpFunc)
{
	int i, j, swapF;
	int compare;
	char *dataChar = _data;
	char *temp = NULL;
	
	temp = malloc(sizeof(char)*_itemSize);
	if(!temp) {
		return;
	}

	for(i=0, swapF = 1; i<_nItems-1 && swapF; ++i) {
		swapF = 0;

		for(j=0; j<(_nItems-i-1); ++j) {
		
			compare = _cmpFunc( _data, j, _itemSize);
		
			if(compare > 0) {
				swapF = 1;
				
				memcpy( (void*)temp, (void*)&dataChar[j*_itemSize], _itemSize );
				memcpy( (void*)&dataChar[j*_itemSize], (void*)&dataChar[(j+1)*_itemSize], _itemSize );
				memcpy( (void*)&dataChar[(j+1)*_itemSize], (void*)temp, _itemSize );
				
			/*	GenericSwapPtrs( (void**)&dataChar[j*_itemSize] , (void**)&dataChar[(j+1)*_itemSize] ); 
				SWAP( dataChar[j*_itemSize], dataChar[(j+1)*_itemSize], char);	*/
			}
		}
	}
	free(temp);
}
/*--------------------------------------*/


/* comparison func - integers */
int  CompFuncInt(void *_data, int _index, size_t _itemSize)
{
	int data1 = *(int*) ( (void*)((char*)_data + _index*_itemSize ));
	int data2 = *(int*) ( (void*)((char*)_data + (_index+1)*_itemSize ));
	
	return data1 - data2;
}
/*--------------------------------------*/


/* swap any 2 pointers of the same type - any type */
void GenericSwapPtrs(void **ptr1, void **ptr2)
{
	void *temp;
	
	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}
/*--------------------------------------*/

