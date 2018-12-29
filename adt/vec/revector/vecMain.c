#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "vec.h"

void printInt(Vector* _vec);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	
	main1();
	main2();  //block size = 0
	main3();  //size = 0

    return 0; 	
}

/*------------------------------------------------------------------------------------*/	
	
void main1(){
	int numToSet = 555;
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	Vector* vec=VectorCreate(2,2);
	PrintFunc printIntsfunc = printInt;
	int err;

	printf("main1111111111:  size=2 blockSize=2\n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	

	if(err=VectorInsert(vec,&arrOfInts[0])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[1])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[2])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[3])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[4])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[5])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[6])!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	printf("insert*7\n");	
	
	
	printf("VectorItemsNum : %d\n",VectorItemsNum(vec));    //itemsNum
	printf("VectorSize : %d\n",(int)VectorSize(vec));	 //vec size	

	VectorPrint(vec,printIntsfunc);    //print
	
	
	VectorRemove(vec, &(voidToGet)    ) ;          //remove void** + casting
	printf("remove : %d\n",*((int*)voidToGet));


	VectorGet(vec, 0 ,&(voidToGet)    ) ;          //get void** + casting
	printf("get(0)=%d\n",*((int*)voidToGet));

	VectorGet(vec, 1 ,&(numToGet)    ) ;          //get
	printf("get(1)=%d\n",*(numToGet));


	VectorSet(vec, 2 , &numToSet    ) ;             //set
	printf("set(2)=555\n");	

	VectorPrint(vec,printIntsfunc);    //print
	
	numToSet = 444;                    //change object
	printf("change object 555->444\n");		
	
	VectorPrint(vec,printIntsfunc);    //print


	printf("remove*4\n");		
	VectorRemove(vec, &(numToGet2)    ) ;          //remove
	VectorRemove(vec, &(numToGet2)    ) ;          //remove
	VectorRemove(vec, &(numToGet2)    ) ;          //remove
	VectorRemove(vec, &(numToGet2)    ) ;          //remove


	VectorPrint(vec,printIntsfunc);    //print

	printf("VectorItemsNum : %d\n",VectorItemsNum(vec));    //itemsNum
	printf("VectorSize : %d\n",(int)VectorSize(vec));	    //vec size

	VectorDestroy(vec);

}

/*------------------------------------------------------------------------------------*/
	
void main2(){
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int numToSet = 555;
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	int err;
	Vector* vec=VectorCreate(2,0);
	PrintFunc printIntsfunc = printInt;

	printf("\nmain222222222:  size=2 blockSize=0 -> overflow \n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	

	printf("insert*3\n");

	if(err=VectorInsert(vec,&arrOfInts[0])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[1])!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,&arrOfInts[2])!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	VectorDestroy(vec);

}


/*------------------------------------------------------------------------------------*/
	

void main3(){
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int numToSet = 555;
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	int err;
	Vector* vec=VectorCreate(0,2);
	PrintFunc printIntsfunc = printInt;

	printf("\nmain33333333:  size=0 blockSize=2 -> overflow \n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	
	printf("insert\n");
	if(err=VectorInsert(vec,&arrOfInts[0])!=0){    //insert
		printf("error no. %d\n",err);
	}	

	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("insert\n");
	if(err=VectorInsert(vec,&arrOfInts[1])!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("insert\n");	
	if(err=VectorInsert(vec,&arrOfInts[2])!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	
	VectorDestroy(vec);

}



/*------------------------------------------------------------------------------------*/

void printInt(Vector* _vec){
	int iter=0;
	
	printf("\n"); 
	while(iter < _vec->m_nItems){
		printf("%d , ", *(   (int*)(_vec->m_data[iter]) )      );		
		iter++;
	}
	printf("\n\n"); 
}

/*------------------------------------------------------------------------------------*/

