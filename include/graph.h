#ifndef GRAPH_H
#define GRAPH_H
#include "shared.h"

typedef struct {
	char data;
	int id;
} Vertex;

typedef struct {
	Vertex *vertecies;
	bool **edges;
	int size;
} graph;

void graphInit(graph *g);
int graphAddVertex(graph *g, char data);
void graphAddEdge(graph *g, int id1, int id2);
void graphAddDoubleEdge(graph *g, int id1, int id2);
void graphRemoveEdge(graph *g, int id1, int id2);
void graphRemoveDoubleEdge(graph *g, int id1, int id2);
bool graphIsAdjacent(const graph *g, int id1, int id2);
int graphGetAdjacentN(const graph *g, int id, int n);
void graphFree(graph *g);
void graphForEveryAdjacent(graph *g, int id, void (*action)(int));
char graphGetData(const graph *g, int id);
void graphPrint(const graph *g);

#endif
