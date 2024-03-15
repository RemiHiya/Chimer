#include "include/asm_frontend.h"
#include "include/AST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *asmFCompound(AST_T *ast) {
    char *value = calloc(1, sizeof(char));

    for (int i=0; i<(int) ast->children->size; ++i) {
        AST_T *childAst = (AST_T*) ast->children->items[i];
        char *nextValue = asmF(childAst);
        value = realloc(value, (strlen(nextValue)+1) * sizeof(char));
        strcat(value, nextValue);
    }
    return value;
}

char *asmFAssignement(AST_T *ast) {
    char *s = calloc(1, sizeof(char));

    if (ast->value->type == AST_FUNCTION) {
        const char *template =
            ".globl %s\n"
            "%s:\n"
            "pushl %%ebp\n"
            "movl %%esp, %%ebp\n";
        s = realloc(s, (strlen(template) + 2*strlen(ast->name) + 1) * sizeof(char));
        sprintf(s, template, ast->name, ast->name);

        AST_T *asmValue = ast->value;
        char *asmValVal = asmF(asmValue->value);

        s = realloc(s, ((strlen(s) + strlen(asmValVal)) + 1) * sizeof(char));
        strcat(s, asmValVal);

    }
    return s;
}

char *asmFVariable(AST_T *ast, int id) {
    char *s = calloc(1, sizeof(char));
    if (ast->type == AST_INT) {
        const char *template = "$%d";
        s = realloc(s, (strlen(template) + 256) * sizeof(char));
        sprintf(s, template, ast->intValue);
    }
    else {
        const char *template = "%d(%%esp)";
        s = realloc(s, (strlen(template) + 8) * sizeof(char));
        sprintf(s, template, id);
    }
    return s;
}

char *asmFCall(AST_T *ast) {
    char *s = calloc(1, sizeof(char));

    if (strcmp(ast->name, "return") == 0) {
        AST_T *firstArg = ast->value->children->size ? ast->value->children->items[0] : 0;
        char *varS = calloc(3, sizeof(char));
        varS[0] = '$';
        varS[1] = '0';
        varS[2] = '\0';

        if (firstArg && firstArg->type == AST_VARIABLE) {
            char *asVarS = asmFVariable(firstArg, 8);
            varS = realloc(varS, (strlen(asVarS) + 1)* sizeof(char));
            strcpy(varS, asVarS);
            free(asVarS);
        }
        const char *template =
            "movl %s, %%eax\n"
            "movl %%ebp, %%esp\n"
            "popl %%ebp\n"
            "ret\n";
        char *retS = calloc(strlen(template) + 128, sizeof(char));
        sprintf(retS, template, varS);
        s = realloc(s, (strlen(retS) + 1) * sizeof(char));
        strcat(s, retS);
    }
    return s;
}

char *asmFInt(AST_T *ast) {

}

char *asmFAccess(AST_T *ast) {
    char *s = calloc(1, sizeof(char));
    //AST_T *firstArg = ast->value->children->size ? ast->value->children->items[0] : 0;
    return s;
}

char *asmFRoot(AST_T *ast) {
    const char *sectionText =
        ".section .text\n"
        ".globl _start\n"
        "_start:\n"
        "pushl 0(\%esp)\n"
        "call main\n"
        "addl $4, \%esp\n"
        "movl \%eax, \%ebx\n"
        "movl $1, \%eax\n"
        "int $0x80\n\n";

    char *value = calloc(strlen(sectionText) + 128, sizeof(char));
    strcat(value, sectionText);

    char *nextValue = asmF(ast);
    value = realloc(value, (strlen(value) + strlen(nextValue) + 1) * sizeof(char));
    strcat(value, nextValue);

    return value;
}

char *asmF(AST_T *ast) {
    char *value = calloc(1, sizeof(char));
    char *nextValue = 0;

    switch (ast->type) {
        case AST_COMPOUND: nextValue = asmFCompound(ast); break;
        case AST_ASSIGNEMENT: nextValue = asmFAssignement(ast); break;
        case AST_VARIABLE: nextValue = asmFVariable(ast, 0); break;
        case AST_CALL: nextValue = asmFCall(ast); break;
        case AST_INT: nextValue = asmFInt(ast); break;
        case AST_ACCESS: nextValue = asmFAccess(ast); break;
        default: printf("[Asm Frontend]: No frontend for AST of tpye %d.\n", ast->type); exit(1); break;
    }

    value = realloc(value, (strlen(nextValue)+1) * sizeof(char));
    strcat(value, nextValue);

    return value;
}
