#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calendar2.h"


static void InsertApi(calendar* cal);
static void FindApi(calendar cal);
static void RemoveApi(calendar* cal);
static void freeAllApi(calendar* cal1);
static void printArrApi(calendar cal); 
static meeting** createApi(int n);


int main(int argc,char** argv)
{
	calendar cal1;	
     
    cal1.curIndex = 0;	    
	if((cal1.adr = createApi(N))==NULL){
		printf("Allocation failed\n");
		exit(0);
	}	
    cal1.size = N;  
	
	int cont = 1;			
	unsigned int option;
	
	while (cont) {
		printf("Choose option: \n");
		printf("1: Insert  \n");
		printf("2: Find    \n");
		printf("3: Remove  \n");
		printf("4: Print  \n");
		printf("Any another number - stop \n");

		scanf("%d", &option);
		switch (option) {
			case 1:
				InsertApi(&cal1);	break;
			case 2:
				FindApi(cal1);		break;
			case 3:
				RemoveApi(&cal1);	break;
			case 4:
				printArrApi(cal1);  break;

			default: cont = 0;	break;
		}
	}
	

    freeAllApi(&cal1);	
	
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

static meeting** createApi(int n){
	return create(n);
}

/*------------------------------------------------------------------------------------*/

static void InsertApi(calendar* cal){
	int start,end;
	char subject[50];  
	meeting* meetPtr;

	printf("Enter subject : ");
	scanf("%s", &subject);
	printf("Enter start time : ");
	scanf("%d",&start);
    printf("Enter end time : ");
	scanf("%d",&end);

	if(start > end){
		printf("input errrorrrrrrr\n");
		return;
	}

	meetPtr = (meeting*)malloc(sizeof(meeting));

	if(meetPtr == NULL){
		printf("failed\n");
		return;
	}

	strcpy(meetPtr->subject,subject);
	meetPtr->start = start;
	meetPtr->end = end;

	if(Insert(cal , meetPtr)==-1){
		printf("insert failed\n\n");	
	}else{
		printArr(*cal);
	}
}


/*------------------------------------------------------------------------------------*/

static void RemoveApi(calendar* calPtr){
	int startTimeToRemove;

	printf("Enter start time of meeting to remove : ");
	scanf("%d", &startTimeToRemove);


	if(Remove(calPtr , startTimeToRemove)==-1)
		printf("meeting wasnt found\n\n");
	else
		printArr(*calPtr);
}



/*------------------------------------------------------------------------------------*/

static void FindApi(calendar cal){
	int startTimeToFind;

	meeting meet;
		

	printf("Enter start time of meeting to find : ");
	scanf("%d", &startTimeToFind);

	meet.start = startTimeToFind;

	if(Find(cal , &meet)==-1)
		printf("meeting wasnt found\n\n");
	else{
		printf("Subject: %s , Start: %d , End: %d \n\n",meet.subject,meet.start,meet.end);
	}
}



/*------------------------------------------------------------------------------------*/

static void freeAllApi(calendar* cal1){	
	freeAll(cal1);
}


/*------------------------------------------------------------------------------------*/

static void printArrApi(calendar cal){
	printArr(cal);
}















