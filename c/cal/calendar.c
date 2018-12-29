#include<stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXPEND_VALUE 4
#define N 4



typedef struct meeting{
	int start;
	int end;
	char subject[50];
}meeting;


typedef struct calendar{
	meeting** adr;
	int curIndex;
	int size;	
}calendar;




void InsertApi(calendar* cal);
void FindApi(calendar cal);
void RemoveApi(calendar* cal);
void freeAllApi(calendar* cal1);
void printArrApi(calendar cal); 

static int Insert(calendar* cal , meeting* meet);
static int Find(calendar cal , meeting* meet);
static int Remove(calendar* cal , int startHour);

static meeting** create(int n);          
static void printArr(calendar cal); 
static void freeAll(calendar* cal);   
static int findIndexToInsert(calendar* cal , int start , int end);
static void shiftR(calendar* cal , int indexToInsert);


int main(int argc,char** argv)
{
	calendar cal1;	
     
    cal1.curIndex = 0;	    
	cal1.adr = create(N);	
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
	

    freeAll(&cal1);	
	
    return 0; 	
}

/*------------------------------------------------------------------------------------*/

static meeting** create(int n){ 
	meeting** adr = (meeting*)malloc(n*sizeof(meeting*)); 
	if (adr == NULL){
		printf("create failed\n");
		exit(0);
	}
	
	return adr;
}

/*------------------------------------------------------------------------------------*/

void InsertApi(calendar* cal){
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



static int Insert(calendar* cal , meeting* meet){
    
	int indexToInsert;	

	if(cal->curIndex > cal->size){
		printf("realloc\n");
	    cal->adr = (meeting**)realloc(cal->adr, (cal->size + EXPEND_VALUE)*sizeof(meeting*)); 
		if (cal->adr == NULL){
			/*printf("Insert failed\n");*/
			return -1;
		}else{
			cal->size = cal->size + EXPEND_VALUE;
		}
	}

		
	indexToInsert = findIndexToInsert(cal , meet->start , meet->end);
	if(indexToInsert==-1){
		/*printf("meeting overlap\n");*/
		return -1;
	}
	shiftR(cal , indexToInsert);
	

	cal->adr[indexToInsert] = meet; 
	(cal->curIndex)++;
	
	
	return 0;
}


/*------------------------------------------------------------------------------------*/


static int findIndexToInsert(calendar* cal , int start , int end){
	int toRet = 0;
	int i=0;
	int flag=0;
	
	if(cal->curIndex == 0){
		toRet = 0;
	}else{
		for(i=0;i<cal->curIndex;i++){
			if(end <= cal->adr[i]->start ){
				toRet = i;
				flag=1;	
				break;
			} 	
		}
	}
	
	if(!flag){
		toRet=cal->curIndex;
	}

	if(i>0){   /*check if overlap with the meeting before*/
		if(start<cal->adr[i-1]->end){
						toRet = -1;
		} 
	}

	return toRet;
}


static void shiftR(calendar* cal , int indexToInsert){
	int i;	
	for(i=cal->curIndex;i>indexToInsert;i--){
		cal->adr[i] = cal->adr[i-1];
	}
}


/*------------------------------------------------------------------------------------*/

void RemoveApi(calendar* calPtr){
	int startTimeToRemove;

	printf("Enter start time of meeting to remove : ");
	scanf("%d", &startTimeToRemove);


	if(Remove(calPtr , startTimeToRemove)==-1)
		printf("meeting wasnt found\n");
	else
		printArr(*calPtr);
}



static int Remove(calendar* cal , int startHour){
	int i=0;
	int toReturn=-1;
	while(i<cal->curIndex){
		if(cal->adr[i]->start > startHour){  /*if next meeting is later then the meeting we search -> search meeting is not at calendar*/ 
			return -1;		
		}else if(cal->adr[i]->start == startHour){
			free(cal->adr[i]);
			while(i<cal->curIndex-1){
				cal->adr[i] = cal->adr[i+1];
				i++;		
			}	
			(cal->curIndex)--;		
			toReturn=0;
		}
		i++;
	}
	return toReturn;
}



/*------------------------------------------------------------------------------------*/

void FindApi(calendar cal){
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



static int Find(calendar cal , meeting* meet){   /*meet pointer is updated with subject */
	int i=0;
	int toReturn=-1;
	while(i<cal.curIndex){
		if(cal.adr[i]->start > meet->start){ /*if next meeting is later then the meeting we search -> search meeting is not at calendar*/ 
			break;		
		}else if(cal.adr[i]->start == meet->start){
			strcpy(meet->subject,cal.adr[i]->subject);
			meet->end = cal.adr[i]->end;
			toReturn = 0;
			break;		
		}	
		i++;
	}
	return toReturn;
}

/*------------------------------------------------------------------------------------*/

void freeAllApi(calendar* cal1){	
	freeAll(cal1);
}


static void freeAll(calendar* cal1){
	int i = 0;
	
	if(cal1->curIndex == 0)
		return;

	for(i=0;i<cal1->curIndex;i++){
		free(cal1->adr[i]);
	}


	free(cal1->adr);
	cal1->curIndex = 0;	
	
}

/*------------------------------------------------------------------------------------*/

void printArrApi(calendar cal){
	printArr(cal);
}


static void printArr(calendar cal){
	int i;	
	for(i=0;i<cal.curIndex;i++){
		printf("%d : subject=> %s, start=> %d,  end=> %d  \n",i,cal.adr[i]->subject,cal.adr[i]->start,cal.adr[i]->end);
	}
	printf("\n\n");
}
















