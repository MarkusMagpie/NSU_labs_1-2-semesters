#include <stdint.h>
#include <stdlib.h>

#include "graph.h"

Graph *InitGraph(size_t N) {
    Graph *g = malloc(sizeof(Graph));
    g->adjacencyMatrix = calloc(N * (N / 8 + (N % 8 != 0)), sizeof(char));              //9 vertices = (8 * (9 / 8 + (9 % 8 != 0)))
    g->vertexCount = N;
    
    return g;
}

void FreeGraph(Graph *g) {
    free(g->adjacencyMatrix);
    free(g);
    return;
}

void addEdge(Graph *g, int i, int j) {
    int byteIndex = j / 8;      // Get the byte index
    int bitPosition = j % 8;    // Get the bit position within the byte
    int BitsILine = g->vertexCount / 8 + (g->vertexCount % 8 != 0);
    g->adjacencyMatrix[(i * BitsILine) + byteIndex] |= (1 << bitPosition);
    return;
}

int getEdge(Graph *g, int i, int j) {
    int byteIndex = j / 8;
    int bitPosition = j % 8;
    int BitsILine = g->vertexCount / 8 + (g->vertexCount % 8 != 0);
    return g->adjacencyMatrix[(i * BitsILine) + byteIndex] & (1 << bitPosition);
}
