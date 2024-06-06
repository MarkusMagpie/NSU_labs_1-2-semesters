#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;

void init_bcs(uchar *bcs, uchar *pattern, int pattern_length) {
    for (int i = 0; i < 256; ++i) {
        bcs[i] = pattern_length;
    }
    for (int i = 0; i < pattern_length - 1; ++i) {
        bcs[(int)pattern[i]] = pattern_length - 1 - i;
    }
}

int bmh(uchar *text, uchar *pattern, long int text_length, long int pattern_length, int page, uchar *bcs) {
    long int stop_symbol_text = text_length - 1;
    for (long int i = text_length - 1; i >= 0; --i) {
        printf("%ld ", i + 1 + page);
        if (text[i] != pattern[i]) {
            int output = bcs[(int)text[stop_symbol_text]];
            return output;
        }
    }
    return pattern_length;
}

int main(void) {
    uchar pattern[17];
    for (unsigned int i = 0; i < sizeof(pattern); ++i) {
        pattern[i] = getchar();
        if (pattern[i] == '\n') {
            pattern[i] = '\0';
            break;
        }
    }
    long int pattern_length = strlen((char *)pattern);

    int move_count = 0, page = 0;
    int final_page = 0;

    uchar *text = (uchar *)malloc(4 * pattern_length * sizeof(char));
    long int text_length = fread(text, 1, pattern_length, stdin);

    uchar bcs[256];
    init_bcs(bcs, pattern, pattern_length);

    while (text_length == pattern_length) {
        int move = 0;
        if ((final_page == 0) && (move_count >= pattern_length || move_count == 0)) {
            text_length = fread(&text[pattern_length], 1, pattern_length, stdin);
            move_count %= pattern_length;
        }
        if (text_length < pattern_length) {
            text_length = pattern_length;
            final_page = 1;
        }
        move = bmh(text, pattern, text_length, pattern_length, page, bcs);
        if (final_page == 1 && move == pattern_length) {
            break;
        }
        move_count += move;
        page += move;
        memmove(text, text + move, 2 * pattern_length - move);
    }

    free(text);
    printf("\n");
    return 0;
}
