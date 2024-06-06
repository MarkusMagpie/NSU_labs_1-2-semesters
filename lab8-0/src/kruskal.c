#include "kruskal.h"

DSU *InitDSU(int num_of_vertices) {
    DSU* dsu = malloc(sizeof(DSU));
    dsu->Parent = calloc(num_of_vertices, sizeof(int));
    for (int i = 0; i < num_of_vertices; ++i) {
        dsu->Parent[i] = i;
    }
    dsu->Rank = calloc(num_of_vertices, sizeof(int));
    return dsu;
}

int FindSet(DSU *dsu, int vertex) {
    if (vertex == dsu->Parent[vertex]) {
        return vertex;
    }
    dsu->Parent[vertex] = FindSet(dsu, dsu->Parent[vertex]);
    return dsu->Parent[vertex];
}

void UniteSets(DSU *dsu, int setX, int setY) {
    if (dsu->Rank[setX] > dsu->Rank[setY]) {
        dsu->Parent[setY] = setX;
        return;
    }
    dsu->Parent[setX] = setY;
    if (dsu->Rank[setX] == dsu->Rank[setY]) {
        dsu->Rank[setY]++;
    }
}

int Kruskal(Edge *edges, DSU *dsu, Edge *tree, int num_of_vertices, int num_of_edges) {
    int num_edges = 0;
    int new_num = 0;
    while (num_edges < num_of_edges && new_num < num_of_vertices) {
        int setX = FindSet(dsu, edges[num_edges].x - 1);
        int setY = FindSet(dsu, edges[num_edges].y - 1);
        if (setX != setY) {
            tree[new_num] = edges[num_edges];
            UniteSets(dsu, setX, setY);
            ++new_num;
        }
        ++num_edges;
    }
    return new_num;
}

void Print(Edge *edges, int num_of_vertices, int new_num) {
    if (new_num != num_of_vertices - 1) {
        printf("no spanning tree\n");
    } else {
        for (int i = 0; i < num_of_vertices - 1; ++i) {
            printf("%d %d\n", edges[i].x, edges[i].y);
        }
    }
}

void FreeDSU(DSU *dsu) {
    free(dsu->Parent);
    free(dsu->Rank);
    free(dsu);
}
