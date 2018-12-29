#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "vec.h"
#include "err.h"




void printInt(Vector* _vec);

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	
	main1();  /*checks flow - insert , realloc , remove ,decrease size , set ,get , print 
			    changing object by the user and make sure it changes in the vector
			    make sure adrress of item that we get from vector equals to address of object that has been set to the vector */
	main2();  /*block size = 0*/
	main3();  /*size = 0*/
	main4();  /*check vector is not decreased more then origSize*/
	main5();  /*remove when vector is empty */

    return 0; 	
}

/*------------------------------------------------------------------------------------*/	
	
int main1(){
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

	printf("insert*7\n");	

	if(err=VectorInsert(vec,(void*)(&arrOfInts[0]))!=0){    /*insert*/
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[1]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[2]))!=0){    //insert    -  check realloc 
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[3]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[4]))!=0){    //insert    - second realoc
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[5]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[6]))!=0){    //insert     - third realloc
		printf("error no. %d\n",err);
	}
	
	
	printf("VectorItemsNum : %d\n",VectorItemsNum(vec));    //itemsNum
	printf("VectorSize : %d\n",(int)VectorSize(vec));	 //vec size	

	VectorPrint(vec,printIntsfunc);    //print
	
	
	err = VectorRemove(vec, &(voidToGet)    ) ;          //remove void** + casting
	if(err!=0){   
		printf("error no. %d\n",err);
	}
	printf("remove : %d\n",*((int*)voidToGet)); 


	err= VectorGet(vec, 0 ,&(voidToGet)    ) ;          //get void** + casting
	if(err!=0){   
		printf("error no. %d\n",err);
	}
	printf("get(0)=%d\n",*((int*)voidToGet));

	err=VectorGet(vec, 1 ,(void**)(&(numToGet))    ) ;          //get
	if(err!=0){   
		printf("error no. %d\n",err);
	}
	printf("get(1)=%d\n",*(numToGet));


	err=VectorSet(vec, 2 ,(void*)(&numToSet)    ) ;             //set
	if(err!=0){   
		printf("error no. %d\n",err);
	}
	printf("set(2)=555\n");	

	//make sure adrress of item that we get from vector equals to address of object that has been set to the vector 
	printf("object adress : %d\n", (int)&numToSet );   
	
	err=VectorGet(vec, 2 ,(void**)(&(numToGet))    ) ;          //get
	if(err!=0){   
		printf("error no. %d\n",err);
	}
	printf("get(2) address :%d\n",(int)numToGet);
		
	
	VectorPrint(vec,printIntsfunc);    //print
	
	numToSet = 444;                    //change object
	printf("change object 555->444\n");		
	
	VectorPrint(vec,printIntsfunc);    //print


	printf("remove*4\n");		
	VectorRemove(vec, (void**)(&(numToGet2))   ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove


	VectorPrint(vec,printIntsfunc);    //print

	printf("VectorItemsNum : %d\n",VectorItemsNum(vec));    //itemsNum
	printf("VectorSize : %d\n",(int)VectorSize(vec));	    //vec size   -  vector size decreased

	VectorDestroy(vec);

}

/*------------------------------------------------------------------------------------*/
	
int main2(){
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

	if(err=VectorInsert(vec,(void*)(&arrOfInts[0]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[1]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	if(err=VectorInsert(vec,(void*)(&arrOfInts[2]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	VectorDestroy(vec);

}


/*------------------------------------------------------------------------------------*/
	

int main3(){
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int numToSet = 555;
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	int err;
	Vector* vec=VectorCreate(0,2);
	PrintFunc printIntsfunc = printInt;

	printf("\nmain33333333:  size=0 blockSize=2 \n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	
	printf("insert\n");
	if(err=VectorInsert(vec,(void*)(&arrOfInts[0]))!=0){    //insert
		printf("error no. %d\n",err);
	}	

	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("insert\n");
	if(err=VectorInsert(vec,(void*)(&arrOfInts[1]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("insert\n");	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[2]))!=0){    //insert
		printf("error no. %d\n",err);
	}
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	
	VectorDestroy(vec);

}


/*------------------------------------------------------------------------------------*/


int main4(){
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int numToSet = 555;
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	int err;
	Vector* vec=VectorCreate(4,2);
	PrintFunc printIntsfunc = printInt;

	printf("\nmain4444444:  check vector is not decreased more then origSize size=3 block=3 \n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	
	printf("insert*7\n");
	if(err=VectorInsert(vec,(void*)(&arrOfInts[0]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[1]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[2]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[3]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[4]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[5]))!=0){    //insert
		printf("error no. %d\n",err);
	}	
	if(err=VectorInsert(vec,(void*)(&arrOfInts[6]))!=0){    //insert
		printf("error no. %d\n",err);
	}	

	
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("remove*2\n");		
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("remove*2\n");		
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("remove*2\n");		
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove
	printf("VectorSize : %d\n",(int)VectorSize(vec));
	printf("remove\n");		
	VectorRemove(vec, (void**)(&(numToGet2))    ) ;          //remove	
	printf("VectorSize : %d\n",(int)VectorSize(vec));

	VectorDestroy(vec);

}	

/*------------------------------------------------------------------------------------*/

int main5(){
	int arrOfInts[7]={5,7,15,777,888,999,222};
	int numToSet = 555;
	int* numToGet=NULL;
	int* numToGet2=NULL;
	void* voidToGet=NULL;
	int err;
	Vector* vec=VectorCreate(2,2);
	PrintFunc printIntsfunc = printInt;

	printf("\nmain5555555:  remove when empty \n\n");
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));	
	printf("insert\n");
	if(err=VectorInsert(vec,&arrOfInts[0])!=0){    //insert
		printf("error no. %d\n",err);
	}	

	
	printf("VectorSize : %d\n",(int)VectorSize(vec));

	printf("remove*4\n");		
	VectorRemove(vec, (void**)&(numToGet2)    ) ;          //remove
	VectorRemove(vec, (void**)&(numToGet2)    ) ;          //remove
	VectorRemove(vec, (void**)&(numToGet2)    ) ;          //remove
	VectorRemove(vec, (void**)&(numToGet2)    ) ;          //remove
	
	printf("VectorSize : %d\n",(int)VectorSize(vec));

	VectorDestroy(vec);

}



/*------------------------------------------------------------------------------------*/

void printInt(Vector* _vec){
	int iter=0;
	int* toGet;	

	printf("\n"); 
	while(iter < VectorItemsNum(_vec) ){
		VectorGet (_vec,iter,(void**)&toGet) ;
		printf("%d , ", *toGet);		
		iter++;
	}
	printf("\n\n"); 
}

/*------------------------------------------------------------------------------------*/

