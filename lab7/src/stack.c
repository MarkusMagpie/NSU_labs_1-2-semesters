#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

Stack *initStack(size_t N) {
    Stack *stack = malloc(sizeof(Stack));
    stack->array = calloc(N, sizeof(int));
    stack->len = 0;
    return stack;
}

void freeStack(Stack *s) {
    free(s->array);
    free(s);
}

void addToStack(Stack *s, int vertex) {
    s->array[s->len] = vertex;
    s->len++;
}

void PrintStack(Stack *s) {
    while (s->len > 0) {
        printf("%d ", s->array[s->len - 1]);
        s->len--;
    }
}
