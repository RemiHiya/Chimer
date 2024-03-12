#include "include/AST.h"
#include "include/list.h"
#include <stdlib.h>


AST_T *initAst(int type) {
    AST_T *ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    if (type == AST_COMPOUND)
        ast->children = initList(sizeof(struct AST_STRUCT*));

    return ast;
}
