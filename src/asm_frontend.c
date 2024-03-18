#include "include/asm_frontend.h"
#include "include/AST.h"
#include "include/list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static AST_T *varLookup(list_T *list, const char *name) {

    for (int i=0; i< (int) list->size; ++i) {
        AST_T *childAst = (AST_T*) list->items[i];

        if (childAst->type != AST_VARIABLE || !childAst->name) {
            continue;
        }
        if (strcmp(childAst->name, name) == 0) {
            return childAst;
        }
    }
    return 0;
}

char *asmFCompound(AST_T *ast, list_T *list) {
    char *value = calloc(1, sizeof(char));

    for (int i=0; i<(int) ast->children->size; ++i) {
        AST_T *childAst = (AST_T*) ast->children->items[i];
        char *nextValue = asmF(childAst, list);
        value = realloc(value, (strlen(nextValue)+1) * sizeof(char));
        strcat(value, nextValue);
    }
    return value;
}

char *asmFAssignement(AST_T *ast, list_T *list) {
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

        for (unsigned int i=0; i<asmValue->children->size; ++i) {
            AST_T *farg = asmValue->children->items[i];
            AST_T *argVariable = initAst(AST_VARIABLE);
            argVariable->name = farg->name;
            argVariable->intValue = (4 * asmValue->children->size) - 4*i;
            listPush(list, argVariable);
        }

        char *asmValVal = asmF(asmValue->value, list);

        s = realloc(s, ((strlen(s) + strlen(asmValVal)) + 1) * sizeof(char));
        strcat(s, asmValVal);

    }
    return s;
}

char *asmFVariable(AST_T *ast, list_T *list) {
    char *s = calloc(1, sizeof(char));

    AST_T *var = varLookup(list, ast->name);

    if (!var) {
        printf("[Asm Frontend]: `%s` is not defined.\n", var->name);
        exit(1);
    }

    const char *template = "%d(%%esp)";
    s = realloc(s, (strlen(template) + 8) * sizeof(char));
    sprintf(s, template, var->intValue);
    return s;
}

char *asmFCall(AST_T *ast, list_T *list) {
    char *s = calloc(1, sizeof(char));

    if (strcmp(ast->name, "return") == 0) {
        AST_T *firstArg = ast->value->children->size ? ast->value->children->items[0] : 0;
        char *varS = calloc(3, sizeof(char));
        varS[0] = '$';
        varS[1] = '0';
        varS[2] = '\0';

        if (firstArg) {
            char *asVarS = asmF(firstArg, list);
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

char *asmFInt(AST_T *ast, list_T *list) {
    const char *template = "$%d";
    char *s = calloc(strlen(template) + 128, sizeof(char));
    sprintf(s, template, ast->intValue);
    return s;
}

char *asmFAccess(AST_T *ast, list_T *list) {
    AST_T *left = varLookup(list, ast->name);
    char *leftAs = asmF(left, list);
    AST_T *firstArg = ast->value->children->size ? ast->value->children->items[0] : 0;

    const char *template =
        "%s, %%eax\n"
        "movl %d(%%eax)";

    char *s = calloc(strlen(template) + strlen(leftAs) + 128, sizeof(char));
    sprintf(s, template, leftAs, 4 * (firstArg ? firstArg->intValue : 0));

    free(leftAs);

    return s;
}

char *asmFRoot(AST_T *ast, list_T *list) {
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

    char *nextValue = asmF(ast, list);
    value = realloc(value, (strlen(value) + strlen(nextValue) + 1) * sizeof(char));
    strcat(value, nextValue);

    return value;
}

char *asmF(AST_T *ast, list_T *list) {
    char *value = calloc(1, sizeof(char));
    char *nextValue = 0;

    switch (ast->type) {
        case AST_COMPOUND: nextValue = asmFCompound(ast, list); break;
        case AST_ASSIGNEMENT: nextValue = asmFAssignement(ast, list); break;
        case AST_VARIABLE: nextValue = asmFVariable(ast, list); break;
        case AST_CALL: nextValue = asmFCall(ast, list); break;
        case AST_INT: nextValue = asmFInt(ast, list); break;
        case AST_ACCESS: nextValue = asmFAccess(ast, list); break;
        default: printf("[Asm Frontend]: No frontend for AST of tpye %d.\n", ast->type); exit(1); break;
    }

    value = realloc(value, (strlen(nextValue)+1) * sizeof(char));
    strcat(value, nextValue);

    return value;
}
