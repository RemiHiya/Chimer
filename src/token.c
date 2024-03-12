#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_T* initToken(char* value, int type) {
    token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->value = value;
    token->type = type;

    return token;
}

const char *tokenTypeToStr(int type) {
    switch (type) {
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_EQUALS: return "TOKEN_EQUALS";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_ARROW_RIGHT: return "TOKEN_ARROW_RIGHT";
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_SEMI: return "TOKEN_SEMI";
        case TOKEN_EOF: return "TOKEN_EOF";
    }
    return "not_stringable";
}

char *tokenToStr(token_T *token) {
    const char *typeStr = tokenTypeToStr(token->type);
    const char *template = "<type=%s, int_type=%d, value=`%s`>";

    char *str = calloc(strlen(typeStr) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, typeStr, token->type, token->value);

    return str;
}
