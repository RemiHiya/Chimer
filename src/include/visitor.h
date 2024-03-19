#ifndef CHR_VISITOR_H
#define CHR_VISITOR_H
#include "AST.h"
#include "list.h"

typedef struct VISITOR_STRUCT {
    AST_T *object;
} visitor_T;

visitor_T *initVisitor();

AST_T *visitorVisit(visitor_T *visitor, AST_T *node, list_T *list);

AST_T *visitorVisitCompound(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitAssignement(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitVariable(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitCall(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitInt(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitString(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitAccess(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitorVisitFunction(visitor_T *visitor, AST_T *node, list_T *list);

#endif // CHR_VISITOR_H
