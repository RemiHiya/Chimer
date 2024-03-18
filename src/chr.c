#include "include/asm_frontend.h"
#include "include/AST.h"
#include "include/chr.h"
#include "include/io.h"
#include "include/lexer.h"
#include "include/list.h"
#include "include/parser.h"
#include "include/token.h"
#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char *sh(const char *cmd) {
    char *output = (char*) calloc(1, sizeof(char));
    output[0] = '\0';

    FILE *fp;
    char path[1035];

    fp = popen(cmd, "r");

    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        output = realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
        strcat(output, path);
    }
    pclose(fp);
    return output;
}

void chrCompile(char *src) {
    lexer_T *lexer = initLexer(src);
    parser_T *parser = initParser(lexer);
    AST_T *root = parserParse(parser);

    visitor_T *visitor = initVisitor();
    visitorVisit(visitor, root, initList(sizeof(struct AST_STRUCT)));

    return;
    char *s = asmFRoot(root, initList(sizeof(struct AST_STRUCT*)));

    chrWriteFile("a.s", s);
    sh("as --32 a.s -o a.o");
    sh("ld a.o -o a.out -m elf_i386");
}

void chrCompileFile(const char *filename) {
    char *src = chrReadFile(filename);
    chrCompile(src);
    free(src);
}
