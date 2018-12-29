#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "graph.h"
#include "queue.h"
#include "heapD.h"


#define RETURN_ERR if(err!=0) return err


typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

/*--------------------------------------------------------------------------------*/

struct Graph{
	Vector*  	  m_nodes;	
    int*     	  m_arcs;
    unsigned int  m_graphMode;      /*bit 0 : DIRECTED_BIT , bit 1 : WEIGHTED_BIT */
};


typedef struct DijstraStruct{
	size_t s_index;
	int s_minDistance;
	int s_hasBeenSeen;
	size_t s_fatherAtPath;
}DijstraStruct;


/*--------------------------------------------------------------------------------*/

static int IsDirected(Graph* _graph);
static int IsWeighted(Graph* _graph);
static int IsBitOn(unsigned int _graphMode, int _bitNum);

static ADTERR GraphJoinCall (Graph* _graph, size_t _src, size_t _dst , int _weight , int* _oldweight);
/*static ADTERR GraphRemoveCall (Graph* _graph, size_t _src, size_t _dst , int* _oldWeight );*/

static ADTERR CheckIndexes(Graph* _graph, size_t _src, size_t _dst);

static int IsReachableDFSrec(Graph* _graph, size_t _src, size_t _dst, int* hasBeenSeenArr, int* _isfirstRoundflag);


static int WeekFuncDijstra (void* item1, void* item2);
/*static void PrintFuncDijstra(Vector* _vec);*/
/*static void findDijstraStructByIndex(Vector* _vec, int _graphIndex ,  DijstraStruct** _retDijstraStruct,  int* _retHeapIndex);*/
static void Relax(Heap* _heap, DijstraStruct* _fatherDijstraStruct , int _sonIndex , int _arcWeight);

/*--------------------------------------------------------------------------------*/

Graph* GraphCreate (Vector* _vec, unsigned int _graphMode){
	Graph* graph;
	int numOfNodes; 
	
	
	assert(_vec);
	
	graph = (Graph*) malloc (sizeof(Graph)) ;
	if ( graph == NULL ) {
		return NULL;
	}
	
	graph->m_nodes = _vec;
	numOfNodes = VectorItemsNum(_vec);
	
	/* graph->m_arcs will be cast to int*   :  [i][j] = (i * numOfitems) + j */
	graph->m_arcs =  calloc ( numOfNodes * numOfNodes , sizeof(int) ); 
	if (graph->m_arcs == NULL){
		free(graph);
		return NULL;
	}
	
	/*
	graph->m_arcs = (int**) malloc ( sizeof(int*) * numOfNodes); 
	if (graph->m_arcs == NULL){
		free(graph);
		return NULL;
	}
	
	
	for(i=0; i<numOfNodes; i++){
		graph->m_arcs[i] = (int*) calloc (numOfNodes, sizeof(int) * numOfNodes); 
		if(graph->m_arcs[i] == NULL){
			free(graph);
			return NULL;
		}
	}
*/
	
	graph->m_graphMode = _graphMode; 
	
	return graph;	

}

/*--------------------------------------------------------------------------------*/

void GraphDestroy (Graph* _graph){
	assert(_graph);
	
	free(_graph->m_arcs);
	free(_graph);

}

/*--------------------------------------------------------------------------------*/


static ADTERR GraphJoinCall (Graph* _graph, size_t _src, size_t _dst , int _weight , int* _oldweight){
	int err = 0;
	int* arcs;
	int numOfNodes;
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);

	err = CheckIndexes(_graph, _src, _dst);
	RETURN_ERR;
	
	arcs = _graph->m_arcs ; 
	
	if(_oldweight){   /*return old weight*/
		*_oldweight = arcs[_src * numOfNodes +  _dst];
	} 	
	
	arcs[_src * numOfNodes + _dst] = _weight;
	
	if ( !IsDirected(_graph) ){
		arcs[_dst * numOfNodes + _src] = _weight;
	}
	
	return OK;


}


ADTERR GraphJoin (Graph* _graph, size_t _src, size_t _dst){
	assert(_graph);
	
	if(IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphJoinCall(_graph, _src, _dst, TRUE, NULL);

}


ADTERR GraphJoinWT (Graph* _graph, size_t _src, size_t _dst, int _weight ){
	
	assert(_graph);
	
	
	if(!IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphJoinCall(_graph, _src, _dst, _weight, NULL);


}


/*--------------------------------------------------------------------------------*/


ADTERR GraphRemove (Graph* _graph, size_t _src, size_t _dst){
	assert(_graph);
	
	if(IsWeighted(_graph)){
		return WRONG_API;
	}
	
	/*return GraphRemoveCall(_graph, _src, _dst, NULL);*/
	return GraphJoinCall(_graph, _src, _dst, FALSE, NULL);

}


ADTERR GraphRemoveWT (Graph* _graph, size_t _src, size_t _dst, int* _oldWeight ){
	assert(_graph);
	
	if(!IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphJoinCall(_graph, _src, _dst, FALSE, _oldWeight);

}



/*--------------------------------------------------------------------------------*/



ADTERR GraphChangeWT (Graph* _graph, size_t _src, size_t _dst, int _newWeight , int* _oldWeight  ){
	assert(_graph);
	
	if(!IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphJoinCall(_graph, _src, _dst, _newWeight, _oldWeight);

}


/*--------------------------------------------------------------------------------*/

static int GraphGetWeightCall (Graph* _graph, size_t _src, size_t _dst){
	int arcWeight, err;
	int numOfNodes;
	int* arcs;
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);	

	arcs = _graph->m_arcs ; 

	err = CheckIndexes(_graph, _src, _dst);
	if (err!=0){
		return 0;
	}
	
	arcWeight = arcs[_src * numOfNodes + _dst];
	
	return arcWeight;
}


int GraphGetWT (Graph* _graph, size_t _src, size_t _dst){
	if(!IsWeighted(_graph)){
		return WRONG_API;
	}

	return GraphGetWeightCall (_graph, _src, _dst);
}


int GraphIsAdjacent (Graph* _graph, size_t _src, size_t _dst){
	int toRet = FALSE;
	
	if ( GraphGetWeightCall (_graph, _src, _dst) != 0){
		toRet = TRUE;
	}
	
	return toRet;
}

/*--------------------------------------------------------------------------------*/

static ADTERR CheckIndexes(Graph* _graph, size_t _src, size_t _dst){
	int numOfNodes;
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);
	if ( _src >= numOfNodes || _dst >= numOfNodes ){
		return OVERFLOW;
	} 
	
	return OK;
}



/*--------------------------------------------------------------------------------*/

static int IsBitOn(unsigned int _graphMode, int _bitNum){
	int bitWise = 1;
	
	_graphMode >>= _bitNum;
	
	return (_graphMode & bitWise);
	 	
}


static int IsDirected(Graph* _graph){
	unsigned int graphMode;
	graphMode = _graph->m_graphMode;
		
	return IsBitOn(graphMode, DIRECTED_BIT) ;
	 	
}


static int IsWeighted(Graph* _graph){
	unsigned int graphMode;
	graphMode = _graph->m_graphMode;
		
	return IsBitOn(graphMode, WEIGHTED_BIT) ;
	 	
}


/*--------------------------------------------------------------------------------*/



void GraphPrint(Graph* _graph){
	int numOfNodes, i=0 , j=0 , weight;
	int* arcs;

	numOfNodes = VectorItemsNum(_graph->m_nodes);
	arcs = _graph->m_arcs ; 	

	while(i < numOfNodes){
		printf("node : %d =>" , i);
		for(j=0; j < numOfNodes; j++){
			weight = arcs[i * numOfNodes + j];
			if( weight > 0){
				printf("%d(%d) ", j , weight);
			}
		}
		printf("\n");
		i++;
	}	
	 	
}

/*--------------------------------------------------------------------------------*/

int IsReachableBFS(Graph* _graph, size_t _src, size_t _dst){
	Queue* queue;
	int numOfNodes, i, err;
	int* hasBeenSeenArr;
	int* indexesArr;
	void* popItem; 
	int* arcs;
	int toret = FALSE;

	assert(_graph);
	
	err = CheckIndexes(_graph, _src, _dst);
	if (err != OK){
		return -1;
	}

	numOfNodes = VectorItemsNum(_graph->m_nodes);

	arcs = _graph->m_arcs ; 
	
	/*arr of indexes to insert to queue*/
	indexesArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(indexesArr == NULL){
		return -1;
	}
	
	for(i=0; i<numOfNodes; i++){
		indexesArr[i] = i;
	}
	
	hasBeenSeenArr = (int*) calloc (sizeof(int) , numOfNodes); 
	if(hasBeenSeenArr == NULL){
		return -1;
	}

	queue = QueueCreate (numOfNodes);

	QueueInsert(queue, &_src);

	while( !IsQueueEmpty(queue) ){
		QueueRemove(queue, &popItem);
		if ( *(int*)popItem == _dst){
			toret = TRUE;
			break;
		}

		for(i=0; i<numOfNodes; i++){
			if(arcs[ (*(int*)popItem) * numOfNodes + i] != 0){   /*check if i is son of popItem*/
				if(!hasBeenSeenArr[i] ){
					hasBeenSeenArr[i] = TRUE;					
					QueueInsert(queue, &indexesArr[i] );
				}	
			}
		}

	}

	free(hasBeenSeenArr);
	free(indexesArr);
	QueueDestroy(queue);
	
	return toret;

}


/*--------------------------------------------------------------------------------*/


int IsReachableDFS(Graph* _graph, size_t _src, size_t _dst){
	int* hasBeenSeenArr;
	int toRet = FALSE , numOfNodes, err , isfirstRoundflag = TRUE;
	
	assert(_graph);
	
	err = CheckIndexes(_graph, _src, _dst);
	if (err != OK){
		return -1;
	}
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);
	hasBeenSeenArr = (int*) calloc (sizeof(int) , numOfNodes); 
	
	toRet = IsReachableDFSrec(_graph, _src, _dst, hasBeenSeenArr, &isfirstRoundflag);    /*recursive func*/
	
	free(hasBeenSeenArr);
	
	return toRet;
}



static int IsReachableDFSrec(Graph* _graph, size_t _src, size_t _dst, int* hasBeenSeenArr, int* _isfirstRoundflag){
	int numOfNodes, i , sonIndex;
	
	if(_src == _dst && (!*_isfirstRoundflag) ){
		return TRUE;
	}

	*_isfirstRoundflag = FALSE;	

	numOfNodes = VectorItemsNum(_graph->m_nodes);
	
	for(i=0; i<numOfNodes; i++){
		sonIndex = (_src * numOfNodes) + i;
		if(_graph->m_arcs[sonIndex] != 0){   /*for all sons*/
			if(!hasBeenSeenArr[i]){
				hasBeenSeenArr[i] = TRUE;
				if (IsReachableDFSrec(_graph, i, _dst, hasBeenSeenArr, _isfirstRoundflag)){   /*recursion*/
					return TRUE;
				}
			}
		}
	}
	
	return FALSE; 

}

/*--------------------------------------------------------------------------------*/


ADTERR AllPathsFromSrcBFS(Graph* _graph, size_t _src, int* _distancesArr, int* _pathArr ){
	Queue* queue;
	int numOfNodes, i, sonIndex;
	int* hasBeenSeenArr;
	int* indexesArr;
	int* popItem;
	int* arcs; 

	assert(_graph);
	assert(_distancesArr);
	assert(_pathArr);
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);
	
	hasBeenSeenArr = (int*) calloc (numOfNodes , sizeof(int)); 
	if(hasBeenSeenArr == NULL){
		return ALLOC_FAILED;
	}

	indexesArr = (int*) calloc (numOfNodes , sizeof(int)); 
	if(indexesArr == NULL){
		return ALLOC_FAILED;
	}
	
	arcs = _graph->m_arcs;
	
	for(i=0; i<numOfNodes; i++){
		indexesArr[i] = i;
	}
	
	/*initalization*/
	for(i=0; i<numOfNodes; i++){
		hasBeenSeenArr[i] = FALSE;
		_distancesArr[i] = -1;
		_pathArr[i] = -1;
	} 
	
	/*initalization of src*/
	hasBeenSeenArr[_src] = TRUE;
	_distancesArr[_src] = 0;
	_pathArr[_src] = -1;
	
	queue = QueueCreate (numOfNodes);
	QueueInsert(queue, &indexesArr[_src]);  /*insert src to head of queue*/	
	
	while( !IsQueueEmpty(queue) ){
		QueueRemove(queue, (void*)&popItem);
		for(i=0; i<numOfNodes; i++){
			sonIndex = ( (*popItem) * numOfNodes ) + i;
			if(arcs[sonIndex] != 0){   /*check if i is son of popItem*/
				if(	!hasBeenSeenArr[i] ){
					hasBeenSeenArr[i] = TRUE;
					_distancesArr[i] = _distancesArr[(*popItem)] + 1;   /*set distance to src*/
					_pathArr[i] = *popItem ;                            /*set father at path to src*/
					QueueInsert(queue, &indexesArr[i]);  /*insert son to queue*/
				}
			}
		}
	}
	
	
	free(indexesArr);
	free(hasBeenSeenArr);
	QueueDestroy(queue);
	
	return OK;

}

/*--------------------------------------------------------------------------------*/

int IsLoopExists(Graph* _graph){
	int numOfNodes, i;

	assert(_graph);
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);	


	for(i=0; i<numOfNodes; i++){
		if(IsReachableDFS(_graph, i, i) == TRUE){
			return TRUE;
		}
	}

	return FALSE;

}

/*--------------------------------------------------------------------------------*/

ADTERR Dijkstra(Graph* _graph, size_t _src , int* indexesArr , int*  distancesArr, int* pathArr ){
	DijstraStruct* DijstraStructArr;
	Vector* vec;
	Vector* resultVec;	
	int i, numOfNodes, sonArcIndex, arcWeight;
	WeakOrderFunc heapFunc= WeekFuncDijstra;
	/*PrintFunc printFunc = PrintFuncDijstra;*/
	Heap* heap;
	DijstraStruct* curDijstraStruct;
	/*DijstraStruct* sonDijstraStruct;*/
	DijstraStruct* toGet;
	
	assert(_graph);
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);	
	
	
	DijstraStructArr = (DijstraStruct*) calloc (numOfNodes , sizeof(DijstraStruct) ); 
	if(DijstraStructArr == NULL){
		return ALLOC_FAILED;
	}
	
	vec = VectorCreate(numOfNodes, numOfNodes);
	resultVec = VectorCreate(numOfNodes, numOfNodes);
	
	/*initalization*/
	for(i=0; i<numOfNodes; i++){
		DijstraStructArr[i].s_index = i;
		if(i == _src){
			DijstraStructArr[i].s_minDistance = 0;
			DijstraStructArr[i].s_fatherAtPath = 0;
		}else{
			DijstraStructArr[i].s_minDistance = INT_MAX;
			DijstraStructArr[i].s_fatherAtPath = -1;
		}	
		DijstraStructArr[i].s_hasBeenSeen = FALSE;	
			
		VectorInsert(vec, &DijstraStructArr[i]);    /*into work vector*/
	}


	heap = HeapBuild(vec , heapFunc);

	while(  VectorItemsNum(vec) > 0 ){
		curDijstraStruct = HeapExtract(heap);
		curDijstraStruct->s_hasBeenSeen = TRUE;
		VectorInsert(resultVec, curDijstraStruct);     /*insert into result vector*/
			
		for(i=0; i < numOfNodes ; i++){
			sonArcIndex = (curDijstraStruct->s_index * numOfNodes) + i;
			arcWeight = _graph->m_arcs[sonArcIndex] ;
			if(arcWeight != 0){   /*for all sons*/
				/*findDijstraStructByIndex(vec, i , &sonDijstraStruct , &heapIndex);  */ /*  graph index(i) -> heap index(struct->index)  */
				Relax(heap , curDijstraStruct , i , arcWeight);
			}
		}
		

		/*print for check*/
		/*printf("work vecror from heap:\n");
		VectorPrint(vec , printFunc);
		printf("result vector :\n");
		VectorPrint(resultVec , printFunc);*/
			
	}
	
	/*return distances and paths */
	for(i=0; i < numOfNodes ; i++){
		VectorGet (resultVec, i, (void**)&toGet) ;  /*get item from result vector*/
		indexesArr[i] = toGet->s_index;
		distancesArr[i] = toGet->s_minDistance;
		pathArr[i] = toGet->s_fatherAtPath;
	}

	
	
	HeapDestroy(heap);
	free(DijstraStructArr);
	
	return OK;

}



static void Relax(Heap* _heap, DijstraStruct* _fatherDijstraStruct , int _sonIndex , int _arcWeight){
	int newDistance , fatherIndex , heapIndex ;
	int* hushIndexes;
	DijstraStruct* sonDijstraStruct;
	
	hushIndexes = HeapGetHushIndexes(_heap);   /*get hash of indexes from heap*/
	heapIndex = hushIndexes[_sonIndex];        /*  graph index(i) -> heap index(struct->index)  */  
		
	VectorGet(  HeapGetVector(_heap) , heapIndex , (void**)&sonDijstraStruct  )   ;
	
	fatherIndex = _fatherDijstraStruct->s_index;
	newDistance = _fatherDijstraStruct->s_minDistance + _arcWeight;
	
	if( sonDijstraStruct->s_minDistance > newDistance){
		sonDijstraStruct->s_minDistance = newDistance;
		sonDijstraStruct->s_fatherAtPath = fatherIndex;
		HeapIncreaceKeyFix(_heap, heapIndex  );   /*fix heap*/ 
	}

}


/*
static void findDijstraStructByIndex(Vector* _vec, int _graphIndex , DijstraStruct** _retDijstraStruct,  int* _retHeapIndex){
	int numOfNodes, i; 
	DijstraStruct* curDijstraStruct;
	
	numOfNodes = VectorItemsNum(_vec);	
	
	for(i=0; i<numOfNodes; i++){
		VectorGet(_vec, i, (void**)&curDijstraStruct);
		if(curDijstraStruct->s_index == _graphIndex){
			*_retDijstraStruct = curDijstraStruct;
			*_retHeapIndex = i;
		}
	}
	
}
*/


static int WeekFuncDijstra (void* item1, void* item2){
	if(  ((DijstraStruct*)item1)->s_minDistance > ((DijstraStruct*)item2)->s_minDistance   )   {
		return 1;
	}else{
		return 0;
	}
	
}

/*
static void PrintFuncDijstra(Vector* _vec){
	int iter=0;
	DijstraStruct* toGet;	

	printf("\n"); 
	while(iter < VectorItemsNum(_vec) ){
		VectorGet (_vec,iter,(void**)&toGet) ;
		printf("index:%d  minDist:%d  wasSeen:%d father:%d\n", toGet->s_index, toGet->s_minDistance , toGet->s_hasBeenSeen , toGet->s_fatherAtPath);		
		iter++;
	}
	printf("\n\n");

}
*/

/*--------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------*/




