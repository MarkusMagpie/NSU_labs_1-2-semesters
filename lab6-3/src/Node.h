#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 10001

typedef struct Node Node;
struct Node {
	char* key;
	int len;
    bool Flag;
	struct Node* child;
	struct Node* bro;
};

typedef struct Root Root;
struct Root {
	Node* root;
	int count;
};

Root* NewRoot(void); 

Node* NewNode(char* newKey, int keyLen, bool HasWord);
void FreeNode(Node* node);

void Split(Node* node, int splitIndex);
Node* Insert(Node* node, char* word, int wordLen);

int PrefixLength(char* word, int wordLen, char* key, int keyLen);
