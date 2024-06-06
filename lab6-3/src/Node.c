#include <stdio.h>
#include <stdlib.h>

#include "Node.h"

Root *NewRoot(void) {
    Root *New = malloc(sizeof(Root));
    New->root = NULL;
    New->count = 0;
    return New;
}

Node* NewNode(char* newKey, int keyLen, bool Flag) {
    Node* New = malloc(sizeof(Node));
    New->key = malloc((keyLen + 1) * sizeof(char));
    for (int i = 0; i < keyLen; i++) {	// add insides of newKey into New->key
        New->key[i] = newKey[i];
    }
	New->key[keyLen] = '\0';


    New->len = keyLen;
    New->child = NULL;
    New->bro = NULL;
    New->Flag = Flag;

    return New;
}

void FreeNode(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->child != NULL) {
        FreeNode(node->child);
    }
    if (node->bro != NULL) {
        FreeNode(node->bro);
    }
    free(node->key);
    free(node);
}

void Split(Node* old, int splitIndex) {
    Node* New = NewNode(old->key + splitIndex, old->len - splitIndex, false);
    New->child = old->child;
    old->child = New;

    char* key_prefix = malloc((splitIndex + 1) * sizeof(char));
    key_prefix[splitIndex] = '\0';
    strncpy(key_prefix, old->key, splitIndex);

    free(old->key);

    old->key = key_prefix;
    old->len = splitIndex;
}

Node* Insert(Node* node, char* word, int word_len) {
    if (word_len == 0) {
        word_len = strlen(word) + 1;
    }
    if (node == NULL) {    //first node
        return NewNode(word, word_len, true);
    }

    int splitIndex = PrefixLength(word, word_len, node->key, node->len);
    if (splitIndex == 0) {
        node->bro = Insert(node->bro, word, word_len);
    } else if (splitIndex <= word_len) {
        if (splitIndex < node->len) {
            Split(node, splitIndex);
            if (node->Flag == true) {
                node->child->Flag = true;
            }
            node->Flag = false;
        }
        
        int comparison_res = strcmp(word, node->key);    //node->key is the new word after split (aaaaaaa\na case)
        if (comparison_res == 0) {
            node->Flag = true;
        } else if (comparison_res != 0) {
            node->child = Insert(node->child, word + splitIndex, word_len - splitIndex);
        }
    }
    return node;
}

int PrefixLength(char* word, int word_len, char* key, int keyLen) {
    for (int i = 0; i < word_len; i++) {
        if (i == keyLen || word[i] != key[i]) {
            return i;
        }
    }
    return word_len;
}
