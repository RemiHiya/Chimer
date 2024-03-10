#ifndef CHR_LEXER_H_
#define CHR_LEXER_H_
#include "token.h"
#include <stdio.h>

typedef struct LEXER_STRUCT {
    char *src;
    size_t src_size;
    char c;
    unsigned int i;
} lexer_T;

lexer_T *initLexer(char *src);

void lexerAdvance(lexer_T *lexer);
char lexerPeak(lexer_T *lexer, int offset);
token_T *lexerAdvanceWith(lexer_T *lexer, token_T *token);
void lexerSkipWhitespace(lexer_T *lexer);
token_T *lexerParseId(lexer_T *lexer);
token_T *lexerNextToken(lexer_T *lexer);

#endif // CHR_LEXER_H_
