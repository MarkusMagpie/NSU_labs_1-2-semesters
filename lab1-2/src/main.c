#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LPS(char* ptrn, int pattern_len, int *text) {
    int i = 0, j = 0;
    text[0] = 0;
    for (i = 1, j = 0; i < pattern_len; ++i) {
        while ((j > 0) && (ptrn[i] != ptrn[j])) j = text[j - 1];
        if (ptrn[i] == ptrn[j]) {
            ++j;
        }
        text[i] = j;
    }
}

int KMP(char* pattern, char* text, int pattern_length, int text_length, int *lps, int plus, int j) {
    int i = 0;
    if (j != 0 && text[i] != pattern[j]) {
        printf("%.*s ", j, text + (i - j + plus));
        j = lps[j - 1];
    }
    while (i < text_length) {
        if (pattern[j] == text[i]) {
            ++j;
            ++i;
            if ((i <= pattern_length) && (j < pattern_length) && (pattern[j] != text[i])) {
                printf("%d %d ", i - j + 1 + plus, j);
                j = lps[j - 1];
            }
        }
        if (j == pattern_length && 2 * text_length - i >= pattern_length) {
            printf("%d %d ", i - j + 1 + plus, j);
            j = lps[j - 1];
        } else if (i < 2 * text_length && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }
    return j;
}

int main(void) {
    char pattern[17];
    for (unsigned int i = 0; i < sizeof(pattern); ++i) {
        pattern[i] = getchar();
        if (pattern[i] == '\n') {
            pattern[i] = '\0';
            break;
        }
    }
    int pattern_length = strlen(pattern);

    int lps[17];
    if (pattern_length) {
        LPS(pattern, pattern_length, lps);
        for (int i = 0; i < pattern_length; ++i) {
            printf("%d ", lps[i]);
        }
        printf("\n");
    }
    else {
        printf("\n");
        return 0;
    }

    int plus = 0;
    int j = 0;
    char text[2 * pattern_length + 1];
    int text_length = fread(text, 1, pattern_length, stdin);

    while (text_length == pattern_length) {
        text_length = fread(&text[pattern_length], 1, pattern_length, stdin);
        if (text_length < pattern_length) {
            text_length = pattern_length + text_length;
            j = KMP(pattern, text, pattern_length, text_length, lps, plus, j);
            break;
        }
        j = KMP(pattern, text, pattern_length, text_length, lps, plus, j);
        memmove(text, text + text_length, text_length);
        plus += text_length;
    }

    printf("\n");
    return 0;
}
