#include <stdio.h>

#include "Search.h"

void PrintNodeAndChildren(Node* root, char* Word, bool* is_found) {
    if (root == NULL) {
        return;
    }
    
    if (root->Flag) {
        *is_found = true;
        printf("%s ", Word);
    }
    PrintChildren(root->child, Word, is_found);
}

void Print(Node* root, char* Word, bool* is_found, char *case_type) {
    *is_found = true;
    int len_before = strlen(Word);
    Word = strcat(Word, root->key);
    if (root->Flag && strcmp(case_type, "root") == 0) {
        printf("%s ", Word);
    } else if (root->child != NULL && strcmp(case_type, "children") == 0) {
        PrintChildren(root->child, Word, is_found);
    }
    memset(Word + len_before, '\0', strlen(Word) - len_before);
}

void PrintChildren(Node* root, char* Word, bool* is_found) {
    if (root == NULL) {
        return;
    }
    
    if (root->Flag) {
        Print(root, Word, is_found, "root");
    }

    if (root->child == NULL && root->bro == NULL) {
        return;
    }

	// 1) have brothers -> check brothers
	// 2) have children -> 1) -> chech children
    if (root->bro != NULL) {
        *is_found = true;
        PrintChildren(root->bro, Word, is_found);
    }
    
    if (root->child != NULL) {
        Print(root, Word, is_found, "children");
    }
}
//https://habr.com/ru/articles/151421/
void Find(Node* root, char* prefix, char* Word, int WordLen, bool* is_found) {
    if (root == NULL) {
		return;
	}
    if (WordLen == 0) {
        WordLen = strlen(prefix) + 1;
    }
    
    int CurPrefix = PrefixLength(prefix, WordLen, root->key, root->len);
    // 1) CurPrefix == 0 -> search brothers
	// 2) CurPrefix == prefix length -> print all children
	// 3) CurPrefix == root->len -> search children
	if (CurPrefix == 0) {
        Find(root->bro, prefix, Word, WordLen, is_found);
    }
    if (CurPrefix == (int)strlen(prefix)) {
        Word = strcat(Word, root->key + CurPrefix);
        PrintNodeAndChildren(root, Word, is_found);
        return;
    }
    if (CurPrefix == root->len) {
        Find(root->child, prefix + CurPrefix, Word, WordLen - CurPrefix, is_found);
    }
}

void NoneOutput(bool *is_found) {
    if (*is_found == false) {
        printf("None");
    }
}
//prefix - "part of word" I am looking for in compressed Trie
void SearchForPrefix(Node* root, char* prefix) {
    if (root == NULL) {
        printf("None\n");
        return;
    }
	
    char *Word = malloc(MAX_WORD_LENGTH * sizeof(char));
    strcpy(Word, prefix);
    bool is_found = false;

    Find(root, prefix, Word, 0, &is_found);
    NoneOutput(&is_found);

    free(Word);
    printf("\n");
}
