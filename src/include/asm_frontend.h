#ifndef CHR_ASM_FRONTEND
#define CHR_ASM_FRONTEND
#include "AST.h"
#include "list.h"

char *asmF(AST_T *ast, list_T *list);
char *asmFRoot(AST_T *ast, list_T *list);

#endif // CHR_ASM_FRONTEND
