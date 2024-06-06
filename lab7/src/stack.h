#pragma once

#include <stdint.h>

typedef struct _Stack {
    int *array;                         //array for connectivity
    int len;                            //length of array
} Stack;

Stack *initStack(size_t N);
void freeStack(Stack *s);
void PrintStack(Stack *s);
void addToStack(Stack *s, int vertex);
