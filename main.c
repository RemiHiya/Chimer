#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    SEMI,
    OPEN_PAREN,
    CLOSE_PAREN,
} TypeSeparator;

typedef enum {
    EXIT,
} TypeKeyword;

typedef enum {
    INT,
} TypeLiteral;

typedef struct {
    TypeKeyword type;
} TokenKeyword;

typedef struct {
    TypeSeparator type;
} TokenSeparator;

typedef struct {
    TypeLiteral type;
    int value;
} TokenLiteral;

TokenLiteral *generateNumber(char current, FILE *file) {
    TokenLiteral *token = malloc(sizeof(TokenLiteral));
    token->type = INT;
    char *value = malloc(sizeof(char) * 8);
    int valueIndex = 0;
    while (isdigit(current) && current != EOF) {
        if (!isdigit(current)) break;
        value[valueIndex] = current;
        ++valueIndex;
        current = fgetc(file);
    }
    ungetc(current, file);
    token->value = atoi(value);
    free(value);
    return token;
}

TokenKeyword *generateKeyword(char current, FILE *file) {
    TokenKeyword *token = malloc(sizeof(TokenKeyword));
    char *keyword = malloc(sizeof(char) * 8);
    int keywordIndex = 0;
    while (isalpha(current) && current != EOF) {
        keyword[keywordIndex] = current;
        current = fgetc(file);
        ++keywordIndex;
    }
    ungetc(current, file);
    if (strcmp(keyword, "exit") == 0) {
        printf("Exit found\n");
        token->type = EXIT;
    }
    printf("Found keyword : %s\n", keyword);
    free(keyword);
    return token;
}


void lexer(FILE *file) {
    char current = fgetc(file);

    while (current != EOF) {
        if (current == ';') {
            printf("Point virgule\n");
        }
        else if (current == '(') {
            printf("Parenthese ouverte\n");
        }
        else if (current == ')') {
            printf("Parenthese fermée\n");
        }
        else if (isdigit(current)) {
            TokenLiteral *tk = generateNumber(current, file);
            printf("Digit : %d\n", tk->value);
            //printf("FOUND DIGIT : %d\n", current-'0');
        }
        else if (isalpha(current)) {
            TokenKeyword *tk = generateKeyword(current, file);
            //printf("%s", tk-value);
        }
        current = fgetc(file);
    }
}


int main() {
    FILE *file;
    file = fopen("test.chr", "r");
    lexer(file);

    return 0;
}
