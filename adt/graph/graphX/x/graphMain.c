#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graph.h"


#define VEC_INIT_SIZE 100 
#define VEC_BLOCK_SIZE 100

#define NUM_OF_NODES 8
#define NUM_OF_ARCS 7

/*
#define DIRECTED_GRAPH 1
#define WEIGHTED_GRAPH 0
*/

typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

#define RETURN_ERR if(err!=0) printf("FAIL:%d",err)


typedef enum {FAIL,PASS} Result;


#define DEBUG 0


static Vector* VectorInit();
static Graph* GraphInit(Vector* _vec, int _directed, int _weighted);

static void SetBitOn(int* _mode, int _bitNum);
static void SetWeighted(int* _mode);
static void SetDirected(int* _mode);


static void Check1(int _directed, int _weighted, int* src, int* dst);
static void Check2(int _directed, int _weighted, int* src, int* dst);
static void Check3(int* src, int* dst);

static void CheckBfs(int* src, int* dst);

static Graph* SetGraph(Vector* _vec, int _directed, int _weighted, int* src, int* dst);
static int CheckArcs(Graph* _graph, int _directed, int _weighted, int* src, int* dst );
static int CheckRemove(Graph* _graph, int _directed, int _weighted, int* src, int* dst );


char* resultArr[2] = {"FAIL","PASS"};

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int src[NUM_OF_ARCS] = {0,0,0,2,4,3,3};
	int dst[NUM_OF_ARCS] = {1,2,3,4,7,5,6};
	int directed = 0 , weighted = 0;
	
	printf("/*---------------------------------------------*/\n");
	printf("not directed , not weighted : \n");
	Check1(directed, weighted , src , dst );	/*insert*/
	Check2(directed, weighted , src , dst );	/*remove*/	
	
	directed = 1;
	printf("/*---------------------------------------------*/\n");
	printf("directed , not weighted : \n");
	Check1(directed, weighted, src , dst);	/*insert*/
	Check2(directed, weighted, src , dst);	/*remove*/	
	
	directed = 0;
	weighted = 1;
	printf("/*---------------------------------------------*/\n");
	printf("not directed , weighted : \n");
	Check1(directed, weighted, src , dst);	/*insert*/
	Check2(directed, weighted, src , dst);	/*remove*/	
	
	directed = 1;
	weighted = 1;
	printf("/*---------------------------------------------*/\n");
	printf("directed , weighted : \n");
	Check1(directed, weighted, src , dst);	/*insert*/
	Check2(directed, weighted, src , dst);	/*remove*/	
	
	printf("/*---------------------------------------------*/\n");
	printf("change weight : \n");
	Check3(src , dst);	             /*change weight*/	

	printf("/*---------------------------------------------*/\n");
	CheckBfs(src , dst);

    return 0; 	

    
}




/*------------------------------------------------------------------------------------*/	


static void Check1(int _directed, int _weighted, int* src, int* dst){
	Graph* graph;
	Vector* vec;
	
	int resultIndex;
	
	vec = VectorInit();    
	graph = SetGraph(vec, _directed, _weighted, src, dst);
	
	printf("Check 1 : ");
	
	/*functionality check*/
	resultIndex = CheckArcs(graph, _directed, _weighted, src, dst);
	
	printf("%s\n", resultArr[resultIndex]);	 	
		
	if(DEBUG){		
		GraphPrint(graph);
		printf("/----------------------------------------------------\n");
	}
		
	
	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 
}


/*------------------------------------------------------------------------------------*/	


static void Check2(int _directed, int _weighted, int* src, int* dst){
	Graph* graph;
	Vector* vec;
	int err , resultIndex = PASS;
	int oldWeight;
	
	printf("Check 2 : ");

	/*setup*/	
	vec = VectorInit();    
	graph = SetGraph(vec, _directed, _weighted, src, dst);
	
	/*remove*/
	if(!_weighted){
		err = GraphRemove(graph , src[0] , dst[0]) ;
		RETURN_ERR;  
	}else{
		err = GraphRemoveWT(graph , src[0] , dst[0] , &oldWeight ) ;
		RETURN_ERR;                                 
	}
	
	/*functionality check*/
	resultIndex = CheckRemove(graph, _directed, _weighted, src, dst);
	
	
	printf("%s\n", resultArr[resultIndex]);	 	
		
	if(DEBUG){		
		GraphPrint(graph);
		printf("/----------------------------------------------------\n");
	}
	
	
	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 

}



/*------------------------------------------------------------------------------------*/


static void Check3(int* src, int* dst){
	Graph* graph;
	Vector* vec;
	int err , resultIndex = PASS;
	int oldWeight;

	printf("Check 3: ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, TRUE, src, dst);   /*setup*/
		
	/*change weight*/	
	err = GraphChangeWT(graph , src[0] , dst[0], src[0] , &oldWeight);
	RETURN_ERR;

	/*functionality check*/
	if(oldWeight != dst[0]){
		resultIndex = 0;
	}
	
	if(GraphGetWT(graph , src[0] , dst[0]) != src[0]){
		resultIndex = 0;
	}
	
	printf("%s\n", resultArr[resultIndex]);	 	
		
	if(DEBUG){		
		GraphPrint(graph);
		printf("/----------------------------------------------------\n");
	}

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 
	

}

/*------------------------------------------------------------------------------------*/

static void CheckBfs(int* src, int* dst){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS;

	printf("Check BFS : ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, FALSE, src, dst);   /*setup*/
		

	/*check BFS*/
	if (IsReachableBFS(graph, 0 , 7 ) != TRUE ){
		resultIndex = FAIL;
	}
	if (IsReachableBFS(graph, 3 , 5 ) != TRUE ){
		resultIndex = FAIL;
	}

	if (IsReachableBFS(graph, 1 , 4 ) == TRUE ){
		resultIndex = FAIL;
	}
	if (IsReachableBFS(graph, 4 , 0 ) == TRUE ){
		resultIndex = FAIL;
	}

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	

}



/*------------------------------------------------------------------------------------*/

static Graph* GraphInit(Vector* _vec, int _directed, int _weighted ){
	Graph* graph;
	
	int graphMode = 0;
	if(_directed){
		SetDirected(&graphMode);
	}
	if(_weighted){
		SetWeighted(&graphMode);
	}
		                 
	graph = GraphCreate (_vec, graphMode);
	
	return graph;

}


static Vector* VectorInit(){
	int i=0,err=0;
	char* cities[NUM_OF_NODES] = {"node0" , "node1" , "node2" , "node3" , "node4", "node5", "node6", "node7"};
	
	Vector* vec = VectorCreate(VEC_INIT_SIZE, VEC_BLOCK_SIZE);
	
	while(i < NUM_OF_NODES){
		if((err=VectorInsert(vec,(void*)(&cities[i])))!=0){  
			printf("error no. %d\n",err);
		}
		i++;
	}
	return vec;
}





/*------------------------------------------------------------------------------------*/

static void SetDirected(int* _mode){
	SetBitOn(_mode, DIRECTED_BIT);

}

static void SetWeighted(int* _mode){
	SetBitOn(_mode, WEIGHTED_BIT);

}

static void SetBitOn(int* _mode, int _bitNum){
	int bitWise = TRUE;
	
	bitWise <<= _bitNum;
	
	*_mode = *_mode | bitWise;

}



/*------------------------------------------------------------------------------------*/
static Graph* SetGraph(Vector* _vec, int _directed, int _weighted, int* src, int* dst){
	int i, err;
	Graph* graph;

	_vec = VectorInit();    
	graph = GraphInit(_vec, _directed, _weighted );   /*setup*/
	
	for(i = 0 ; i < NUM_OF_ARCS ; i++){
		if(!_weighted){
			err = GraphJoin(graph , src[i] , dst[i]);
		}else{
			err = GraphJoinWT(graph , src[i] , dst[i], dst[i] );
		}	
		RETURN_ERR;                                /*output check*/
	}

	return graph;
}


static int CheckArcs(Graph* _graph, int _directed, int _weighted, int* src, int* dst ){
	int i , resultIndex = PASS;

	for(i = 0 ; i < NUM_OF_ARCS ; i++){
		if(!_weighted){
			if(!GraphIsAdjacent(_graph , src[i] , dst[i])) {
				resultIndex = FAIL;
			}	
		}else{
			if(GraphGetWT(_graph , src[i] , dst[i]) != dst[i]) {
				resultIndex = FAIL;
			}	
		}
		
		if( !_directed){  /*if not directed - check mirror as well)*/
			if(!_weighted){
				if(!GraphIsAdjacent(_graph , dst[i] , src[i])) {
					resultIndex = FAIL;
				}	
			}else{
				if(GraphGetWT(_graph , dst[i] , src[i]) != dst[i]) {
					resultIndex = FAIL;
				}	
			}
		}
	}
	
	
	return resultIndex;

}


static int CheckRemove(Graph* _graph, int _directed, int _weighted, int* src, int* dst ){
	int resultIndex = PASS;

	if(!_weighted){
		if(GraphIsAdjacent(_graph , src[0] , dst[0])) {
			resultIndex = 0;
		}	
		if(!_directed){
			if(GraphIsAdjacent(_graph , dst[0] , src[0])) {
				resultIndex = 0;
			}	
		}
	}else{
		if(GraphGetWT(_graph , src[0] , dst[0]) != FALSE) {
			resultIndex = 0;
		}	
		if(!_directed){
			if(GraphGetWT(_graph , dst[0] , src[0]) != FALSE ) {
				resultIndex = 0;
			}	
		}
	}

	return resultIndex;

}
