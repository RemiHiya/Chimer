#include "include/lexer.h"
#include "include/token.h"
#include "include/macro.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexer_T *initLexer(char *src) {
    lexer_T *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->src = src;
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
    return init_token(value, TOKEN_ID);
}

token_T *lexerNextToken(lexer_T *lexer) {
    while (lexer->c != '\0') {
        if (isalnum(lexer->c)) {
            return lexerAdvanceWith(lexer, lexerParseId(lexer));
        }
        switch (lexer->c) {
            case '=': {
                if (lexerPeak(lexer, 1) == '>') return lexerAdvanceWith(lexer, init_token("=>", TOKEN_ARROW_RIGHT));
                return lexerAdvanceWith(lexer, init_token("=", TOKEN_EQUALS));
                break;
            }
        }
    }
    return init_token(0, TOKEN_EOF);
}
