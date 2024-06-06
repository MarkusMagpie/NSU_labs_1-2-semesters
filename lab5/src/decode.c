#include "decode.h"

struct DNode* NewEdge(int value) {
    struct DNode *edge = malloc(sizeof(DNode));
    edge->symbol = value;
    edge->left = NULL;
    edge->right = NULL;

    return edge;
}

void FreeDecodeTree(struct DNode *root) {
    if (root == NULL) {
        return;
    }
    if (root->left) FreeDecodeTree(root->left);
    if (root->right) FreeDecodeTree(root->right);
    free(root);
}

bool IsLeafD(struct DNode *root) {
    if (root == NULL) return false;
    return ((!root->left && !root->right) ? true : false);
}

bool WriteValue(DWriteInfo *info, uchar *value, FILE *in) {
    uchar val = 0;
    // Move the bits from the current byte to the first part of the val
    val |= (info->byte << info->pos);
    // Read the first part of the value from the next byte of input stream
    if (fread(&info->byte, sizeof(uchar), 1, in) == 0) {
        return false;
    }
    // Move the bits from the current byte(I've just read) to the last part of the val
    val |= (info->byte >> (8 - info->pos));

    if (info->pos == 8) {
        fread(&info->byte, sizeof(uchar), 1, in);
        info->pos = 0;
    }

    (*value) = val;
    return true;
}

struct DNode* RecoverTree(DWriteInfo *info, FILE *in) {
    if (info->pos == 8) {
        fread(&info->byte, sizeof(uchar), 1, in);
        info->pos = 0;
    }
    info->pos++;

    if (((info->byte >> (8 - info->pos) & 1) + '0') == '1') {
        uchar value;
        if (!(WriteValue(info, &value, in))) {
            return NULL;
        }
        return NewEdge(value);
    } else {
        // Build a tree with the recursive left and right children (not a leaf)
        DNode *root = NewEdge(0);
        root->left = RecoverTree(info, in);
        root->right = RecoverTree(info, in);
        return root;
    }
}

// I traverse the tree and write the value to the output
bool RecoverData(struct DNode *root, DWriteInfo *info, FILE *in, FILE *out) {
    while (!IsLeafD(root)) {
        info->pos++;
        if (info->pos > 8) {
            // if (fread(&info->byte, sizeof(uchar), 1, in) == 0) return false;
            int temp;
            if ((temp = fgetc(in)) == EOF) return false;
            info->byte = temp;
            info->pos = 1;
        }
        // Check the current bit in the byte(same as in RecoverTree function)
        // if it is 1 - go to the right child; if it is 0 - go to the left; (frequency tree)
        if (((info->byte >> (8 - info->pos) & 1) + '0') == '1') {
            root = root->right;
        }
        if (((info->byte >> (8 - info->pos) & 1) + '0') == '0') {
            root = root->left;
        }
    }

    fwrite(&(root->symbol), sizeof(char), 1, out);    
    return true;
}

void Decode(FILE *in, FILE *out) {
    int data_length = 0;
    if (fread(&data_length, sizeof(int), 1, in) == 0) {
        return;
    }
    DWriteInfo info;
    info.pos = 0;
    info.byte = 0;

    if (fread(&(info.byte), sizeof(uchar), 1, in) == 0) {
        return;
    }

    struct DNode *root = RecoverTree(&info, in);
    if (root == NULL) return;

    for (int i = 0; i < data_length; ++i) {
        if (!(RecoverData(root, &info, in, out))) {
            FreeDecodeTree(root);
            return;
        }
    }

    FreeDecodeTree(root);
}
