#include <stdlib.h>
#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void make_heap(int *N, int len, int i) {
    int parent = i;

    while (parent < len) {
        int l_child = 2 * parent + 1;
        int r_child = 2 * parent + 2;

        if (l_child < len && N[parent] < N[l_child]) parent = l_child;
        if (r_child < len && N[parent] < N[r_child]) parent = r_child;
        if (parent == i) break;
        swap(&N[i], &N[parent]);
        i = parent;
    }
}

void heap_sort(int len, int *N) {
    for (int i = len / 2; i >= 0; --i) {
        make_heap(N, len, i);
    }
    for (int i = len - 1; i >= 0; --i) {
        swap(&N[0], &N[i]);
        make_heap(N, i, 0);
    }
}

int main(void) {
    int *N = NULL;
    int array_len = 0;
    int man = scanf("%d\n", &array_len);
    if (man != 0 && man != EOF) {
        N = malloc(array_len * sizeof(int));
        for (int i = 0; i < array_len; ++i) {
            man = scanf("%d", &N[i]);
            if (man != 0 && man != EOF) {
                continue;
            }
        }
    }

    heap_sort(array_len, N);

    for (int i = 0; i < array_len; ++i) {
        printf("%d ", N[i]);
    }

    free(N);
    printf("\n");
    return 0;
}
