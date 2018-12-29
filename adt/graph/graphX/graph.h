#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "err.h"
#include "vec.h"


typedef struct Graph Graph;

/*----------------------------------------------------------------------------------------*/
 /* graphMode =>  bit 0 : DIRECTED_BIT , bit 1 : WEIGHTED_BIT */
Graph* GraphCreate (Vector* _vec, unsigned int _graphMode);
void GraphDestroy (Graph* _graph);
/*----------------------------------------------------------------------------------------*/
ADTERR GraphJoin (Graph* _graph, size_t _src, size_t _dst);
ADTERR GraphRemove (Graph* _graph, size_t _src, size_t _dst);
/*----------------------------------------------------------------------------------------*/
ADTERR GraphJoinWT (Graph* _graph, size_t _src, size_t _dst, int _weight);
ADTERR GraphRemoveWT (Graph* _graph, size_t _src, size_t _dst, int* _weight);
ADTERR GraphChangeWT (Graph* _graph, size_t _src, size_t _dst, int _newWeight, int* _oldWeight);
/*----------------------------------------------------------------------------------------*/
int GraphIsAdjacent (Graph* _graph, size_t _src, size_t _dst);
int GraphGetWT (Graph* _graph, size_t _src, size_t _dst);
/*----------------------------------------------------------------------------------------*/
void GraphPrint(Graph* _graph);
/*----------------------------------------------------------------------------------------*/
/*BFS algo - is _dst reachable from _src
	space complexity: o(n)
	time complexity: o(n^2)
	returns 1 if reachable , 0 not reachable , -1 for error
*/
int IsReachableBFS(Graph* _graph, size_t _src, size_t _dst);
/*----------------------------------------------------------------------------------------*/
/*DFS algo - is _dst reachable from _src
	space complexity: o(1)
	time complexity: o(n^2)
	returns 1 if reachable, 0 not reachable , -1 for error
*/
int IsReachableDFS(Graph* _graph, size_t _src, size_t _dst);
/*----------------------------------------------------------------------------------------*/

/*BFS extended algo - finds path and distance from _src to all nodes 
	parameters: 
			_distancesArr = user supplied initialized arr (malloc) - size of nodes number 
			_pathArr      = user supplied initialized arr (malloc) - size of nodes number
							will return the father of each node at the path
	space complexity: o(n)
	time complexity: o(n^2)
*/
ADTERR AllPathsFromSrcBFS(Graph* _graph, size_t _src, int* _distancesArr, int* _pathArr );
/*----------------------------------------------------------------------------------------*/

int IsLoopExists(Graph* _graph);

/*----------------------------------------------------------------------------------------*/
#endif /* #ifndef __GRAPH_H__ */






