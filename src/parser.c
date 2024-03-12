#include "include/parser.h"
#include "include/AST.h"
#include "include/lexer.h"
#include "include/list.h"
#include "include/token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

parser_T *initParser(lexer_T *lexer) {
    parser_T *parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->token = lexerNextToken(lexer);
    return parser;
}


token_T *parserEat(parser_T *parser, int type) {
    if ((int) parser->token->type != type) {
        printf("[Parser]: Unexpected token: %s, was expecting %s\n", tokenToStr(parser->token), tokenTypeToStr(type));
        exit(1);
    }

    parser->token = lexerNextToken(parser->lexer);
    return parser->token;
}

AST_T *parserParse(parser_T *parser) {
    return parserParseCompound(parser);
}

AST_T *parserParseId(parser_T *parser) {
    char *value = calloc(strlen(parser->token->value)+1, sizeof(char));
    strcpy(value, parser->token->value);
    parserEat(parser, TOKEN_ID);

    if (parser->token->type == TOKEN_EQUALS) {
        // Parse assignement
        parserEat(parser, TOKEN_EQUALS);
        AST_T *ast = initAst(AST_ASSIGNEMENT);
        ast->name = value;
        ast->value = parserParseExpr(parser);
        return ast;
    }
    // Parse variable
    AST_T *ast = initAst(AST_VARIABLE);
    ast->name = value;
    return ast;
}

AST_T *parserParseList(parser_T *parser) {
    parserEat(parser, TOKEN_LPAREN);
    AST_T *compound = initAst(AST_COMPOUND);

    listPush(compound->children, parserParseExpr(parser));

    while (parser->token->type == TOKEN_COMMA) {
        parserEat(parser, TOKEN_COMMA);
        listPush(compound->children, parserParseExpr(parser));
    }
    parserEat(parser, TOKEN_RPAREN);

    return compound;
}

AST_T *parserParseExpr(parser_T *parser) {
    switch (parser->token->type) {
        case TOKEN_ID: return parserParseId(parser);
        case TOKEN_LPAREN: return parserParseList(parser);
        default :
            printf("[Parser]: Unexpected token %s\n", tokenToStr(parser->token));
            exit(1);
    }
}

AST_T *parserParseCompound(parser_T *parser) {
    AST_T *compound = initAst(AST_COMPOUND);
    while (parser->token->type != TOKEN_EOF) {
        listPush(compound->children, parserParseExpr(parser));
    }

    return compound;
}
