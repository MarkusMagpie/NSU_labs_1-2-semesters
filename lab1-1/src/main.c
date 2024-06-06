#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int hash_pattern(char *Pattern, int len) {
	int pattern_hash = 0;
	int mult = 1;
	for (int i = 0; i < len; ++i) {
		pattern_hash += ((unsigned int)Pattern[i] % 3) * mult;
		mult *= 3;
	}
	return pattern_hash;
}

int mult_count(int len) {
	int mult = 1;
	for (int i = 0; i < len; ++i) mult *= 3;
	return mult;
}

int rabin_karp(char *Text, char *Pattern, int pattern_len, int text_len, int ptrn_hsh, int string_hash, int all_len, int mult) {
	for (int i = 0; i < text_len; ++i) {
		if (string_hash == ptrn_hsh) {
			for (int j = 0; j < pattern_len; ++j) {
				printf(" %d", i + j + 1 + all_len);
				if (Pattern[j] != Text[i + j]) {
					break;
				}
			}
		}
		string_hash = (((string_hash - ((unsigned int)Text[i]) * 0) / 3) + (((unsigned int)Text[i + pattern_len]) % 3) * mult);	
	}
	return string_hash;
}

int main(void) {
	char ptrn[17];
	int pattern_len = 0;
	for (int i = 0; i < 17; ++i) {
		ptrn[i] = getchar();
		if (ptrn[i] == '\n') {
			ptrn[i] = '\0';
			break;
		}
	}
	pattern_len = strlen(ptrn);

	int ptrn_hsh = hash_pattern(ptrn, pattern_len);
	printf("%d", ptrn_hsh);

	char text[2 * pattern_len];
	int text_len = fread(text, 1, pattern_len, stdin);

	int string_hsh = hash_pattern(text, pattern_len);
	int mult = mult_count(pattern_len - 1);
	
	int all_len = 0;
	while (text_len == pattern_len) {
		text_len = fread(&text[pattern_len], 1, pattern_len, stdin);
		if (text_len < pattern_len) ++text_len;
		if (text[text_len + pattern_len - 1] != '\n') {
			string_hsh = rabin_karp(text, ptrn, pattern_len, text_len, ptrn_hsh, string_hsh, all_len, mult);
			for (int i = 0; i < pattern_len; ++i) {
				text[i] = text[pattern_len+i];
			}
		}
		all_len += text_len;
	}
	printf("\n");
	return 0;
}
