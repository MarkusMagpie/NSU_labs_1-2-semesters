#include <stdlib.h>
#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int len_st, int len_fin, int *N) {
    int pivot = N[(len_fin + len_st) / 2]; // pivot value - middle element
    int left = len_st - 1, right = len_fin + 1;
    while (left < right) {
        do {
            ++left;
        } while (N[left] < pivot);
        do {
            --right;
        } while (N[right] > pivot);
        if (left >= right) return right;
        swap(&N[left], &N[right]);
    }
    return right;
}

void quick_sort(int *N, int start, int end) {
    if (start >= 0 && end >= 0 && start < end) {
        int pivot = partition(start, end, N);
        quick_sort(N, start, pivot);
        quick_sort(N, pivot + 1, end);   
    }
}

int main(void) {
    int array_len = 0;
    int *N = {0};
    if (scanf("%d\n", &array_len)) {
        N = (int *)malloc(array_len * sizeof(int));
        for (int i = 0; i < array_len; ++i) {
            if (scanf("%d\n", &N[i])) {
                continue;
            }
        }
    }
    quick_sort(N, 0, array_len - 1);

    for (int i = 0; i < array_len; ++i) {
        printf("%d ", N[i]);
    }

    free(N);
    printf("\n");
    return 0;
}
