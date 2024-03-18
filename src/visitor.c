#include "include/AST.h"
#include "include/builtins.h"
#include "include/list.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

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

visitor_T *initVisitor() {
    visitor_T *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->object = initAst(AST_COMPOUND);
    builtinsInit(visitor->object->children);
    return visitor;
}

AST_T *visitorVisit(visitor_T *visitor, AST_T *node, list_T *list) {
    switch (node->type) {
        case AST_COMPOUND: return visitorVisitCompound(visitor, node, list); break;
        case AST_ASSIGNEMENT: return visitorVisitAssignement(visitor, node, list); break;
        case AST_VARIABLE: return visitorVisitVariable(visitor, node, list); break;
        case AST_CALL: return visitorVisitCall(visitor, node, list); break;
        case AST_INT: return visitorVisitInt(visitor, node, list); break;
        case AST_ACCESS: return visitorVisitAccess(visitor, node, list); break;
        case AST_FUNCTION: return visitorVisitFunction(visitor, node, list); break;
        default: printf("[Visitor]: Don't know how to handle AST of tpye %d.\n", node->type); exit(1); break;
    }
    return node;
}

AST_T *visitorVisitCompound(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *compound = initAst(AST_COMPOUND);
    for (unsigned int i=0; i<node->children->size; ++i) {
        listPush(compound->children, visitorVisit(visitor, node->children->items[i], list));
    }
    return compound;
}

AST_T *visitorVisitAssignement(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *newVar = initAst(AST_VARIABLE);
    newVar->name = node->name;
    newVar->value = visitorVisit(visitor, node->value, list);
    listPush(list, newVar);
    return newVar;
}

AST_T *visitorVisitVariable(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *var = varLookup(visitor->object->children, node->name);
    if (var)
        return var;

    return node;
}

AST_T *visitorVisitFunction(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *compound = initAst(AST_COMPOUND);
    for (unsigned int i=0; i<node->value->children->size; ++i) {
        listPush(compound->children, visitorVisit(visitor, node->value->children->items[i], list));
    }
    return compound;
}

AST_T *visitorVisitCall(visitor_T *visitor, AST_T *node, list_T *list) {
    AST_T *var = varLookup(visitor->object->children, node->name);

    if (var) {
        if (var->fptr)
            return var->fptr(visitor, var, node->value->children);
    }
    return node;
}

AST_T *visitorVisitInt(visitor_T *visitor, AST_T *node, list_T *list) {
    return node;
}

AST_T *visitorVisitAccess(visitor_T *visitor, AST_T *node, list_T *list) {
    return node;
}
