#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define uchar unsigned char

typedef struct DNode DNode;
struct DNode {
    char symbol;
    struct DNode* left;
    struct DNode* right;
};

typedef struct DWriteInfo DWriteInfo;
struct DWriteInfo {
    uchar byte;
    int pos;
};

struct DNode* NewEdge(int value);
void FreeDecodeTree(struct DNode *root);
bool IsLeafD(struct DNode *root);

bool WriteValue(DWriteInfo *info, uchar *value, FILE *in);
struct DNode* RecoverTree(DWriteInfo *info, FILE *in);
bool RecoverData(struct DNode *root, DWriteInfo *info, FILE *in, FILE *out);

void Decode(FILE *in, FILE *out);
