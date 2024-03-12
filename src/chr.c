#include "include/chr.h"
#include "include/AST.h"
#include "include/io.h"
#include "include/lexer.h"
#include "include/token.h"
#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>

void chrCompile(char *src) {
    lexer_T *lexer = initLexer(src);
    parser_T *parser = initParser(lexer);
    AST_T *root = parserParse(parser);
    token_T *tok = 0;

    while ((tok = lexerNextToken(lexer))->type != TOKEN_EOF) {
        printf("%s\n", tokenToStr(tok));
    }

}

void chrCompileFile(const char *filename) {
    char *src = chrReadFile(filename);
    chrCompile(src);
    free(src);
}
