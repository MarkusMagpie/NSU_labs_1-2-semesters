#include "outputs.h"

void FirstOutput(long *distance_spt, int num_of_vertices) {
    for (int i = 0; i < num_of_vertices; ++i) {
        if (distance_spt[i] == LONG_MAX) {
            printf("oo ");
        } else if (distance_spt[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%ld ", distance_spt[i]);
        }
    }
}

void SecondOutput(int *adjacencyMatrix, long *distance_spt, int *parent, int num_of_vertices, int start, int finish) {
    if (distance_spt[finish] == -1 || distance_spt[finish] == LONG_MAX) {
        printf("no path");
    } else {
        bool overflowed = false;
        if (distance_spt[finish] > INT_MAX) {
            for (int i = 0; i < num_of_vertices; ++i) {
                int cur_cell = adjacencyMatrix[i * num_of_vertices + finish];
                if (cur_cell != -1 && distance_spt[i] + cur_cell > INT_MAX && i != parent[finish]) {
                    overflowed = true;
                    break;
                }
            }
            if (overflowed) printf("overflow");
        }
        if (!overflowed) {
            int i = finish;
            printf("%d ", finish + 1);
            while (i != start) {
                printf("%d ", parent[i] + 1);
                i = parent[i];
            }
        }
    }
}
