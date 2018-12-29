/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/*------------------------------------------------------------------------------------*/

/*#include "list.h"*/
#include "DogList.h"
#include "CatList.h"

/*------------------------------------------------------------------------------------*/

typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

#define NUM_OF_ITEMS 8

#define RETURN_ERR if(err!=0) printf("FAIL:%d ",err)


typedef enum {FAIL,PASS} Result;
char* resultArr[2] = {"FAIL","PASS"};
#define SUCCESS 1

#define DEBUG 0

/*------------------------------------------------------------------------------------*/

void TestDogCat();

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{	
	TestDogCat();
	
    return 0; 	
}

/*------------------------------------------------------------------------------------*/


void TestDogCat()
{
	DogList* doglist;
	CatList* catlist;
	
	Dog dog1; 
	Dog* dogToGet ;
	
	Cat cat1; 
	Cat* catToGet ;
	
	dog1.m_hauHau = 777;
	cat1.m_miauMiau = 555;
	
	printf("/------------------------------------------------------/\n");
	
	doglist = DogListCreate();
	
	DogListPushHead(doglist, &dog1);
	
	DogListPopHead(doglist, &dogToGet  )  ;
	
	printf("Dog to get : %d\n", dogToGet->m_hauHau );  
	
	DogListDestroy(doglist);
	
	printf("/------------------------------------------------------/\n");
	
	catlist = CatListCreate();
	
	CatListPushHead(catlist, &cat1);
	/*CatListPushHead(catlist, &dog1); */            /*warning - parm 2*/
	/*CatListPushHead(doglist, &cat1); */            /*warning - parm 1*/
	
	CatListPopHead(catlist, &catToGet);
	/*CatListPopHead(catlist, &dogToGet); */         /*warning - parm 2*/
	/*CatListPopHead(doglist, &dogToGet); */         /*warning - parm 1 & parm 2*/
	
	printf("Cat to get : %d\n", catToGet->m_miauMiau );  
	
	CatListDestroy(catlist);
	
	
	printf("/------------------------------------------------------/\n");
	
	
}



