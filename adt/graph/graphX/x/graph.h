#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "err.h"
#include "vec.h"




typedef struct Graph Graph;


Graph* GraphCreate (Vector* _vec, unsigned int _graphMode);
void GraphDestroy (Graph* _graph);

ADTERR GraphJoin (Graph* _graph, size_t _src, size_t _dst);
ADTERR GraphRemove (Graph* _graph, size_t _src, size_t _dst);

ADTERR GraphJoinWT (Graph* _graph, size_t _src, size_t _dst, int _weight);
ADTERR GraphRemoveWT (Graph* _graph, size_t _src, size_t _dst, int* _weight);
ADTERR GraphChangeWT (Graph* _graph, size_t _src, size_t _dst, int _newWeight, int* _oldWeight);

int GraphIsAdjacent (Graph* _graph, size_t _src, size_t _dst);
int GraphGetWT (Graph* _graph, size_t _src, size_t _dst);

void GraphPrint(Graph* _graph);

int IsReachableBFS(Graph* _graph, size_t _src, size_t _dst);



#endif /* #ifndef __GRAPH_H__ */






