#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void bad_input() { 
    puts("bad input\n");
    exit(0);
}

void swap(char *a, char *b) {
    char t = *a;
    *a = *b;
    *b = t;
}

int isValidInput(char arr[], int str_len) {
    int list[10] = {0};
    for (int i = 0; i < str_len; ++i) {
        //fprintf(stderr, "%d\n", list[arr[i] - '0']);
        if (arr[i] < '0' || arr[i] > '9' || arr[i] == ' ') return 1;
        if (list[arr[i] - '0'] != 0) {
            //fprintf(stderr, "!!!%d\n", list[arr[i] - '0']);
            return 1;
        }
        list[arr[i] - '0'] = 1;
    }
    return 0;
}

int ReverseString(int l, int i, char *str) {
    for (int j = l - 1; i < j; --j) {
        swap(&str[i], &str[j]);
        ++i;
    }
    return i;
}

//https://habr.com/ru/articles/428552/
char *nextpermutation(char *str) {
    int l = strlen(str);
    char *r = NULL;
    for (int i = l - 1; i > 0; --i) {
        if (str[i - 1] < str[i]) {
            int pivot = i;
            for (int j = pivot - (pivot - i); j < l; ++j) {
                if (str[j] <= str[pivot] && str[i - 1] < str[j]) {
                    pivot = j;
                }
            }
            swap(&str[i - 1], &str[pivot]);
            ReverseString(l, i, str);
            r = str;
            break;
        }
    }
    return r;
}

int main(void) {
    char line[12];
    int str_len = 0;
    for (int i = 0; i < 11; ++i) {
        line[i] = getchar();
        if (line[i] == '\n') {
            line[i] = '\0';
            break;
        }
    }
    str_len = strlen(line);
    if (strlen(line) >= 11) {
        bad_input();
    }
    int change; 
    if (scanf("%d", &change) != 1){
        bad_input();
    }
    if (isValidInput(line, str_len)){
        bad_input();
    }
    for (int i = 0; i < change; ++i) {
        char *res = nextpermutation(line);
        if (res == NULL) {
            break;
        }
        printf("%s\n", res);
    }
    return 0;
}
