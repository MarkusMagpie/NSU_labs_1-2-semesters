#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Edge Edge;
struct Edge {
    int x, y, length;
};

typedef struct DSU DSU;
struct DSU {
    int *Parent;
    int *Rank;
};

DSU *InitDSU(int num_of_vertices);
int FindSet(DSU *dsu, int vertex);
void UniteSets(DSU *dsu, int setX, int setY);
int Kruskal(Edge *edges, DSU *dsu, Edge *tree, int num_of_vertices, int num_of_edges);
void Print(Edge *edges, int num_of_vertices, int new_num);
void FreeDSU(DSU *dsu);
