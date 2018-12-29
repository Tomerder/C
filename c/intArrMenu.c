#include<stdio.h>
#define EXPEND_VALUE 4
#define N 4


typedef struct intArr{
	int* adr;
	int curIndex;
	int size;	
}intArr;



void Insert(intArr* arr);
void Find(intArr arr);
void Remove(intArr* arr);

int* create(int n);      /*  ==>     void create(int n , int** adr);     */
void insertNum(intArr* arr, int num);
int removeNum(intArr* arr , int num);
void printArr(intArr arr); 


int main(int argc,char** argv)
{
	intArr arr1;	
	
      
    arr1.curIndex = 0;
    arr1.adr = create(N);	
	arr1.size = N;
    
	
	int cont = 1;			
	unsigned int option;
	
	while (cont) {
		printf("Choose option: \n");
		printf("1: Insert  \n");
		printf("2: Find    \n");
		printf("3: Remove  \n");
		printf("Any another number - stop \n");

		scanf("%d", &option);
		switch (option) {
			case 1:
				Insert(&arr1);	break;
			case 2:
				Find(arr1);		break;
			case 3:
				Remove(&arr1);	break;

			default: cont = 0;	break;
		}
	}

	
	

 
/*	  
    insertNum(&arr1 , 1) ;
    printArr(arr1);
    insertNum(&arr1 , 2) ;
    printArr(arr1);
    insertNum(&arr1 , 3) ;
    printArr(arr1);
    insertNum(&arr1 , 4) ;
    printArr(arr1);
    insertNum(&arr1 , 5) ;
    printArr(arr1);	
    insertNum(&arr1 , 6) ;
    printArr(arr1);	
    insertNum(&arr1 , 7) ;
    printArr(arr1);	  
	insertNum(&arr1 , 8) ;
    printArr(arr1);	
    insertNum(&arr1 , 9) ;
    printArr(arr1);	
	insertNum(&arr1 , 10) ;
    printArr(arr1);	    
    
	printf("remove 5\n");  
	removeNum(&arr1 ,5); 
    printArr(arr1);	
	printf("remove 1\n");    
    removeNum(&arr1 ,1);
	printArr(arr1);	
	printf("remove 9\n");  
	removeNum(&arr1 ,9);		 
    printArr(arr1);	   	
	printf("remove 55\n");  
	removeNum(&arr1 ,55);
 	printArr(arr1);	
*/

    free(arr1.adr);	
    return 0; 	
}


/*------------------------------------------------------------------------------------*/

Insert(intArr* arrPtr){
    int num;
	printf("Enter number to insert:\n");
	scanf("%d",&num);
    printf("%d\n",num);
	insertNum(arrPtr,num);
	printArr(*arrPtr);
	printf("\n\n");	
}


Remove(intArr* arrPtr){
	int num;
	printf("Enter number to remove:\n");
	scanf("%d",&num);
	removeNum(arrPtr,num);
	printArr(*arrPtr);
	printf("\n\n");	
}


Find(intArr arr){
	
}


/*------------------------------------------------------------------------------------*/

int* create(int n){ 
	int* adr = (int*)malloc(n*sizeof(int)); 
	
	if (adr == NULL){
		printf("create failed");
		exit(0);
	}
	
	return adr;
}

/*------------------------------------------------------------------------------------*/

void insertNum(intArr* arr, int num){
	if(arr->curIndex > arr->size){
	    arr->adr = (int*)realloc(arr->adr, (arr->size + EXPEND_VALUE)*sizeof(int));
		printf("realloc\n");
		if (arr->adr == NULL){
			printf("insert failed");
			exit(0);
		}else{
			arr->size = arr->size + EXPEND_VALUE;
		}		 
	}

	arr->adr[arr->curIndex] = num;
	(arr->curIndex)++;		
}

/*------------------------------------------------------------------------------------*/

int removeNum(intArr* arr , int num){
	int i = 0;
        int flag = 0;
	while(i < arr->curIndex){
		if(arr->adr[i] == num){
			flag = 1;	/*found*/
			break;
		}
		i++;
 	}
	if (!flag){
	    printf("not found\n");
		return -1;   /*not found*/
	}else{
		while(i < arr->curIndex){
			arr->adr[i] = arr->adr[i+1];
			i++;	
		}
		(arr->curIndex)--;
	}	 		
	
	return 0;
}

/*------------------------------------------------------------------------------------*/

void printArr(intArr arr){
	int i;	
	for(i=0;i<arr.curIndex;i++){
		printf("%d : %d  , ",i,arr.adr[i]);
	}
	printf("\n");
}
















