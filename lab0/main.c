#include <stdio.h>
#include <string.h>
#include <ctype.h>

// перевод в число
int to_int(char n) {
    int smol = tolower(n);
    if(n == '.') return -1;
    else if(smol >= 'a' && smol <= 'f') return (smol - 'a' + 10);
    else if (isdigit(n)) return (n - '0');
    else return 10;
}
// перевод в строку
char to_str(long long int n) {
    if (n >= 0 && n <= 9) return (char)(n + '0');
    else return (char)(n - 10 + 'A');
}
// перевод в b2 для целой части
void print_int_part(int b2, long in) {
    int len = 0;
    long long num = in;
    char out[128];
    if (in == 0) {
        printf("0");
        return;
    }
    while(num) {
        out[len++] = to_str(num % b2);
        num /= b2;
    }
    for(int i = len-1; i >= 0; --i) {
        printf("%c", out[i]);
    }
}
// перевод в b2 для дробной части
void print_fract_part(int b2, long long n, long long d) {
    printf(".");
    for (int i  = 0; i < 12 && n != 0; ++i) {
        n *= b2;
        int res = n / d;
        n -= d*res;
        printf("%c", to_str(res));
    }
}

int main(void) {
    int b1;
    int b2;
    char str[128];
    // dot_loc локация точки, len_after_dot длина дробной части
    int dot_loc = 0;
    int len_after_dot = 0;
    // сюда кладем целую и дробную часть в 10 сс
    long int cel = 0;
    long long drob_numer = 0, drob_denom = 1;
    if(scanf("%d%d", &b1, &b2) != 2 || (b1 < 2 || b1 > 16) || (b2 < 2 || b2 > 16)) {
        printf("bad input\n");
        return 0;
    }
    // вводим второе число в str
    if (scanf("%13s", str) != 1) {
        printf("bad input\n");
        return 0;
    }
    
    for(unsigned int i = 0; i < strlen(str); i++) {
        if(str[i] == '.') { 
            if(dot_loc != 0) { // если больше 1 точки - плохо
                printf("bad input\n");
                return 0;
            }
            dot_loc = i; //локация точки
            len_after_dot = strlen(str) - i - 1; // длина дробной части
            if(len_after_dot == 0 || dot_loc == 0) { // точка в начале или в конце - плохо
                printf("bad input\n");
                return 0;
            }
        }
        else if(to_int(str[i]) >= b1) {
            printf("bad input\n");
            return 0;
        }
    }
    // если нет точек, локация точки равна длине массива
    if(dot_loc == 0) {
        dot_loc = strlen(str);
    }
    for(int i = 0; i < dot_loc; ++i) {
        cel += to_int(str[dot_loc-i-1]) * drob_denom;
        drob_denom *= b1;
    }
    drob_denom = 1;
    for(int i = 1; i <= len_after_dot; ++i) {
        drob_numer += to_int(str[strlen(str)-i]) * drob_denom;
        drob_denom *= b1;
    }
    print_int_part(b2, cel); 
    if(drob_numer != 0) {
        print_fract_part(b2, drob_numer, drob_denom);
    }
    printf("\n");
    return 0;
}
