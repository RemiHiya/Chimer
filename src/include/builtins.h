#ifndef CHR_BUILTINS
#define CHR_BUILTINS
#include "AST.h"
#include "visitor.h"

AST_T *fptrPrint(visitor_T *visitor, AST_T *node, list_T *list);
void builtinsInit(list_T *list);

#endif // CHR_BUILTINS
