#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // for 'strcspn' in main

#define OPERATION true // for struct Item 'is_operation'
#define NUMBER false

enum last_item {
    START_FILE,
    LAST_OPERATION,
    LAST_OPEN_BRACKET,
    LAST_CLOSE_BRACKET,
    LAST_NUMBER,
};

struct Item {
    int value;
    bool is_operation;
};

bool is_operation_char(char c) {
    return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

bool is_number(char c) {
    return (c >= '0' && c <= '9');
}

int operation_priority(char operation) {
    if (operation == '*' || operation == '/') return 1;
    else if (operation == '+' || operation == '-') return 0;
    return -1;
}

int stack_size(struct Item stack[]) {
    int i = 0;
    while (!stack[i].is_operation || stack[i].value != 0) { // not '\0'
        ++i;
    }
    return i;
}
// used for reverse polish notation, made them global for add_oper_to_stack
int stack_pos, operators_stack_len;

void add_oper_to_stack(char *operators_stack, struct Item *stack) {
    struct Item item = {operators_stack[operators_stack_len - 1], OPERATION};
    stack[stack_pos++] = item;
    --operators_stack_len;
}

void syntax_error(void) {
    printf("syntax error\n");
    exit(0);
}

void division_by_zero(void) {
    printf("division by zero\n");
    exit(0);
}

struct Item* rvrs_polish_notation(char *input, int input_len) {
    struct Item *output_stack = malloc(input_len * sizeof(struct Item));
    char *operators_stack = malloc(input_len * sizeof(char));

    int pos = 0;

    enum last_item last_item_type;
    last_item_type = START_FILE;

    while (input[pos] != '\0') {
        if (is_number(input[pos])) {
            if (last_item_type == LAST_CLOSE_BRACKET)
                syntax_error();
            int number = 0;
            while (is_number(input[pos])) {
                number = number * 10 + (input[pos] - '0');
                ++pos;
            }
            pos--;
            struct Item item = {number, NUMBER};
            output_stack[stack_pos++] = item;
            last_item_type = LAST_NUMBER;
        }
        else if (input[pos] == '(') {
            if (last_item_type == LAST_NUMBER) {
                syntax_error();
            }
            operators_stack[operators_stack_len++] = input[pos];
            last_item_type = LAST_OPEN_BRACKET;
        }
        else if (input[pos] == ')') {
            if (last_item_type == LAST_OPEN_BRACKET || last_item_type == LAST_OPERATION) {
                syntax_error();
            }
            bool open_bracket = false;
            while (operators_stack_len > 0) {
                if (operators_stack[operators_stack_len - 1] == '(') {
                    open_bracket = true;
                    break;
                }
                add_oper_to_stack(operators_stack, output_stack);
            }
            if (!open_bracket) {
                syntax_error();
            }
            --operators_stack_len;
            last_item_type = LAST_CLOSE_BRACKET;
        }
        else if (is_operation_char(input[pos])) {
            if (last_item_type == LAST_OPERATION || last_item_type == LAST_OPEN_BRACKET || last_item_type == START_FILE) {
                syntax_error();
            }
            if ((operators_stack_len > 0) && (operation_priority(input[pos]) <= operation_priority(operators_stack[operators_stack_len - 1]))) {
                add_oper_to_stack(operators_stack, output_stack);
            }
            operators_stack[operators_stack_len++] = input[pos];
            last_item_type = LAST_OPERATION;
        }
        else {
            syntax_error();
        }
        ++pos;
    }

    if (last_item_type == LAST_OPERATION || last_item_type == LAST_OPEN_BRACKET)
        syntax_error();

    while (operators_stack_len > 0) {
        add_oper_to_stack(operators_stack, output_stack);
    }

    free(operators_stack);
    
    struct Item item = {0, OPERATION};
    output_stack[stack_pos] = item;
    return output_stack;
}

void calc(struct Item stack[], int stack_len) {
    struct Item calculation[stack_len];
    int pos = 0;
    int a, b;

    for (int i = 0; i < stack_len; i++) {
        if (!stack[i].is_operation) {
            calculation[pos++].value = stack[i].value;
        }
        else {
            b = calculation[--pos].value;
            a = calculation[--pos].value;
            if (stack[i].value == '*') {
                calculation[pos++].value = a * b;
            } else if (stack[i].value == '/') {
                if (b == 0) {
                    division_by_zero();
                }
                calculation[pos++].value = a / b;
            } else if (stack[i].value == '+') {
                calculation[pos++].value = a + b;
            } else if (stack[i].value == '-') {
                calculation[pos++].value = a - b;
            }
        }
    }
    printf("%d\n", calculation[0].value);
}

int main(void) {
    char input[1001];
    //fgets(input, sizeof(input), stdin);
    //input[strcspn(input, "\n")] = '\0';

    for (unsigned int i = 0; i < sizeof(input); i++) {
        input[i] = getchar();
        if (input[i] == EOF || input[i] == '\n') {
            input[i] = '\0';
            break;
        }
    }

    struct Item *stack = rvrs_polish_notation(input, sizeof(input));
    if ((stack[0].is_operation) && (stack[0].value == 0)) {
        syntax_error();
    }
    calc(stack, stack_size(stack));

    free(stack);
    return 0;
}
