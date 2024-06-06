#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node; 

struct Node {
    int data;
    int height;
    struct Node *left;
    struct Node *right;
};

Node *NewNode(int data, Node *arr, int i) { 
    Node *newNode = &arr[i];
    newNode->data = data;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int height(Node *root) {
    return root?root->height:0;
}

int balance_factor(Node *root) {
    return height(root->right) - height(root->left);
}

void fix_height(Node *root) {
    int height_l = height(root->left);
    int height_r = height(root->right);
    root->height = (height_l > height_r?height_l:height_r) + 1;
}

Node *RotateRight(Node *root) { //Small right rotation
    Node *rotated_to_right = root->left; 
    root->left = rotated_to_right->right;
    rotated_to_right->right = root;
    fix_height(root);
    fix_height(rotated_to_right);
    return rotated_to_right;
}

Node *RotateLeft(Node *root) { //Small left rotation
    Node *rotated_to_left = root->right; 
    root->right = rotated_to_left->left;
    rotated_to_left->left = root;
    fix_height(root);
    fix_height(rotated_to_left);
    return rotated_to_left;
}

Node *Balance(Node *root) {
    fix_height(root);
    if (balance_factor(root) == 2) { //right side is longer by 2 (inappropriate)
        if (balance_factor(root->right) < 0) {
            root->right = RotateRight(root->right);
        }
        root = RotateLeft(root);
    }
    if (balance_factor(root) == -2) {
        if (balance_factor(root->left) > 0) {
            root->left = RotateLeft(root->left);
        }
        root = RotateRight(root);
    }
    return root;
}

Node *Insert(Node *root, Node *NewNode) {
    if (root == NULL) {
        return NewNode;
    }
    if (NewNode->data <= root->data) {
        root->left = Insert(root->left, NewNode);
    } else {
        root->right = Insert(root->right, NewNode);
    }
    return Balance(root);
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }

    Node *root = NULL;
    Node *arr = malloc(sizeof(Node) * N);
    int input;
    
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &input) == 1) {
            Node *newNode = NewNode(input, arr, i);
            root = Insert(root, newNode);
        } else {
            break;
        }
    }

    printf("%d", height(root));

    free(arr);
    printf("\n");
    return 0;
}
