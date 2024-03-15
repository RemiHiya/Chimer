#include "include/lexer.h"
#include "include/token.h"
#include "include/macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexer_T *initLexer(char *src) {
    lexer_T *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];

    return lexer;
}

void lexerAdvance(lexer_T *lexer) {
    if (lexer->i < lexer->src_size && lexer->c != '\0') {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

char lexerPeak(lexer_T *lexer, int offset) {
    return lexer->src[MIN(lexer->i + offset, lexer->src_size)];
}

token_T *lexerAdvanceWith(lexer_T *lexer, token_T *token) {
    lexerAdvance(lexer);
    return token;
}

token_T *lexerAdvanceCurrent(lexer_T *lexer, int type) {
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';

    token_T *token = initToken(value, type);
    lexerAdvance(lexer);
    return token;
}

void lexerSkipWhitespace(lexer_T *lexer) {
    while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t')
        lexerAdvance(lexer);
}

token_T *lexerParseId(lexer_T *lexer) {
    char *value = calloc(1, sizeof(char));
    while(isalnum(lexer->c)) {
        value = realloc(value, (strlen(value)+2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexerAdvance(lexer);
    }
    return initToken(value, TOKEN_ID);
}

token_T *lexerParseNumber(lexer_T *lexer) {
    char *value = calloc(1, sizeof(char));
    while(isdigit(lexer->c)) {
        value = realloc(value, (strlen(value)+2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexerAdvance(lexer);
    }
    return initToken(value, TOKEN_INT);
}

token_T *lexerNextToken(lexer_T *lexer) {
    while (lexer->c != '\0') {
        lexerSkipWhitespace(lexer);
        if (isalpha(lexer->c)) {
            return lexerParseId(lexer);
        }

        if (isdigit(lexer->c)) {
            return lexerParseNumber(lexer);
        }
        switch (lexer->c) {
            case '=': {
                if (lexerPeak(lexer, 1) == '>')
                    return lexerAdvanceWith(lexer, lexerAdvanceWith(lexer, initToken("=>", TOKEN_ARROW_RIGHT)));
                return lexerAdvanceWith(lexer, initToken("=", TOKEN_EQUALS));
            } break;
            case '(': return lexerAdvanceCurrent(lexer, TOKEN_LPAREN);
            case ')': return lexerAdvanceCurrent(lexer, TOKEN_RPAREN);
            case '{': return lexerAdvanceCurrent(lexer, TOKEN_LBRACE);
            case '}': return lexerAdvanceCurrent(lexer, TOKEN_RBRACE);
            case '[': return lexerAdvanceCurrent(lexer, TOKEN_LBRACKET);
            case ']': return lexerAdvanceCurrent(lexer, TOKEN_RBRACKET);
            case ':': return lexerAdvanceCurrent(lexer, TOKEN_COLON);
            case ',': return lexerAdvanceCurrent(lexer, TOKEN_COMMA);
            case '>': return lexerAdvanceCurrent(lexer, TOKEN_GT);
            case '<': return lexerAdvanceCurrent(lexer, TOKEN_LT);
            case ';': return lexerAdvanceCurrent(lexer, TOKEN_SEMI);
            case '\0': break;
            default: printf("[Lexer]: Unexpected character '%c'\n", lexer->c); exit(1); break;
        }
    }
    return initToken(0, TOKEN_EOF);
}
