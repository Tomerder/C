/*-----------------------------------------Tomer Dery-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SIZE 10

/*------------------------------------------------------------------------------------*/

typedef struct Node Node;

struct Node{
	void* m_data;
	Node* m_prev;
	Node* m_next;
};


/*------------------------------------------------------------------------------------*/

void dummy(void){  printf("ABC\n");     } 

typedef int(*Func)();

/*------------------------------------------------------------------------------------*/

void bug1(void);
void bug2(void);
void bug2B(void);
void bug3(void);
void bug4(void);
void bug5(void);
void bug6(void);
void bug7(void);
void bug8(void);
void bug9(void);
void bug10(void);
void bug11(void* _mainAdr);
int bug12(int _par);
int bug13(int _i);
int bug14(int _i);
int writeOverPc();

/*------------------------------------------------------------------------------------*/


int main()
{
	int retVal = 0;
	
	//bug1();             /*access pointer after free*/                                						    /*detected by valgrind*/
    //bug2();      		  /*writing AFTER memory allocated*/                            /* NOT stop at line */  /*detected by valgrind*/
    //bug2B();    		  /*writing AFTER memory allocated*/                            /* NOT stop at line */  /*detected by valgrind*/
    //bug3();       	  /*writing BEFORE memory allocated */     	         		    /* stop at line */      /*detected by valgrind*/
    //bug4();     		  /*writing to RO memory*/                 	          		    /* stop at line */      /*detected by valgrind*/
    //bug5();     		  /*writing to unalocated memory*/         	           		    /* stop at line */      /*detected by valgrind*/
    //bug6();     		  /*writing to address 1*/                             			/* stop at line */      /*detected by valgrind*/
    //bug7();      		  /*writing to unvalid address on STACK*/      		 /* NOT stop at line - show func */ /*valgrind - stack smashing*/
    //bug8();      		  /*writing to const Static Int using pointer - RO memory*/     /* NOT stop at line */    /*detected by valgrind*/
    //bug9();      		  /*free unknown location (garbage) */                       /* NOT stop at line - show func*/ /*detected by valgrind*/
    //bug10();            /*realloc unknown location (garbage) - wasnt allocated*/      /* NOT stop at line */         /*detected by valgrind*/
    
    /*------------------------------------------corupt stack--------------------------------------*/
    
    //Func func = main;
    //bug11(func);        /*corupt main address - memset over main address */           /* NOT stop at line */      /*detected by valgrind*/
   
    //retVal = bug12(5);  /*corupt stack - memset after parameter address*/                /* NOT stop at line */     /*------------------------*/
    
	//retVal = bug13(1); /*corupt pc - main address - memset after local par address*//*NOT stop at line*/ /*valgrind - jump to invalid adr*/      
    
    //retVal = bug14(1);          /*corupt pc - address to return from func */                                     /*------------------------*/
    
    //retVal = writeOverPc(); /*corupt pc - address to return from func - with the address of writeOverPc() */     /*------------------------*/
    
    
    
    printf("main end : %d\n", retVal);                                                                     
    
    return 0;
      
}



/*------------------------------------------------------------------------------------*/

void bug1(void){
	Node* node;
	Node* node1;
	Node* node2;
	
	node1 = malloc ( sizeof(Node) ) ;	 
	node2 = malloc ( sizeof(Node) ) ; 

	node1->m_next = node2;
	node2->m_next = NULL;
	
	node = node1;
	while(node != NULL){
		free(node);
		node = node->m_next;   /*node does not exist*/
	}
	
}

/*------------------------------------------------------------------------------------*/

void bug2(void){
	int* intArr;
	int* intArr2;
	int i;
	
	intArr = malloc ( sizeof(int) * SIZE) ;	
	
	for(i=0; i < SIZE *2 ; i++){    /*writing to address AFTER alocated chank of memory*/
		intArr[i] = i;
	}
	
	intArr2 = malloc ( sizeof(int) * SIZE) ;  /*crash at next malloc*/
}


/*------------------------------------------------------------------------------------*/

void bug2B(void){
	int* intArr;
	int i;
	
	intArr = malloc ( sizeof(int) * SIZE) ;	
	
    intArr[SIZE+1] = 7;   /*writing to address AFTER alocated chank of memory  - malloc meta data override*/
	
	free(intArr);
}


/*------------------------------------------------------------------------------------*/

void bug3(void){
	int* ip1;
	int* ip2;
	
	ip1 = malloc ( sizeof(int)) ;
		
	ip2 = ip1 - (sizeof(int)); 
	
	*ip2 = 5;   /*writing to address BEFORE alocated chank of memory*/

}

/*------------------------------------------------------------------------------------*/

void bug4(void){
	char* str = "ABC";
	
	str[0] = 't';   /*writing to RO memory*/

}

/*------------------------------------------------------------------------------------*/

void bug5(void){
	int* ip;
	
	*ip = 2;  /*writing to unalocated memory*/

}

/*------------------------------------------------------------------------------------*/

void bug6(void){
	int* ip;
	
	ip = (int*)1;
	
	*ip = 5;   /*writing to address 1*/

}


/*------------------------------------------------------------------------------------*/

void bug7(void){
	char str[SIZE];
	
	str[SIZE + 1] = 'x';   /*writing to unvalid address on STACK*/ 
	

}


/*------------------------------------------------------------------------------------*/

void bug8(void){
	const static int constStaticInt = 7;
	int* ip = &constStaticInt;

	*ip = 5;  /*writing to const Static Int using pointer - RO memory*/

}

/*------------------------------------------------------------------------------------*/

void bug9(void){
	
	int *ip;
	free(ip);    /*free unknown location ( garbage) */
}


/*------------------------------------------------------------------------------------*/

void bug10(void){
	
	int *ip;
	realloc ( ip , SIZE);    /*realloc unknown location (garbage) - wasnt allocated*/
}






/************************************************************************************************************************/
/*stack coruptions*/




/*------------------------------------------------------------------------------------*/

void bug11(void* _mainAdr){
	size_t sizeToOverride = 1;
	void* adr = _mainAdr;
	 
    printf("bug11 begin\n"); 
	                  
	memset(adr, 1, sizeToOverride) ;     /*override adress of main - pc*/
	
	printf("bug11 end\n");
	
}                          

/*------------------------------------------------------------------------------------*/

int bug12(int _par){
	size_t sizeToOverride = 1000;
	
	printf("bug12 begin\n"); 
	
	memset(&_par,1,sizeToOverride);              /*override stack*/
	
	printf("bug12 end\n");
	
	return 777;
}                     


/*------------------------------------------------------------------------------------*/

int bug13(int _i){
	int loc;
	//size_t sizeToOverride = 1;
	
	printf("bug13 begin\n"); 
	
	memset((&loc)+4,1,1);              /*override stack*/
	
	printf("bug13 end\n");
	
	return 777;
}                     

/*------------------------------------------------------------------------------------*/

int bug14(int _i){
	int* loc = (int*)bug14;
	//size_t sizeToOverride = 1;
	
	printf("bug14 begin\n"); 
	
	memset((&loc) + 4, (int)loc ,1);              /*override stack adress of main - pc*/
	
	printf("bug14 end\n");
	
	return 777;
}                     


/*------------------------------------------------------------------------------------*/


int f_robert(int byts, void* _main)
{
	int some = 1;

	printf("%p\n",&some);

	memset((&some)+4,(int)_main,byts);
	
	return some;
}


int writeOverPc()
{
	int var=0;
	printf("%p\n",f_robert);

	var = f_robert(1,writeOverPc);

	printf("%d\n",var);

	return var;
}


/*------------------------------------------------------------------------------------*/

	
