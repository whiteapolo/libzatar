#include <stdlib.h>
#include <string.h>
#include "graph.h"

static int idCounter = 0;

void graphInit(graph *g)
{
	g->vertecies = NULL;
	g->edges = NULL;
	g->size = 0;
}

int graphAddVertex(graph *g, char data)
{
	g->vertecies = realloc(g->vertecies, sizeof(Vertex) * ++g->size);

	g->vertecies[g->size - 1].data = data;
	g->vertecies[g->size - 1].id = idCounter++;

	g->edges = realloc(g->edges, sizeof(bool*) * g->size);

	for (int i = 0; i < g->size - 1; i++) {
		g->edges[i] = realloc(g->edges[i], sizeof(bool));
		g->edges[i][g->size -1 ] = 0;
	}

	g->edges[g->size -1 ] = calloc(sizeof(bool), g->size);

	return idCounter - 1;
}

void graphAddEdge(graph *g, int id1, int id2)
{
	g->edges[id1][id2] = true;
}

void graphAddDoubleEdge(graph *g, int id1, int id2)
{
	g->edges[id1][id2] = true;
	g->edges[id2][id1] = true;
}

void graphRemoveEdge(graph *g, int id1, int id2)
{
	g->edges[id1][id2] = false;
}

void graphRemoveDoubleEdge(graph *g, int id1, int id2)
{
	g->edges[id1][id2] = false;
	g->edges[id2][id1] = false;
}

bool graphIsAdjacent(const graph *g, int id1, int id2)
{
	return g->edges[id1][id2];
}

int graphGetAdjacentN(const graph *g, int id, int n)
{
	int count = 0;
	for (int i = 0; i < g->size; i++)
		if (graphIsAdjacent(g, id, i) && count++ == n)
			return i;
	return -1;
}

void graphFree(graph *g)
{
	for (int i = 0; i < g->size; ++i)
		free(g->edges[i]);
	free(g->edges);
	free(g->vertecies);
}

void graphForEveryAdjacent(graph *g, int id, void (*action)(int))
{
	for (int i = 0; i < g->size; i++)
		if (graphIsAdjacent(g, id, i))
			action(i);
}

char graphGetData(const graph *g, int id)
{
	return g->vertecies[id].data;
}

void graphPrint(const graph *g)
{
	for (int i = 0; i < g->size; ++i) {
		for (int j = 0; j < g->size; ++j) {
			printf("%3d", g->edges[i][j]);
		}
		printf("\n");
	}
}
