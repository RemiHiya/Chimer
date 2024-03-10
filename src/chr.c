#include "include/chr.h"
#include "include/io.h"
#include "include/lexer.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>

void chrCompile(char *src) {
    lexer_T *lexer = initLexer(src);
    token_T *tok = 0;

    while ((tok=lexerNextToken(lexer))->type != TOKEN_EOF) {
        printf("TOK(%s) (%d)\n", tok->value, tok->type);
    }
}

void chrCompileFile(const char *filename) {
    printf("coucou111");
    char *src = chrReadFile(filename);
    printf("coucou");
    chrCompile(src);
    free(src);
}
