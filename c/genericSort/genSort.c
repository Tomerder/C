#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

/*------------------------------------------------------------------------------------*/

#define ARR_SIZE 10
#define MAX_VAL 100

enum{FALSE , TRUE};

typedef int(*CompFunc)(void* _item1, void* _item2);

/*------------------------------------------------------------------------------------*/

int CompInt(void* _item1,void* _item2);
void SwapMem(void* _x, void* _y, int _size);
void BubbleSort (void* _elements, size_t _sizeOfElement , size_t _numOfElements ,CompFunc _compFunc);

/*------------------------------------------------------------------------------------*/

int main()
{
	CompFunc IntCompFunc = CompInt;
	int arrOfInts[ARR_SIZE];
	int i;
	
	srand(time(NULL));
	for(i = 0; i < ARR_SIZE ; i++){		
		arrOfInts[i] = rand() % MAX_VAL; 
		printf(" %d , " , arrOfInts[i] );
	}
	printf("\n");
	
	/*----------------------------------------------------------*/
	BubbleSort(arrOfInts , sizeof(int) , ARR_SIZE , IntCompFunc);
	/*----------------------------------------------------------*/
	
	for(i = 0; i < ARR_SIZE ; i++){		
		printf(" %d , " , arrOfInts[i] );
	}
	printf("\n");
	
	return 0;

}

/*------------------------------------------------------------------------------------*/

void BubbleSort (void* _elements, size_t _sizeOfElement , size_t _numOfElements ,CompFunc _compFunc)
{
	int i=0 , wasSwapAtLastPass = TRUE ; 

	assert(_elements);
	assert(_compFunc);

	if(_numOfElements <= 1){
		return;	
	}

	do{
		wasSwapAtLastPass = FALSE;
		for(i=0; i<_numOfElements-1; i++){    /*bubble 1 item up*/		
			if (  _compFunc( ( (char*)_elements + (i*_sizeOfElement)) , ((char*)_elements + ((i+1)*_sizeOfElement))  ) > 0 ){
				wasSwapAtLastPass = TRUE;
				SwapMem(  ((char*)_elements + (i*_sizeOfElement)) , ((char*)_elements + ((i+1)*_sizeOfElement)) , _sizeOfElement ); 
			}		
		}
		--_numOfElements;	 /*last item will be the biggest*/		
	}while(wasSwapAtLastPass);

}

/*------------------------------------------------------------------------------------*/

void SwapMem(void* _x, void* _y, int _size){
	void* tmp = malloc(_size);
	
	memcpy(tmp , _x , _size  );
	memcpy(_x , _y  , _size  );
	memcpy(_y , tmp  , _size  );
	
	free(tmp);
	
}

/*------------------------------------------------------------------------------------*/

int CompInt(void* _item1,void* _item2){
	int item1 = (*((int*)_item1)) ;
	int item2 = (*((int*)_item2)) ;
	return ( item1 -item2 );
}

/*------------------------------------------------------------------------------------*/
