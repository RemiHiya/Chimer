#include "include/builtins.h"
#include "include/AST.h"
#include "include/list.h"
#include "include/utils.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

AST_T *fptrPrint(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *ast = initAst(AST_STRING);

    AST_T *firstArg = list->size ? list->items[0] : 0;
    char *instr = calloc(128, sizeof(char));
    char *hexstr = 0;
    int nrChunks = 0;

    if (firstArg) {
        if (firstArg->type == AST_STRING) {
            instr = firstArg->stringValue;
        }
        else if (firstArg->type == AST_INT) {
            instr = calloc(128, sizeof(char));
            sprintf(instr, "%d", firstArg->intValue);
        }

        char **chunks = strToHexChunk(instr, &nrChunks);

        char *strpush = calloc(1, sizeof(char));
        const char *pushtemplate = "pushl $0x%s\n";

        for (int i=nrChunks-1; i>=0; --i){
            char *pushhex = chunks[i];
            char *push = calloc(strlen(pushhex) + strlen(pushtemplate) + 1, sizeof(char));
            sprintf(push, pushtemplate, pushhex);
            strpush = realloc(strpush, (strlen(strpush)+strlen(push)+1) * sizeof(char));
            strcat(strpush, push);
        }

        hexstr = strpush;
    }

    const char *template =
        "movl $4, %%eax\n" // syscall write
        "movl $1, %%ebx\n" // stdout
        "%s\n" // buffer
        "movl %%esp, %%ecx\n"
        "addl $%d, %%esp\n"
        "movl $%d, %%edx\n" // size
        "int $0x80\n";

    char *asmb = calloc((hexstr ? strlen(hexstr) : 0) + strlen(template) + 1, sizeof(char));
    sprintf(asmb, template, hexstr ? hexstr : "$0", strlen(instr) * 2, nrChunks * 4);
    ast->stringValue = asmb;
    free(hexstr);
    return ast;
}

void builtinsRegisterFptr(list_T *list, const char *name,
                          AST_T *(*fptr)(visitor_T *visitor, AST_T *node, list_T *list)) {
    AST_T *fptrPrintVar = initAst(AST_VARIABLE);
    fptrPrintVar->name = mkstr(name);
    fptrPrintVar->fptr = fptr;

    listPush(list, fptrPrintVar);
}

void builtinsInit(list_T *list) {
    builtinsRegisterFptr(list, "print", fptrPrint);
}
