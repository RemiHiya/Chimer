#include "include/builtins.h"
#include "include/AST.h"
#include "include/list.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

static char *mkstr(const char *str) {
    char *outstr = calloc(strlen(str) + 1, sizeof(char));
    strcpy(outstr, str);
    return outstr;
}


AST_T *fptrPrint(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *ast = initAst(AST_STRING);

    const char *template =
        "movl $4, \%eax\n" // syscall write
        "movl $1, \%ebx\n" // stdout
        "movl $0, \%ecx\n" // buffer
        "movl $0, \%edx\n" // size
        "int $0x80\n";

    ast->stringValue = mkstr(template);
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
