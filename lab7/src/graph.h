#pragma once

#include <stdint.h>
#include "stack.h"

typedef struct _Graph {
    char *adjacencyMatrix;
    int vertexCount;                    //number of vertices
} Graph;

Graph *InitGraph(size_t N);
void FreeGraph(Graph *g);
void addEdge(Graph *g, int i, int j);
int getEdge(Graph *g, int i, int j);
