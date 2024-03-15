#ifndef CHR_AST_H
#define CHR_AST_H
#include "list.h"

typedef struct AST_STRUCT {
    enum {
        AST_COMPOUND,
        AST_FUNCTION,
        AST_CALL,
        AST_ASSIGNEMENT,
        AST_TYPE_DEFINITION,
        AST_VARIABLE,
        AST_STATEMENT,
        AST_ACCESS,
        AST_NOOP,
        AST_INT,
        AST_TYPE,
    } type;
    list_T *children;
    char *name;
    struct AST_STRUCT *value;
    int intValue;
    int data_type;
} AST_T;

AST_T *initAst(int type);

#endif // CHR_AST_H
