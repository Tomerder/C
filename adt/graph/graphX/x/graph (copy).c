#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "queue.h"


#define RETURN_ERR if(err!=0) return err


typedef enum {FALSE, TRUE} IsConected;
typedef enum {DIRECTED_BIT, WEIGHTED_BIT} Bitmap;

/*--------------------------------------------------------------------------------*/

struct Graph{
	Vector*  	  m_nodes;	
    int**    	  m_arcs;
    unsigned int  m_graphMode;
};

/*--------------------------------------------------------------------------------*/

static int IsDirected(Graph* _graph);
static int IsWeighted(Graph* _graph);
static int IsBitOn(unsigned int _graphMode, int _bitNum);

static ADTERR GraphJoinCall (Graph* _graph, size_t _src, size_t _dst , int _weight , int* _oldweight);
static ADTERR GraphRemoveCall (Graph* _graph, size_t _src, size_t _dst , int* _oldWeight );

static ADTERR CheckIndexes(Graph* _graph, size_t _src, size_t _dst);

/*--------------------------------------------------------------------------------*/

Graph* GraphCreate (Vector* _vec, unsigned int _graphMode){
	Graph* graph;
	int numOfNodes,i; 
	
	
	assert(_vec);
	
	graph = (Graph*) malloc (sizeof(Graph)) ;
	if ( graph == NULL ) {
		return NULL;
	}
	
	graph->m_nodes = _vec;
	numOfNodes = VectorItemsNum(_vec);

	
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
	int** arcs;
	
	err = CheckIndexes(_graph, _src, _dst);
	RETURN_ERR;
	
	arcs = _graph->m_arcs ; 
	
	if(_oldweight){   /*return old weight*/
		*_oldweight = arcs[_src][_dst];
	} 	
	
	arcs[_src][_dst] = _weight;
	
	if ( !IsDirected(_graph) ){
		arcs[_dst][_src] = _weight;
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


static ADTERR GraphRemoveCall (Graph* _graph, size_t _src, size_t _dst , int* _oldWeight ){
	int err = 0;
	int** arcs;
	
	err = CheckIndexes(_graph, _src, _dst);
	RETURN_ERR;
	
	arcs = _graph->m_arcs ; 
	
	if(_oldWeight){     /*return old weight*/
		*_oldWeight = arcs[_src][_dst];
	}
	
	arcs[_src][_dst] = FALSE;
	
	if ( !IsDirected(_graph) ){
		arcs[_dst][_src] = FALSE;
	}
	
	return OK;


}


ADTERR GraphRemove (Graph* _graph, size_t _src, size_t _dst){
	assert(_graph);
	
	if(IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphRemoveCall(_graph, _src, _dst, NULL);

}


ADTERR GraphRemoveWT (Graph* _graph, size_t _src, size_t _dst, int* _oldWeight ){
	assert(_graph);
	
	if(!IsWeighted(_graph)){
		return WRONG_API;
	}
	
	return GraphRemoveCall(_graph, _src, _dst, _oldWeight);


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
	
	err = CheckIndexes(_graph, _src, _dst);
	RETURN_ERR;
	
	arcWeight = _graph->m_arcs[_src][_dst];
	
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
	
	numOfNodes = VectorItemsNum(_graph->m_nodes);
	
	while(i < numOfNodes){
		printf("node : %d =>" , i);
		for(j=0; j < numOfNodes; j++){
			weight = _graph->m_arcs[i][j];
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

	assert(_graph);
	
	err = CheckIndexes(_graph, _src, _dst);
	if (err != OK){
		return -1;
	}

	numOfNodes = VectorItemsNum(_graph->m_nodes);
	
	/*arr of indexes to insert to queue*/
	indexesArr = (int*) calloc (sizeof(int) , numOfNodes); 
	for(i=0; i<numOfNodes; i++){
		indexesArr[i] = i;
	}
	
	hasBeenSeenArr = (int*) calloc (sizeof(int) , numOfNodes); 

	queue = QueueCreate (numOfNodes);

	QueueInsert(queue, &_src);

	while( !IsQueueEmpty(queue) ){
		QueueRemove(queue, &popItem);
		if ( *(int*)popItem == _dst){
			return TRUE;
		}

		for(i=0; i<numOfNodes; i++){
			if(_graph->m_arcs[*(int*)popItem][i] != 0){   /*check if i is son of popItem*/
				if(!hasBeenSeenArr[i] ){
					hasBeenSeenArr[i] = TRUE;					
					QueueInsert(queue, &indexesArr[i] );
				}	
			}
		}

	}

	free(indexesArr);
	QueueDestroy(queue);
	
	return FALSE;

}






