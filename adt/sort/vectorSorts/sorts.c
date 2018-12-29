#include <stdlib.h>
#include <assert.h>
#include "sorts.h"




static ADTERR compAndSwap(Vector* _vec,size_t _index1 , size_t _index2, CompFunc _compFunc, int* _isSwap);

static ADTERR swap(Vector* _vec,size_t _index1 , size_t _index2);


static void quicksort(Vector* _vec, int _indexLeft, int _indexRight , CompFunc _compFunc);	
static int partition(Vector* _vec, int _indexLeft, int _indexRight, int _indexPivot , CompFunc _compFunc);


static ADTERR  mergeSort(void** _tempArrOfItemsWork, CompFunc _compFunc , int _startIndex , int _endIndex , void** _tempArrOfItems);
static ADTERR TopDownMerge(void** _tempArrOfItemsWork, CompFunc _compFunc , int _iBegin, int _iMiddle, int _iEnd , void** _tempArrOfItemsOutput);

/*static int getRadix(int _maxNumValue);*/
static ADTERR CountingSortForRadix (Vector* _vec, GetKeyForRadix _getKeyFunc, int _maxValue , int digitNum );

/*------------------------------------------------------------------------------------*/

ADTERR BubbleSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);
	
	size_t vecItemsNum;
	int i=0 , err=0 , isSwap=1 , n , isSwapAtLastPass = 1; 
	vecItemsNum =VectorItemsNum(_vec);
	n = vecItemsNum;

	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}

	while(isSwapAtLastPass){
		isSwapAtLastPass = 0;
		for(i=0;i<n-1;i++){    /*second loop*/		
			err = compAndSwap(_vec,i,i+1,_compFunc,&isSwap);		/*check and replace items if needed*/
			if(err != 0){
				return err;
			}
			if(isSwap){
				isSwapAtLastPass = isSwap;
			}					
		}
		n--;	 /*last item will be the biggest*/		
	}

	return 0;
}

/*------------------------------------------------------------------------------------*/

ADTERR ShakeSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);

	int i=0;
	int err=0;		
	size_t vecItemsNum; 	
	int begin = -1;													
	int end; 
	int isSwap=0;	
	int isSwapAtLastPass = 1; 												
	
	vecItemsNum = VectorItemsNum(_vec);	
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}

	end = vecItemsNum-1;										
	while(isSwapAtLastPass){
		isSwapAtLastPass = 0;
		begin++;													
		for(i=begin;i<end;i++){								   /*from start to end - last item will be the biggest*/				
			err = compAndSwap(_vec,i,i+1,_compFunc,&isSwap);		/*check and replace items if needed*/
			if(err != 0){
				return err;
			}	
			if(isSwap){
				isSwapAtLastPass = isSwap;
			}	
		}													
		if (!isSwapAtLastPass){
			break;
		}
														 										
		end--;													
		for (i=end;i>begin;i--){							   /*from end to start - first item will be the smallest*/
			err = compAndSwap(_vec,i-1,i,_compFunc,&isSwap);		/*check and replace items if needed*/
			if(err != 0){
				return err;
			}	
			if(isSwap){
				isSwapAtLastPass = isSwap;
			}
		}		
	}

	return 0;											 
}

/*------------------------------------------------------------------------------------*/

ADTERR InsertionSort (Vector* _vec, CompFunc _compFunc){
	assert(_vec);
	assert(_compFunc);
	
	size_t vecItemsNum= VectorItemsNum(_vec);
	int err=0 , i=1;
	int holeIndex;		
 	void* itemsTempArr[vecItemsNum];   /*local arr to save pointers to sorted items*/ 
	void* item1;
	 
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}
	
	err=VectorGet(_vec,0,&item1);
	if(err!=0){
		return err;
	}

	itemsTempArr[0] = item1; 
		
	while(i<vecItemsNum){
		err=VectorGet(_vec,i,&item1);
		if(err!=0){
			return err;
		}
		holeIndex = i;
		/*while next item from vec is smaller then previous item at arr => move the hole "down" by advancing items in arr*/
		while(holeIndex>0 && (_compFunc(item1,itemsTempArr[holeIndex-1])<0)){   
			itemsTempArr[holeIndex] = itemsTempArr[holeIndex-1];
			holeIndex--;
		}
		itemsTempArr[holeIndex] = item1; /*place item from vec at hole in arr*/
		i++;
	}

	/*set Items at vector according to arr*/
 	i=0;
	while(i<vecItemsNum){
		err=VectorSet(_vec,i,itemsTempArr[i]);
		if(err!=0){
			return err;
		}
		i++;
	}

	return 0;
}

/*------------------------------------------------------------------------------------*/

ADTERR QuickSort (Vector* _vec , CompFunc _compFunc){
	int indexLeft=0;
	int indexRight;	
	size_t vecItemsNum;

	assert(_vec);
	assert(_compFunc);
	
	vecItemsNum = VectorItemsNum(_vec);
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}
	
	indexRight = vecItemsNum - 1;
	quicksort(_vec,indexLeft,indexRight, _compFunc);

	return 0;
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
	if(err!=0){
		return err;
	}
	 
    for(i = _indexLeft ; i < _indexRight ; i++){   /*from left to right*/
		err = VectorGet(_vec, i, &curItem);
		if(err!=0){
			return err;
		}	
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
		return EMPTY_OR_ONE;	
	}

	i=0;
	while(i<vecItemsNum){						/*place items from vector in work arr*/
		err = VectorGet(_vec, i, &item);
		if(err!=0){
			return err;
		}
		tempArrOfItemsWork[i] = item;
		i++;
	}					

	mergeSort(tempArrOfItemsWork, _compFunc , 0  , vecItemsNum-1, tempArrOfItemsOutput);  /*merge sort to tempArrOfItems*/
	
	i=0;	
	while(i<vecItemsNum){						/*place sorted items from tempArrOfItems to vector*/
		err = VectorSet(_vec,i,tempArrOfItemsOutput[i]) ;
		if(err!=0){
			return err;
		}
		i++;
	}

	return 0;
}



static ADTERR mergeSort(void** _tempArrOfItemsWork, CompFunc _compFunc , int _startIndex , int _endIndex , void** _tempArrOfItemsOutput){	
	int midIndex = ((_endIndex + _startIndex) / 2) ;	
	
	if(_endIndex - _startIndex < 1) {               /* if run size == 1 item */
        return 0;
	}
	
	mergeSort(_tempArrOfItemsWork, _compFunc, _startIndex , midIndex , _tempArrOfItemsOutput);  /*seperate - left*/
	mergeSort(_tempArrOfItemsWork, _compFunc, midIndex+1  , _endIndex , _tempArrOfItemsOutput); /*seperate - right*/
		
	TopDownMerge(_tempArrOfItemsWork, _compFunc , _startIndex , midIndex+1  , _endIndex , _tempArrOfItemsOutput);/*merge the two parts*/ 
			
	return 0;
}




static ADTERR TopDownMerge(void** _tempArrOfItemsWork, CompFunc _compFunc , int _iBegin, int _iMiddle, int _iEnd , void** _tempArrOfItemsOutput)
{
    int index1 = _iBegin;
	int index2 = _iMiddle;
	int j;
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
	 


	return 0;
	
}


/*------------------------------------------------------------------------------------*/


ADTERR CountingSort (Vector* _vec, GetKey _getKeyFunc, int _numOfValues ){  
	assert(_vec);
	assert(_getKeyFunc);

	size_t vecItemsNum;	
	int err=0 , i=0;
	int key;
	int arrOfKeysCount[_numOfValues] ;
	vecItemsNum = VectorItemsNum(_vec);	
	void* item;
	void* tempArrOfItems[vecItemsNum];   /*local arr to sort items in*/
	
	if(_numOfValues <= 1){
		return INPUT_ERR;
	}
	
	if(vecItemsNum <= 1){
		return EMPTY_OR_ONE;	
	}

	i=0;
	while(i < _numOfValues){ 
		arrOfKeysCount[i] = 0;
		i++;
	}

	i=0;
	while(i<vecItemsNum){    /*first loop - count each item */
		err=VectorGet(_vec,i,&item);
		if(err !=0){
			return err;
		}
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
		if(err!=0){
			return err;
		}
		key = _getKeyFunc(item);
		tempArrOfItems[arrOfKeysCount[key]-1] = item; 
		arrOfKeysCount[key]--;
		i--;
	}

	
	i=0;
	while(i<vecItemsNum){    /*fill vector with sorted items from local arr*/
		err = VectorSet(_vec, i, tempArrOfItems[i]);
		if(err!=0){
			return err;
		}
		i++;
	}

	return 0;
		
} 


/*------------------------------------------------------------------------------------*/


ADTERR RadixSort (Vector* _vec, GetKeyForRadix _getKeyForRadixFunc, int _radix , int _numOfValuesToEachDigit){ /*radix = length */
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
	
	for (i = 1 ; i <= _radix ; i++){   /*going through the item digit by digit LS->MS */
		 /* call counting sort with range of radix (e.g (0-9)-> K=10)  for the i digit - starting from LS to MS - using the attribute that 
             counting sort is a stable sort */   
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
	void* tempArrOfItems[vecItemsNum];   /*local arr to sort items in*/
	
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
	while(i<vecItemsNum){    /*first loop - count each item */
		err = VectorGet(_vec,i,&item);
		if(err!=0){
			return err;
		}
		arrOfKeysCount[_getKeyFuncForRadix(item,_digitNum)]++;
		i++;
	}

	i=1;
	while(i<_valuesPerDigit){      /*second loop - sum smaller items of each index*/
		arrOfKeysCount[i] += arrOfKeysCount[i-1];		
		i++;
	}

 
	i=vecItemsNum-1;
	while(i>=0){    /* fill tempArrOfItems with sorted items - at index = value at (arrOfKeysCount[key]-1) */
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
	while(i<vecItemsNum){    /*fill vector with sorted items from local arr*/
		err = VectorSet(_vec, i, tempArrOfItems[i]);
		if(err!=0){
			return err;
		}
		i++;
	}

	return 0;
		


} 


/*------------------------------------------------------------------------------------*/



static ADTERR compAndSwap(Vector* _vec,size_t _index1 , size_t _index2, CompFunc _compFunc,int* _isSwap){
	void* item1;
	void* item2;
	int err=0;
	
	*_isSwap = 0;
	
	err = VectorGet(_vec,_index1,&item1); /*   &(void*) != void**   -> need to send the adress of what we wish to change => &(void*)  */
	if(err !=0){
		return err;
	}
	err = VectorGet(_vec,_index2,&item2);
	if(err !=0 ){	
		return err;
	}
	

	if(_compFunc(item1,item2)>0){
		*_isSwap = 1;
		err = VectorSet(_vec, _index2 , item1);
		if(err !=0){	
			return err;
		}
		err = VectorSet(_vec, _index1 , item2);
		if(err !=0){	
			return err;
		}
	}

	return 0;
	
}

/*------------------------------------------------------------------------------------*/


static ADTERR swap(Vector* _vec,size_t _index1 , size_t _index2){
	void* item1;
	void* item2;
	int err=0;
	
	err = VectorGet(_vec,_index1,&item1); /*   &(void*) != void**   -> need to send the adress of what we wish to change => &(void*)  */
	if(err !=0){
		return err;
	}
	err = VectorGet(_vec,_index2,&item2);
	if(err !=0 ){	
		return err;
	}
	
	
	err = VectorSet(_vec, _index2 , item1);
	if(err !=0){	
		return err;
	}
	err = VectorSet(_vec, _index1 , item2);
	if(err !=0){	
		return err;
	}

	return 0;
	
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
			if(err != 0){
				return err;
			}						
		}	
	}

	return 0;
}


/*------------------------------------------------------------------------------------*/

