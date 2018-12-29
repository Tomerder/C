#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "graph.h"


#define VEC_INIT_SIZE 100 
#define VEC_BLOCK_SIZE 100

#define NUM_OF_NODES 8
#define NUM_OF_ARCS 8

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


static Graph* SetGraph(Vector* _vec, int _directed, int _weighted, int* src, int* dst);
static int CheckArcs(Graph* _graph, int _directed, int _weighted, int* src, int* dst );
static int CheckRemove(Graph* _graph, int _directed, int _weighted, int* src, int* dst );


static void CheckBfs(int* _src, int* _dst);
static void CheckDfs(int* _src, int* _dst);
static int CheckBfsAll(int* _src, int* _dst);
static void CheckIsLoopExists(int* _src ,  int* _dst , int _isLoopExpected );

static ADTERR CheckDijkstra(int* _src ,  int* _dst );



char* resultArr[2] = {"FAIL","PASS"};

/*------------------------------------------------------------------------------------*/

int main(int argc,char** argv)
{
	int src[NUM_OF_ARCS] = {0,0,0,2,4,3,3,7};
	int dst[NUM_OF_ARCS] = {1,2,3,4,7,5,6,0};
	int src2[NUM_OF_ARCS] = {0,0,0,2,4,3,3,4};
	int dst2[NUM_OF_ARCS] = {1,2,3,4,7,5,6,5};

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
	
	printf("/*---------------------------------------------*/\n");
	CheckDfs(src , dst);
	
	printf("/*---------------------------------------------*/\n");
	CheckBfsAll(src , dst);
	
	printf("/*---------------------------------------------*/\n");
	CheckIsLoopExists(src , dst , TRUE);
	CheckIsLoopExists(src2 , dst2 , FALSE);
	
	printf("/*---------------------------------------------*/\n");
	CheckDijkstra(src2 , dst2 );



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


static void Check3(int* _src, int* _dst){
	Graph* graph;
	Vector* vec;
	int err , resultIndex = PASS;
	int oldWeight;

	printf("Check 3: ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, TRUE, _src, _dst);   /*setup*/
		
	/*change weight*/	
	err = GraphChangeWT(graph , _src[0] , _dst[0], _src[0] , &oldWeight);
	RETURN_ERR;

	/*functionality check*/
	if(oldWeight != _dst[0]){
		resultIndex = 0;
	}
	
	if(GraphGetWT(graph , _src[0] , _dst[0]) != _src[0]){
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

static void CheckBfs(int* _src, int* _dst){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS;

	printf("Check BFS : ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, FALSE, _src, _dst);   /*setup*/
		

	/*check BFS*/
	if (IsReachableDFS(graph, 0 , 0 ) != TRUE ){  /*check loop*/
		resultIndex = FAIL;
	}
	if (IsReachableBFS(graph, 0 , 7 ) != TRUE ){
		resultIndex = FAIL;
	}
	if (IsReachableBFS(graph, 3 , 5 ) != TRUE ){
		resultIndex = FAIL;
	}

	if (IsReachableBFS(graph, 1 , 4 ) == TRUE ){
		resultIndex = FAIL;
	}
	/*
	if (IsReachableBFS(graph, 4 , 0 ) == TRUE ){
		resultIndex = FAIL;
	}
	*/

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	

}

/*------------------------------------------------------------------------------------*/

static void CheckDfs(int* _src, int* _dst){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS;

	printf("Check DFS : ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, FALSE, _src, _dst);   /*setup*/
		

	/*check DFS*/
	if (IsReachableDFS(graph, 0 , 0 ) != TRUE ){  /*check loop*/
		resultIndex = FAIL;
	}
	
	if (IsReachableDFS(graph, 0 , 7 ) != TRUE ){
		resultIndex = FAIL;
	}
	if (IsReachableDFS(graph, 3 , 5 ) != TRUE ){
		resultIndex = FAIL;
	}
	
	if (IsReachableDFS(graph, 1 , 4 ) == TRUE ){
		resultIndex = FAIL;
	}
	/*
	if (IsReachableDFS(graph, 4 , 0 ) == TRUE ){
		resultIndex = FAIL;
	}
	*/

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	

}

/*------------------------------------------------------------------------------------*/

static int CheckBfsAll(int* _src, int* _dst){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS, numOfNodes, i, j;
	int* distancesArr;
	int* pathArr;
	int srcIndex ;
	

	printf("Check AllPathsFromSrcBFS :\n");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, FALSE, _src, _dst);   /*setup*/
		
	numOfNodes = VectorItemsNum(vec);
	
	distancesArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(distancesArr == NULL){
		return ALLOC_FAILED;
	}
	
	pathArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(pathArr == NULL){
		return ALLOC_FAILED;
	}


	/*check AllPathsFromSrcBFS*/
	srcIndex = 0;
	AllPathsFromSrcBFS(graph, srcIndex , distancesArr , pathArr ) ;

	/*print*/	
	for(i=0; i<numOfNodes; i++){
		printf("node %d: distance=%d : ",i,distancesArr[i]);
		if(distancesArr[i] > 0){
			printf("%d<=",i);
			j = pathArr[i]; /*get father*/
			while(j != srcIndex){
				printf("%d<=",j);
				j = pathArr[j]; /*next father*/
				if(j<0){
					break;
				}
			}
			printf("%d\n",srcIndex);	
		}else{
			printf("\n");	
		}
	}
	

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	
	
	return OK;

}

/*------------------------------------------------------------------------------------*/

static void CheckIsLoopExists(int* _src ,  int* _dst , int _isLoopExpected){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS;

	printf("Check IsLoopExists : ");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, FALSE, _src, _dst);   /*setup*/
		

	/*check IsLoopExists*/
	if (IsLoopExists(graph) != _isLoopExpected ){ 
		resultIndex = FAIL;
	}

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	

}

/*------------------------------------------------------------------------------------*/


static ADTERR CheckDijkstra(int* _src ,  int* _dst ){
	Graph* graph;
	Vector* vec;
	int resultIndex = PASS, numOfNodes, i;
	int* distancesArr;
	int* pathArr; 
	int* indexesArr; 

	printf("Check Dijkstra :\n");
	
	vec = VectorInit();    
	graph = SetGraph(vec, TRUE, TRUE, _src, _dst);   /*setup*/
		
	numOfNodes = VectorItemsNum(vec);	
		
	distancesArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(distancesArr == NULL){
		return ALLOC_FAILED;
	}
	
	pathArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(pathArr == NULL){
		return ALLOC_FAILED;
	}	
	
	indexesArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(indexesArr == NULL){
		return ALLOC_FAILED;
	}	
		
	Dijkstra(graph , 0, indexesArr , distancesArr, pathArr);
	
	
	for(i=0; i < numOfNodes; i++ ){
		printf("index:%d  minDist:%d  father:%d\n", indexesArr[i] , distancesArr[i] , pathArr[i]);		
	}
	

	printf("%s\n", resultArr[resultIndex]);	 

	VectorDestroy(vec);                             /*cleanup*/
	GraphDestroy(graph); 	
	free(distancesArr);
	free(pathArr);

	return OK;
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

