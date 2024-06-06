#include "outputs.h"

typedef unsigned long long ull;

typedef enum {
    BADNUMLINES,
    BADNUMVERTICES,
    BADNUMEDGES,
    BADVERTEX,
    BADLEN,
    OK,
} Status;

char *ErrorOutput[] = {
    "bad number of lines",
    "bad number of vertices",
    "bad number of edges",
    "bad vertex",
    "bad length",
};

Status ScanfAndCheck(FILE *stream, int *N, int *S, int *F, int *M) {
    if (fscanf(stream, "%d\n%d %d\n%d", N, S, F, M) != 4) {
        return BADNUMLINES;
    }
    if (*N < 0 || *N > 5000) {
        return BADNUMVERTICES;
    }
    if ((*S < 1) || (*S > *N) || (*F < 1) || (*F > *N)) {
        return BADVERTEX;
    }
    if ((*M < 0) || (*M > (*N * (*N - 1) / 2))) {
        return BADNUMEDGES;
    }
    *S = *S - 1;
    *F = *F - 1;
    return OK;
}

Status ReadEdges(FILE *stream, int *adjacencyMatrix, int num_of_vertices, int num_of_edges) {
    int x, y;
    ull length;
    for (int i = 0; i < num_of_edges; ++i) {
        if (fscanf(stream, "%d %d %llu", &x, &y, &length) != 3) {
            return BADNUMLINES;
        }
        if (x < 1 || x > num_of_vertices || y < 1 || y > num_of_vertices) {
            return BADVERTEX;
        }
        if (length > INT_MAX) {
            return BADLEN;
        }
        x--;
        y--;
        adjacencyMatrix[x * num_of_vertices + y] = length;
        adjacencyMatrix[y * num_of_vertices + x] = length;
    }
    return OK;
}

int MinEdge(long *distance_spt, bool *vertices_spt, int num_of_vertices) {
    long min = LONG_MAX;
    int min_index = 0;
    for (int i = 0; i < num_of_vertices; ++i) {
        if ((vertices_spt[i] == false) && (distance_spt[i] <= min) && (distance_spt[i] != LONG_MAX)) {
            min = distance_spt[i];
            min_index = i;
        }
    }
    return min_index;
}

void DijkstraAlgo(int *adjacencyMatrix, int num_of_vertices, int start , int finish) {
    int *parent = malloc(num_of_vertices * sizeof(int));
    long *distance_spt = malloc(num_of_vertices * sizeof(long));
    bool *vertices_spt = malloc(num_of_vertices * sizeof(bool));

    for (int i = 0; i < num_of_vertices; ++i) {
        parent[i] = -1;
        distance_spt[i] = LONG_MAX;
        vertices_spt[i] = false;
    }

    distance_spt[start] = 0;
    int cur_len = 0;

    for (int i = 0; i < num_of_vertices; ++i) {
        int cur_min_vertex = MinEdge(distance_spt, vertices_spt, num_of_vertices);
        vertices_spt[cur_min_vertex] = true;
        for (int j = 0; j < num_of_vertices; ++j) {
            cur_len = adjacencyMatrix[cur_min_vertex * num_of_vertices + j];
            if (cur_len != -1 && vertices_spt[j] == false && distance_spt[j] > distance_spt[cur_min_vertex] + cur_len) {
                distance_spt[j] = distance_spt[cur_min_vertex] + cur_len;
                parent[j] = cur_min_vertex;
            }
        }
    }

    FirstOutput(distance_spt, num_of_vertices);

    printf("\n");

    SecondOutput(adjacencyMatrix, distance_spt, parent, num_of_vertices, start, finish);

    free(parent);
    free(vertices_spt);
    free(distance_spt);
}

int main(void) {
    int num_of_vertices = 0;
    int start, finish;
    int num_of_edges = 0;

    Status status;
    if ((status = ScanfAndCheck(stdin, &num_of_vertices, &start, &finish, &num_of_edges)) != OK) {
        printf("%s\n", ErrorOutput[status]);
        return 0;
    }

    int *adjacencyMatrix = malloc(num_of_vertices * num_of_vertices * sizeof(int));
    memset(adjacencyMatrix, -1, num_of_vertices * num_of_vertices * sizeof(int));

    if ((status = ReadEdges(stdin, adjacencyMatrix, num_of_vertices, num_of_edges)) != OK) {
        printf("%s\n", ErrorOutput[status]);
        free(adjacencyMatrix);
        return 0;
    }

    DijkstraAlgo(adjacencyMatrix, num_of_vertices, start, finish);
    printf("\n");

    free(adjacencyMatrix);
    return 0;
}
