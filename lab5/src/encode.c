#include "encode.h"
#include "time.h"

// Initialization functions
ENode* NewNode(char symbol, int freq) {
    ENode *node = malloc(sizeof(ENode));
    node->symbol = symbol;
    node->frequency = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

PriorityQueue* NewPriorityQueue(int capacity) {
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    queue->size = 0;
    queue->capacity = capacity;
    queue->array = malloc(sizeof(ENode) * capacity);
    return queue;
}

int IsLeaf(ENode *freqtreeroot) {
    if (freqtreeroot == NULL) return 0;
    return (!freqtreeroot->left && !freqtreeroot->right);
}

int CompareFrequency(const ENode *a, const ENode *b) {
    return (a->frequency - b->frequency);
}

void InsertInQueue(PriorityQueue* queue, ENode* node) {
    int i = queue->size++;

    //compare frequency of the parent node (index (i - 1) / 2) with the frequency of the node
    //move up the parent nodes until find the correct position for the new node
    while (i > 0 && CompareFrequency(queue->array[(i - 1) / 2], node) > 0) {
        queue->array[i] = queue->array[(i - 1) / 2];
        i = (i - 1) / 2;   
    }

    queue->array[i] = node;
}

void InitQueue(PriorityQueue* queue, int SymbolFrequency[256]) {
    for (int i = 0; i < 256; i++) {
        if (SymbolFrequency[i] > 0) {
            ENode* node = NewNode(i, SymbolFrequency[i]);
            InsertInQueue(queue, node);
        }
    }
}

void BuildFrequencyTable(FILE* in, PriorityQueue* queue) {
    int SymbolFrequency[256] = {0};

    clock_t begin = clock();
    uchar* buffer = malloc(BUFFER_SIZE * sizeof(uchar));
    size_t length;
    while ((length = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, in)) > 0) {
        // Count the frequency of each ASCII symbol in the input BUFFER
        for (size_t i = 0; i < length; i++) {
            SymbolFrequency[(int)buffer[i]]++;
        }
    }
    clock_t end = clock();
    double timer = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on counting frequencies: %f\n", timer);
    
    clock_t begin2 = clock();
    InitQueue(queue, SymbolFrequency);
    clock_t end2 = clock();
    double timer2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on initializing queue: %f\n", timer2);

    free(buffer);
}

void HeapifyMin(PriorityQueue* queue, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < queue->size && CompareFrequency(queue->array[left], queue->array[smallest]) < 0) {
        smallest = left;
    }

    if (right < queue->size && CompareFrequency(queue->array[right], queue->array[smallest]) < 0) {
        smallest = right;
    }

    if (smallest != index) {
        ENode *temp = queue->array[index];
        queue->array[index] = queue->array[smallest];
        queue->array[smallest] = temp;
        HeapifyMin(queue, smallest);
    }
}

void DeleteFromQueue(PriorityQueue* queue) {
    queue->array[0] = queue->array[queue->size - 1];
    queue->size--;
    HeapifyMin(queue, 0);
}

//FIRST MAIN FUNCTION
ENode* BuildHuffmanTree(PriorityQueue* queue) {
    while (queue->size > 1) {
        // 2 Extract the two nodes with the lowest frequency
        ENode *left = queue->array[0];
        DeleteFromQueue(queue);
        ENode *right = queue->array[0];
        DeleteFromQueue(queue);

        // 3 Create a new node with the sum of the frequencies
        ENode *parent = NewNode(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        InsertInQueue(queue, parent);
    }
    // 4 Return the root node
    ENode* ans = queue->array[0];
    FreeQueue(queue);
    return ans;
}

//SECOND MAIN FUNCTION
void GenerateHuffmanCodes(ENode* root, char *code_storage, char **outcode, int length) {
    if (IsLeaf(root)) {
        outcode[root->symbol] = malloc(strlen(code_storage) + 1);
        strcpy(outcode[root->symbol], code_storage);
        return;
    }

    if (root->left) {
        code_storage[length] = '0';
        code_storage[length + 1] = '\0';
        GenerateHuffmanCodes(root->left, code_storage, outcode, length + 1);
    }

    if (root->right) {
        code_storage[length] = '1';
        code_storage[length + 1] = '\0';
        GenerateHuffmanCodes(root->right, code_storage, outcode, length + 1);
    }
}

//FREE functions
void FreeQueue(PriorityQueue *queue) {
    free(queue->array);
    free(queue);
}

void FreeTree(ENode *root) {
    if (root == NULL) {
        return;
    }
    
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

void FreeData(ENode *root, char *code_storage, char **outcode) {
    for (int i = 0; i < 256; ++i) {
        if (outcode[i] != NULL) {
            free(outcode[i]);
        }
    }
    free(outcode);
    
    free(code_storage);
    FreeTree(root);
}

// WRITE functions
void BitWrite(WriteInfo *info, char bit, FILE *out) {
    if (bit == '1') {
        info->byte |= (1 << (7 - (info->pos)));
    }
    // info->byte = (info->byte << 1) | (bit & '1');
    info->pos++;
    if (info->pos == 8) {
        fwrite(&(info->byte), sizeof(uchar), 1, out);
        info->pos = 0;
        info->byte = 0;
    }
}

void WriteTree(ENode *root, WriteInfo *info, FILE *out) {
    if (IsLeaf(root)) {
        BitWrite(info, '1', out);
        for (int i = 0; i < 8; ++i) {
            BitWrite(info, (((root->symbol >> (7 - i)) & 1)) + '0', out);
        }
    } else {
        BitWrite(info, '0', out);
        if (root->left) WriteTree(root->left, info, out);
        if (root->right) WriteTree(root->right, info, out);
    }
}

void WriteCodes(char **outcodes, WriteInfo *info, FILE *in, FILE *out) {
    fseek(in, 1, SEEK_SET);

    uchar* buffer = malloc(BUFFER_SIZE * sizeof(uchar));
    size_t length;
    while ((length = fread(buffer, sizeof(uchar), BUFFER_SIZE, in)) > 0) {
        for (size_t i = 0; i < length; i++) {
            uchar character = buffer[i];
            for (int j = 0; (unsigned long)j < strlen(outcodes[character]); ++j) {
                BitWrite(info, outcodes[character][j], out);
            }
        }
    }
    free(buffer);

    if (info->pos != 0) {
        fwrite(&(info->byte), sizeof(uchar), 1, out);
    }
}

void Encode(FILE *in, FILE *out) { 
    fseek(in, 1, SEEK_SET);

    PriorityQueue* queue = NewPriorityQueue(256);
    BuildFrequencyTable(in, queue);

    if (queue->size == 0) {
        FreeQueue(queue);
        return;
    }
    clock_t begin = clock();
    ENode *root = BuildHuffmanTree(queue); // Now Huffman tree is built with priority queue as input
    clock_t end = clock();
    double timer = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on building huffman tree: %f\n", timer);

    char *code_storage = calloc(256, sizeof(char));
    char **outcode = calloc(256, sizeof(char *));
    
    clock_t begin2 = clock();
    GenerateHuffmanCodes(root, code_storage, outcode, 0);
    clock_t end2 = clock();
    double timer2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on generating huffman codes: %f\n", timer2);
    if (code_storage == NULL) {
        FreeData(root, code_storage, outcode);
        return;
    }

    fwrite(&(root->frequency), sizeof(int), 1, out);
    
    WriteInfo info;
    info.byte = 0;
    info.pos = 0;

    clock_t begin3 = clock();
    WriteTree(root, &info, out);
    clock_t end3 = clock();
    double timer3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on writing tree: %f\n", timer3);
    
    clock_t begin4 = clock();
    WriteCodes(outcode, &info, in, out);
    clock_t end4 = clock();
    double timer4 = (double)(end4 - begin4) / CLOCKS_PER_SEC;
    fprintf(stderr, "time spent on writing data: %f\n", timer4);

    FreeData(root, code_storage, outcode);
}
