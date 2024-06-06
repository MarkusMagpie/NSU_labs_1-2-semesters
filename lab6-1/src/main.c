#include <stdlib.h>
#include <stdio.h>

#define RED 'r'
#define BLACK 'b'

typedef struct Node Node;
struct Node {
    int data;
    char color;
    struct Node *left;
    struct Node *right;
};

struct Node T_Nil_Node;
Node *nil = &T_Nil_Node;

Node *NewNode(int data, Node *arr, int i) {
    Node *newNode = &arr[i];
    newNode->data = data;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void RotateLeft(Node **root, Node *NewNode, Node *parentArr[], int index) {
    Node *temp = NewNode->right;
    NewNode->right = temp->left;

    if (index == 0) {
        *root = temp;
    } else if (parentArr[index - 1]->left == NewNode) {
        parentArr[index - 1]->left = temp;
    } else {
        parentArr[index - 1]->right = temp;
    }

    temp->left = NewNode;
}

void RotateRight(Node **root, Node *NewNode, Node *parentArr[], int index) {
    Node *temp = NewNode->left;
    NewNode->left = temp->right;
    
    if (index == 0) {
        *root = temp;
    } else if (parentArr[index - 1]->right == NewNode) {
        parentArr[index - 1]->right = temp;
    } else {
        parentArr[index - 1]->left = temp;
    }

    temp->right = NewNode;
}

void ColorSwapParent(Node *parent, Node *grandparent) {
    parent->color = BLACK;
    grandparent->color = RED;
}

void Balance(Node **root, Node *NewNode, Node *parentArr[], int index) {
    if (NewNode == root[0]) {
        NewNode->color = BLACK;
        return;
    }

    Node *Parent = parentArr[index - 1];

    if (Parent->color == RED) {
        Node *temp;
        Node *GrandParent = parentArr[index - 2];
        if (Parent == GrandParent->left) {
            temp = GrandParent->right;
            if (temp->color == RED) {
                temp->color = BLACK;
                ColorSwapParent(Parent, GrandParent);
                if (GrandParent != root[0]) {
                    Balance(root, GrandParent, parentArr, index - 2);
                }
            } else {
                if (Parent->right == NewNode) {                    
                    parentArr[index] = parentArr[index - 1];
                    NewNode = Parent;
                    RotateLeft(root, NewNode, parentArr, index - 1);
                    parentArr[index - 1] = parentArr[index - 2];
                    Parent = parentArr[index - 1];
                    GrandParent = parentArr[index - 2];                }
                ColorSwapParent(Parent, GrandParent);
                RotateRight(root, GrandParent, parentArr, index - 2);
            }
        }
        else {
            temp = GrandParent->left;
            if (temp->color == RED) {
                temp->color = BLACK;
                ColorSwapParent(Parent, GrandParent);
                if (GrandParent != root[0]) {
                    Balance(root, GrandParent, parentArr, index - 2);
                }
            } else {
                if (Parent->left == NewNode) {
                    parentArr[index] = parentArr[index - 1];
                    NewNode = Parent;
                    RotateRight(root, NewNode, parentArr, index - 1);
                    parentArr[index - 1] = parentArr[index - 2];
                    Parent = parentArr[index - 1];
                    GrandParent = parentArr[index - 2];
                }
                ColorSwapParent(Parent, GrandParent);
                RotateLeft(root, GrandParent, parentArr, index - 2);
            }
        }
    }
    root[0]->color = BLACK;
}

void Insert(Node **root, Node *NewNode) {
    Node *new_parent = nil;
    Node *temp = *root;
    
    Node *parentArr[64];
    int index = 0;

    while (temp != nil) {
        new_parent = temp;
        parentArr[index] = temp;
        index++;
        if (NewNode->data < temp->data) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }

    if (new_parent == nil) {
        *root = NewNode;
    } else if (NewNode->data < new_parent->data) {
        new_parent->left = NewNode;
    } else {
        new_parent->right = NewNode;
    }

    NewNode->left = nil;
    NewNode->right = nil;
    NewNode->color = RED;

    Balance(root, NewNode, parentArr, index);
}

int black_height(Node *root) {
    if (root == nil) {
        return 1;
    }

    int RightBlackHeight = black_height(root->right);

    return RightBlackHeight + (root->color == BLACK ? 1 : 0);
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    if (N == 0) {
        printf("0\n");
        return 0;
    }

    Node *root = nil;
    Node *arr = malloc(sizeof(Node) * N);
    int input_data;

    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &input_data) == 1) {
            Node *newNode = NewNode(input_data, arr, i);
            Insert(&root, newNode);
        } else break;
    }

    printf("%d\n", black_height(root));
    free(arr);
    return 0;
}
