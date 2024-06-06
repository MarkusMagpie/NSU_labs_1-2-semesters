#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512
typedef unsigned char uchar;

typedef struct ENode ENode;
struct ENode {
    uchar symbol;           // Symbol represented by node
    int frequency;          // Frequency of symbol
    struct ENode* left;     // Pointer to left child node
    struct ENode* right;    // Pointer to right child node
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue {
    int size;               // Current size of queue
    int capacity;           // Capacity of queue
    ENode** array;          // Array of pointers to nodes
};

typedef struct WriteInfo WriteInfo;
struct WriteInfo {
    uchar byte;
    int pos;
};

//node functions
struct ENode* NewNode(char symbol, int freq);
int IsLeaf(ENode *freqtreeroot);
int CompareFrequency(const ENode *a, const ENode *b);

//priority queue functions
PriorityQueue* NewPriorityQueue(int capacity);
void InsertInQueue(PriorityQueue* queue, struct ENode* node);
void DeleteFromQueue(PriorityQueue* queue);
void HeapifyMin(PriorityQueue* queue, int index); // Heapify the subtree rooted at index
void InitQueue(PriorityQueue* queue, int SymbolFrequency[256]);
void BuildFrequencyTable(FILE* in, PriorityQueue* queue);

//main functions
ENode* BuildHuffmanTree(PriorityQueue* queue);
void GenerateHuffmanCodes(ENode* root, char *code_storage, char **outcode, int length);

//write functions
void BitWrite(WriteInfo *info, char bit, FILE *out);
void FlushBuffer(int *pos, uchar *byte, FILE *out);
void WriteTree(ENode *root, WriteInfo *info, FILE *out);
void WriteCodes(char **outcodes, WriteInfo *info, FILE *in, FILE *out);

//free functions
void FreeTree(struct ENode *root);
void FreeData(struct ENode *root, char *code, char **outcode);
void FreeQueue(struct PriorityQueue *queue);

void Encode(FILE *in, FILE *out);
