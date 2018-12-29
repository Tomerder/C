#include <stdlib.h>
#include <assert.h>
#include "sorts.h"
#include "limits.h"
#include <math.h>


#define BITS_IN_INT sizeof(int)*8
#define RETURN_ERR if(err!=0) return err


static ADTERR compAndSwap(Vector* _vec,size_t _index1 , size_t _index2, CompFunc _compFunc, int* _isSwap);

static ADTERR swap(Vector* _vec,size_t _index1 , size_t _index2);


static void quicksort(Vector* _vec, int _indexLeft, int _indexRight , CompFunc _compFunc);	
static int partition(Vector* _vec, int _indexLeft, int _indexRight, int _indexPivot , CompFunc _compFunc);


static ADTERR  mergeSort(void** _tempArrOfItemsWork, CompFunc _compFunc , int _startIndex , int _endIndex , void** _tempArrOfItems);
static ADTERR TopDownMerge(void** _tempArrOfItemsWork, CompFunc _compFunc , int _iBegin, int _iMiddle, int _iEnd , void** _tempArrOfItemsOutput);



static int getLengthForRadix(int _radix);
static int getKeyForRadix(void* _item);
/*static ADTERR CountingSortForRadix (Vector* _vec, GetKey _getKeyFunc, int _radix );*/

typedef struct StructForRadix{
	int    s_radix;	
	int    s_index;
    GetKey s_getKeyFunc;
}StructForRadix;

static StructForRadix structForRadix;    /*global static var for passing values to get at GetKeyForRadix func */

/*------------------------------------------------------------------------------------*/

ADTERR BubbleSort (Vector* _vec, CompFunc _compFunc){
	size_t vecItemsNum;
	int i=0 , err=0 , isSwapAtLastPass = 1 , wasSwap = 0; 

	assert(_vec);
	assert(_compFunc);

	vecItemsNum =VectorItemsNum(_vec);

	if(vecItemsNum <= 1){
		return OK;	
	}

	do{
		isSwapAtLastPass = 0;
		for(i=0;i<vecItemsNum-1;i++){    /*bubble 1 item up*/		
			err = compAndSwap(_vec,i,i+1,_compFunc,&wasSwap);		/*check and replace items if needed*/
			RETURN_ERR;		
			if(wasSwap){
				isSwapAtLastPass = 1;
			}		
		}
		vecItemsNum--;	 /*last item will be the biggest*/		
	}while(isSwapAtLastPass);

	return 0;
}

/*------------------------------------------------------------------------------------*/

ADTERR ShakeSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);

	int i=0, err=0, begin=0 , end ,isSwapAtLastPass , wasSwap=0 ;		
	size_t vecItemsNum;  
											
	
	vecItemsNum = VectorItemsNum(_vec);	
	if(vecItemsNum <= 1){
		return OK;	
	}

	begin = 0;
	end = vecItemsNum-1;										
	do{
		isSwapAtLastPass = 0;													
		for(i=begin;i<end;i++){								   /*from start to end - last item will be the biggest*/				
			err = compAndSwap(_vec,i,i+1,_compFunc,&wasSwap);		/*check and replace items if needed*/
			RETURN_ERR;
			if(wasSwap){
				isSwapAtLastPass = 1;
			}	
		}													
		if (!isSwapAtLastPass){
			break;
		}
		end--;													
		
														 										
		for (i=end;i>begin;i--){							   /*from end to start - first item will be the smallest*/
			err = compAndSwap(_vec,i-1,i,_compFunc,&wasSwap);		/*check and replace items if needed*/
			RETURN_ERR;
			if(wasSwap){
				isSwapAtLastPass = 1;
			}	
		}		
		begin++;
	}while(isSwapAtLastPass);

	return OK;											 
}

/*------------------------------------------------------------------------------------*/

ADTERR InsertionSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);
	
	size_t vecItemsNum= VectorItemsNum(_vec);
	int err=0 , i = 1 , holeIndex , wasSwap = 1;		
	void* item1;
    void* item2;
	 
	if(vecItemsNum <= 1){
		return OK;	
	}	
		
	i=1;
	while(i<vecItemsNum){

		holeIndex = i - 1 ; 
		wasSwap = 1;
	    while( holeIndex >= 0 && wasSwap){
			err = compAndSwap(_vec, holeIndex, i , _compFunc ,&wasSwap);	
			RETURN_ERR;
			if(wasSwap){
				i--;
				holeIndex--;
			}
		} 	
				
		i++;
	}

	return OK;
}




 /*TODO fix to in place sort without itemsTempArr*/
/*
ADTERR InsertionSort (Vector* _vec, CompFunc _compFunc){   
	assert(_vec);
	assert(_compFunc);
	
	size_t vecItemsNum= VectorItemsNum(_vec);
	int err=0 , i=1 , holeIndex;		
 	void* itemsTempArr[vecItemsNum];  
	void* item1;
	 
	if(vecItemsNum <= 1){
		return OK;	
	}
	
	err=VectorGet(_vec,0,&item1);
	RETURN_ERR;

	itemsTempArr[0] = item1; 
		
	while(i<vecItemsNum){
		err=VectorGet(_vec,i,&item1);
		RETURN_ERR;

		holeIndex = i;
		
		while(holeIndex>0 && (_compFunc(item1,itemsTempArr[holeIndex-1])<0)){   
			itemsTempArr[holeIndex] = itemsTempArr[holeIndex-1];
			holeIndex--;
		}
		itemsTempArr[holeIndex] = item1; 
		i++;
	}

	
 	i=0;
	while(i<vecItemsNum){
		err=VectorSet(_vec,i,itemsTempArr[i]);
		RETURN_ERR;
		i++;
	}

	return OK;
}
*/
/*------------------------------------------------------------------------------------*/

ADTERR QuickSort (Vector* _vec , CompFunc _compFunc){
	int indexLeft=0;
	int indexRight;	
	size_t vecItemsNum;

	assert(_vec);
	assert(_compFunc);
	
	vecItemsNum = VectorItemsNum(_vec);
	if(vecItemsNum <= 1){
		return OK;	
	}
	
	indexRight = vecItemsNum - 1;
	quicksort(_vec,indexLeft,indexRight, _compFunc);

	return OK;
}


static void quicksort(Vector* _vec, int _indexLeft, int _indexRight , CompFunc _compFunc){
    int pivotNewIndex;
	int indexPivot = (_indexLeft + _indexRight) / 2;     /*choose the middle as pivot*/
     
	if(_indexLeft < _indexRight){    
	    /* Get lists of bigger and smaller items and final position of pivot */
		pivotNewIndex = partition(_vec , _indexLeft, _indexRight , indexPivot , _compFunc);
        
     	quicksort(_vec , _indexLeft , pivotNewIndex-1 , _compFunc);  /* sort items smaller than the pivot */
       
    	quicksort(_vec , pivotNewIndex+1 , _indexRight , _compFunc); /* sort items bigger or equal to the pivot */
	}

}


static int partition(Vector* _vec, int _indexLeft, int _indexRight, int _indexPivot , CompFunc _compFunc){
    void* pivotItem; 
	void* curItem;
	int err=0 , i=0;
	int indexStore = _indexLeft;

    swap(_vec, _indexPivot, _indexRight); /*swap pivot with right*/
	err = VectorGet(_vec, _indexRight, &pivotItem);
	RETURN_ERR;
	 
    for(i = _indexLeft ; i < _indexRight ; i++){   /*from left to right*/
		err = VectorGet(_vec, i, &curItem);
		RETURN_ERR;
        if ( _compFunc(curItem,pivotItem) <= 0){        
			swap(_vec, i ,  indexStore);
            indexStore++;         /* if swapped => increment storeIndex  */
		}
	}	 
	swap(_vec, indexStore ,  _indexRight);   /* Move pivot to its final place*/
	
    return indexStore;
}


/*------------------------------------------------------------------------------------*/

ADTERR MergeSort (Vector* _vec, CompFunc _compFunc){
	size_t vecItemsNum;	
	int err=0 , i=0;
	vecItemsNum = VectorItemsNum(_vec);
	void* tempArrOfItemsOutput[vecItemsNum];  /*local arr to sort items in - output arr*/
	void* tempArrOfItemsWork[vecItemsNum];    /*local arr to sort items in - work arr*/
	void* item;
	
	assert(_vec);
	assert(_compFunc);
	
	
	if(vecItemsNum <= 1){
		return OK;	
	}

	i=0;
	while(i<vecItemsNum){						/*place items from vector in work arr*/
		err = VectorGet(_vec, i, &item);
		RETURN_ERR;
		tempArrOfItemsWork[i] = item;
		i++;
	}					

	mergeSort(tempArrOfItemsWork, _compFunc , 0  , vecItemsNum-1, tempArrOfItemsOutput);  /*merge sort to tempArrOfItems*/
	
	i=0;	
	while(i<vecItemsNum){						/*place sorted items from tempArrOfItems to vector*/
		err = VectorSet(_vec,i,tempArrOfItemsOutput[i]) ;
		RETURN_ERR;
		i++;
	}

	return 0;
}



static ADTERR mergeSort(void** _tempArrOfItemsWork, CompFunc _compFunc , int _startIndex , int _endIndex , void** _tempArrOfItemsOutput){	
	int midIndex = ((_endIndex + _startIndex) / 2) ;	
	
	if(_endIndex - _startIndex < 1) {               /* if run size == 1 item */
        return OK;
	}
	
	mergeSort(_tempArrOfItemsWork, _compFunc, _startIndex , midIndex , _tempArrOfItemsOutput);  /*seperate - left*/
	mergeSort(_tempArrOfItemsWork, _compFunc, midIndex+1  , _endIndex , _tempArrOfItemsOutput); /*seperate - right*/
		
	TopDownMerge(_tempArrOfItemsWork, _compFunc , _startIndex , midIndex+1  , _endIndex , _tempArrOfItemsOutput);/*merge the two parts*/ 
			
	return OK;
}




static ADTERR TopDownMerge(void** _tempArrOfItemsWork, CompFunc _compFunc , int _iBegin, int _iMiddle, int _iEnd , void** _tempArrOfItemsOutput)
{
    int index1 = _iBegin , index2 = _iMiddle , j;
	void* item1;
	void* item2;
	
	   
    for (j = _iBegin; j < _iEnd; j++) {
		item1 = _tempArrOfItemsWork[index1];
		item2 = _tempArrOfItemsWork[index2];
        if (_compFunc(item1,item2) <= 0){
            _tempArrOfItemsOutput[j] = item1;      /*take from left*/
			index1++;	
        }else{
            _tempArrOfItemsOutput[j] = item2;      /*take from right*/
			index2++;
		}
		if(index1 >= _iMiddle || index2 > _iEnd){  /* break when left or right part are finished*/
			break;
		}
    }

	/*add the items that left on one of the parts*/
	j++;
	while(j <= _iEnd){
		if(index1 < _iMiddle){
			_tempArrOfItemsOutput[j] = item1;      /*take from left*/
			index1++;	
			item1 = _tempArrOfItemsWork[index1];
		}else{
			_tempArrOfItemsOutput[j] = item2;      /*take from right*/
			item2 = _tempArrOfItemsWork[index2];
			index2++;
		}			
		j++;
	}

	
	/*  tempArrOfItemsOutput -> tempArrOfItemsWork */
	j=_iBegin;
	while(j <= _iEnd){
		_tempArrOfItemsWork[j] = _tempArrOfItemsOutput[j];
		j++; 
	} 
	 

	return OK;
	
}


/*------------------------------------------------------------------------------------*/


ADTERR CountingSort (Vector* _vec, GetKey _getKeyFunc, int _numOfValues ){  
	assert(_vec);
	assert(_getKeyFunc);

	size_t vecItemsNum;	
	int err=0 , i=0 , key;
	int arrOfKeysCount[_numOfValues] ;
	vecItemsNum = VectorItemsNum(_vec);	
	void* item;
	void* tempArrOfItems[vecItemsNum];   /*local arr to sort items in*/
	
	if(_numOfValues <= 1){
		return INPUT_ERR;
	}
	
	if(vecItemsNum <= 1){
		return OK;	
	}

	i=0;
	while(i < _numOfValues){ 
		arrOfKeysCount[i] = 0;
		i++;
	}

	i=0;
	while(i<vecItemsNum){    /*first loop - count each item */
		err=VectorGet(_vec,i,&item);
		RETURN_ERR;
		arrOfKeysCount[_getKeyFunc(item)]++;
		i++;
	}

	i=1;
	while(i<_numOfValues){      /*second loop - sum smaller items of each index*/
		arrOfKeysCount[i] += arrOfKeysCount[i-1];		
		i++;
	}

 
	i=vecItemsNum-1;
	while(i>=0){    /* fill tempArrOfItems with sorted items - at index = value at (arrOfKeysCount[key]-1) */
		err = VectorGet(_vec,i,&item);
		RETURN_ERR;
		key = _getKeyFunc(item);
		tempArrOfItems[arrOfKeysCount[key]-1] = item; 
		arrOfKeysCount[key]--;
		i--;
	}

	
	i=0;
	while(i<vecItemsNum){    /*fill vector with sorted items from local arr*/
		err = VectorSet(_vec, i, tempArrOfItems[i]);
		RETURN_ERR;
		i++;
	}

	return OK;
		
} 


/*------------------------------------------------------------------------------------*/


ADTERR RadixSort (Vector* _vec, GetKey _getKey, int _radix){   /*if decimal _radix==10*/
	assert(_vec);
	assert(_getKey);

	int vecItemsNum, length, err = 0, i;
	GetKey getKeyForRadixFunc = getKeyForRadix;
	
	structForRadix.s_radix = _radix;
	structForRadix.s_getKeyFunc = _getKey;   /*get key func for counting sort*/ 


	vecItemsNum = VectorItemsNum(_vec);	
	

	if(_radix <= 0){
		return INPUT_ERR;
	}
	if(vecItemsNum <= 1){
		return OK;	
	}
	
	
	length = getLengthForRadix(_radix);   
	
	
	for(i=0 ; i < length ; i++){		
		structForRadix.s_index = i;   /*digit to get key from*/
		err = CountingSort (_vec, getKeyForRadixFunc, _radix );  
		RETURN_ERR;
	}
	
	return OK;

} 




static int getKeyForRadix(void* _item){  /*send to counting sort  =>  item contain 1.item 2.index 3.user getkey func  */
	int i;	
	GetKey getKeyFunc;
	int item , radix , index; 

	radix = structForRadix.s_radix;                /*get radix from singleton struct*/
	getKeyFunc = structForRadix.s_getKeyFunc;      /*get func from singleton struct*/
	index = structForRadix.s_index;                /*get index from singleton struct*/

	item = getKeyFunc(_item);		               /*get item from original user supplied get key func*/			   
	
	/*get to the digit by its index*/
	for(i=0; i < index; i++){
		item = item / radix; 
	}   
	item = item % radix; 

	
	
	return item;
}




static int getLengthForRadix(int _radix){
	int i=0 , maxNumValue;
	/*int bitsInInt = BITS_IN_INT - 1 ;*/	
	maxNumValue = INT_MAX;  /*pow(2, bitsInInt);*/

	while(maxNumValue > 0){
		maxNumValue = maxNumValue / _radix;
		i++;
	}
	
 	
	return i;
}


/*------------------------------------------------------------------------------------*/


/*
ADTERR RadixSort (Vector* _vec, GetKeyForRadix _getKeyForRadixFunc, int _radix , int _numOfValuesToEachDigit){ 
	assert(_vec);
	assert(_getKeyForRadixFunc);

	size_t vecItemsNum;	
	int i=0 , err=0;
	vecItemsNum = VectorItemsNum(_vec);	

	if(_radix <= 0){
		return INPUT_ERR;
	}
	
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}
	
	for (i = 1 ; i <= _radix ; i++){   
        err = CountingSortForRadix(_vec, _getKeyForRadixFunc,  _numOfValuesToEachDigit , i );  
		if(err != 0){
			return err;
		}
	}

	return 0;

}



static ADTERR CountingSortForRadix (Vector* _vec, GetKeyForRadix _getKeyFuncForRadix , int _valuesPerDigit  , int _digitNum ){  
	assert(_vec);
	assert(_getKeyFuncForRadix);
	size_t vecItemsNum;	
	int i=0 , err=0 ,key;
	int arrOfKeysCount[_valuesPerDigit] ;
	vecItemsNum = VectorItemsNum(_vec);	
	void* item;
	void* tempArrOfItems[vecItemsNum];   
	
	if(_valuesPerDigit <= 1){
		return INPUT_ERR;
	}
	
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}

	i=0;
	while(i<_valuesPerDigit){ 
		arrOfKeysCount[i] = 0;
		i++;
	}

	i=0;
	while(i<vecItemsNum){    
		err = VectorGet(_vec,i,&item);
		if(err!=0){
			return err;
		}
		arrOfKeysCount[_getKeyFuncForRadix(item,_digitNum)]++;
		i++;
	}

	i=1;
	while(i<_valuesPerDigit){    
		arrOfKeysCount[i] += arrOfKeysCount[i-1];		
		i++;
	}

 
	i=vecItemsNum-1;
	while(i>=0){   
		err = VectorGet(_vec,i,&item);
		if(err!=0){
			return err;
		}
		key = _getKeyFuncForRadix(item, _digitNum);
		tempArrOfItems[arrOfKeysCount[key]-1] = item; 
		arrOfKeysCount[key]--;
		i--;
	}

	
	i=0;
	while(i<vecItemsNum){   
		err = VectorSet(_vec, i, tempArrOfItems[i]);
		if(err!=0){
			return err;
		}
		i++;
	}

	return 0;
		


} 
*/

/*------------------------------------------------------------------------------------*/



static ADTERR compAndSwap(Vector* _vec,size_t _index1 , size_t _index2, CompFunc _compFunc,int* _isSwap){
	void* item1;
	void* item2;
	int err=0;
	
	*_isSwap = 0;
	
	err = VectorGet(_vec,_index1,&item1); /*   &(void*) != void**   -> need to send the adress of what we wish to change => &(void*)  */
	RETURN_ERR;	

	err = VectorGet(_vec,_index2,&item2);
	RETURN_ERR;	
	

	if(_compFunc(item1,item2)>0){
		*_isSwap = 1;
		
		err = VectorSet(_vec, _index2 , item1);
		RETURN_ERR;	

		err = VectorSet(_vec, _index1 , item2);
		RETURN_ERR;	
	}

	return OK;
	
}

/*------------------------------------------------------------------------------------*/


static ADTERR swap(Vector* _vec,size_t _index1 , size_t _index2){
	void* item1;
	void* item2;
	int err=0;
	
	err = VectorGet(_vec,_index1,&item1); /*   &(void*) != void**   -> need to send the adress of what we wish to change => &(void*)  */
	RETURN_ERR;	

	err = VectorGet(_vec,_index2,&item2);
	RETURN_ERR;	
		
	err = VectorSet(_vec, _index2 , item1);
	RETURN_ERR;	

	err = VectorSet(_vec, _index1 , item2);
	RETURN_ERR;	

	return OK;
	
}



/*------------------------------------------------------------------------------------*/

ADTERR SomeSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);
	
	int i=0 ,j=0;
	int err=0;	
	size_t vecItemsNum=VectorItemsNum(_vec);
	int isSwap=0;	 
	  
	
	if(vecItemsNum == 0){
		return EMPTY_OR_ONE;	
	}


	for(i=0;i<vecItemsNum;i++){    /*first loop*/
		for(j=i+1;j<vecItemsNum;j++){    /*second loop*/		
			err = compAndSwap(_vec,i,j,_compFunc,&isSwap);		/*check and replace items if needed*/
			RETURN_ERR;					
		}	
	}

	return OK;
}


/*------------------------------------------------------------------------------------*/

