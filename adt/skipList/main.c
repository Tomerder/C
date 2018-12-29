/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include "skipList.h"

typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

#define NUM_OF_ITEMS 8

#define RETURN_ERR if(err!=0) printf("FAIL:%d ",err)


typedef enum {FAIL,PASS} Result;
char* resultArr[2] = {"FAIL","PASS"};
#define SUCCESS 1

#define DEBUG 0


/*------------------------------------------------------------------------------------*/

int	DataCompInt (Data _data1, Data _data2)
{
	if ( (int)_data1 == (int)_data2 ) 
		return 0;
	else if ( (int)_data1 < (int)_data2 ) {
		return -1;
	}else{
		return 1;
	}
}

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	SkipList* skipList = NULL;
	SkipListItr itr;
	int toInsert = 1;
	
	skipList = SkipListCreate(DataCompInt);
	
	PrintSkip(skipList);

	printf("/------------------------------------------------------/\n");

	itr = SkipListFind(skipList, (Data)INT_MAX) ;
	printf( "found : %d\n",  (int)SkipListGetData(itr)    );

	printf("/------------------------------------------------------/\n");

	printf("/*******************/\n");
	printf("insert 1\n");
	SkipListInsert(skipList, (Data)toInsert);
	printf("Levels: %d\n", GetNumOfLevels(skipList));
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 5\n");
	SkipListInsert(skipList, (Data)5);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 18\n");
	SkipListInsert(skipList, (Data)18);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 25\n");
	SkipListInsert(skipList, (Data)25);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 9\n");
	SkipListInsert(skipList, (Data)9);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 7\n");
	SkipListInsert(skipList, (Data)7);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 77\n");
	SkipListInsert(skipList, (Data)77);
	printf("Levels: %d\n", GetNumOfLevels(skipList));
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 777\n");
	SkipListInsert(skipList, (Data)777);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 7777\n");
	SkipListInsert(skipList, (Data)7777);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/*******************/\n");
	printf("insert 77777\n");
	SkipListInsert(skipList, (Data)77777);
	printf("Levels: %d\n", GetNumOfLevels(skipList));	
	PrintSkipAllLevels(skipList);
	
	printf("/------------------------------------------------------/\n");
	
	PrintSkip(skipList);

	printf("/------------------------------------------------------/\n");
	
	itr = SkipListFind(skipList, (Data)25) ;
	if(itr == SkipListEnd(skipList)){
		printf("Error : 25 wasnt found\n");
	}else{
		printf( "found : %d\n",  (int)SkipListGetData(itr)    );
	}
	
	itr = SkipListFind(skipList, (Data)9) ;
	if(itr == SkipListEnd(skipList)){
		printf("Error : 9 wasnt found\n");
	}else{
		printf( "found : %d\n",  (int)SkipListGetData(itr)    );
	}
	
	itr = SkipListFind(skipList, (Data)50) ;
	if(itr != SkipListEnd(skipList)){
		printf("Error found 50!!!!\n");
	}
	
	printf("/------------------------------------------------------/\n");
	
	PrintSkipAllLevels(skipList);
	
	
    return 0; 	

    
}

/*------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------*/

















