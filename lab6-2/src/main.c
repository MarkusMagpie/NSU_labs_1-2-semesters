#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node Node;
struct Node {
    int num_keys;
    int *keys;
    struct Node **children;
};

typedef struct BTree BTree;
struct BTree {
    int T;
    Node *root;
    int height;
};

Node *NewNode(int T, bool is_leaf) {
    Node *node = malloc(sizeof(Node));
    node->num_keys = 0;
    node->keys = malloc(sizeof(int) * (2 * T - 1));
    if (is_leaf) {
        node->children = NULL;
    } else {
        node->children = malloc(sizeof(Node *) * (2 * T));
    }
    return node;
}

BTree *NewBTree(int T) {
    BTree *tree = calloc(1, sizeof(BTree));
    tree->T = T;
    Node *node = NewNode(T, true);
    tree->root = node;
    tree->height = 1;
    return tree;
}

bool is_leaf(Node *node) {
    return (node->children == NULL) ? true : false;
}

void Split(Node *parent, int index, int T) {                  
    Node *old = parent->children[index];
    Node *new = NewNode(T, is_leaf(old));
    new->num_keys = T - 1;
    old->num_keys = T - 1;

    memmove(&(new->keys[0]), &(old->keys[T]), sizeof(int) * new->num_keys);
    if (is_leaf(old) == 0) {
        memmove(&(new->children[0]), &(old->children[T]), sizeof(Node *) * (new->num_keys + 1));
    }

    memmove(&(parent->keys[index + 1]), &(parent->keys[index]), sizeof(int) * (parent->num_keys - index));
    if (is_leaf(parent) == 0) {
        memmove(&(parent->children[index + 1]), &(parent->children[index]), sizeof(Node *) * (parent->num_keys - index + 1));
    }

    parent->keys[index] = old->keys[T - 1];
    parent->children[index + 1] = new;

    parent->num_keys++;
}

int GetIndex(Node *node, int key) {
    int left = 0;
    int right = node->num_keys - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (key < node->keys[middle]) {
            right = middle - 1;
        } else {
            left = middle + 1;
        } 
    }
    return right + 1;
}

void InsertNonFull(Node *node, int key, int T) {
    int index = GetIndex(node, key);

    if (is_leaf(node)) {
        memmove(&(node->keys[index + 1]), &(node->keys[index]), sizeof(int) * (node->num_keys - index));
        node->num_keys++;
        node->keys[index] = key;
    } else {
        if (node->children[index]->num_keys == (2 * T - 1)) {
            Split(node, index, T);
            if (key > node->keys[index]) {
                index++;
            }
        }
        InsertNonFull(node->children[index], key, T);
    }
}

void Insert(BTree *tree, int key) {
    if (tree->root->num_keys == (2 * tree->T - 1)) {
        Node *newNode = NewNode(tree->T, false);

        newNode->children[0] = tree->root;

        Split(newNode, 0, tree->T);
        tree->height++;
        tree->root = newNode;
    }
    InsertNonFull(tree->root, key, tree->T);
}

void free_node(Node *node) {
    free(node->keys);
    if (is_leaf(node)) {
        free(node);
    } else {
        for (int i = 0; i <= node->num_keys; ++i) {
            free_node(node->children[i]);
        }
        free(node->children);
        free(node);
    }
}

void free_btree(BTree *tree) {
    free_node(tree->root);
    free(tree);
}

int main(void) {
    int T;
    if (scanf("%d", &T) != 1) {
        printf("0\n");
        return 0;
    }

    int key_count;
    if (scanf("%d", &key_count) != 1) {
        printf("0\n");
        return 0;
    }
    if (key_count == 0) {
        printf("0\n");
        return 0;
    }
    
    BTree *tree = NewBTree(T);

    int key;
    for (int i = 0; i < key_count; ++i) {
        if (scanf("%d", &key) == 1) {
            Insert(tree, key);
        } else break;
    }

    printf("%d\n", tree->height);

    free_btree(tree);
    return 0;
}
