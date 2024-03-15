#include "include/parser.h"
#include "include/AST.h"
#include "include/lexer.h"
#include "include/list.h"
#include "include/token.h"
#include "include/types.h"
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

    if (parser->token->type == TOKEN_COLON) {
        parserEat(parser, TOKEN_COLON);

        while (parser->token->type == TOKEN_ID) {
            ast->data_type = typeNameToInt(parser->token->value);
            parserEat(parser, TOKEN_ID);
            if (parser->token->type == TOKEN_LT) {
                parserEat(parser, TOKEN_LT);
                ast->data_type += typeNameToInt(parser->token->value);
                parserEat(parser, TOKEN_ID);
                parserEat(parser, TOKEN_GT);
            }
        }
    }
    else {
        if (parser->token->type == TOKEN_LPAREN) {
            ast->type = AST_CALL;
            ast->value = parserParseList(parser);
        }
        else if (parser->token->type == TOKEN_LBRACKET) {
            ast->type = AST_ACCESS;
            ast->value = parserParseList(parser);
        }
    }

    return ast;
}

AST_T *parserParseBlock(parser_T *parser) {
    parserEat(parser, TOKEN_LBRACE);

    AST_T *ast = initAst(AST_COMPOUND);

    while (parser->token->type != TOKEN_RBRACE) {
        listPush(ast->children, parserParseExpr(parser));
    }
    parserEat(parser, TOKEN_RBRACE);

    return ast;
}

AST_T *parserParseList(parser_T *parser) {
    unsigned int isBracket = parser->token->type == TOKEN_LBRACKET;
    parserEat(parser, isBracket ? TOKEN_LBRACKET : TOKEN_LPAREN);
    AST_T *ast = initAst(AST_COMPOUND);

    listPush(ast->children, parserParseExpr(parser));

    while (parser->token->type == TOKEN_COMMA) {
        parserEat(parser, TOKEN_COMMA);
        listPush(ast->children, parserParseExpr(parser));
    }
    parserEat(parser, isBracket ? TOKEN_RBRACKET : TOKEN_RPAREN);

    if (parser->token->type == TOKEN_COLON) {
        parserEat(parser, TOKEN_COLON);

        while (parser->token->type == TOKEN_ID) {
            ast->data_type = typeNameToInt(parser->token->value);
            parserEat(parser, TOKEN_ID);
            if (parser->token->type == TOKEN_LT) {
                parserEat(parser, TOKEN_LT);
                ast->data_type += typeNameToInt(parser->token->value);
                parserEat(parser, TOKEN_ID);
                parserEat(parser, TOKEN_GT);
            }
        }
    }

    // If there is an arrow => it's a function
    if (parser->token->type == TOKEN_ARROW_RIGHT) {
        parserEat(parser, TOKEN_ARROW_RIGHT);
        ast->type = AST_FUNCTION;
        ast->value = parserParseCompound(parser);
    }

    return ast;
}

AST_T *parserParseInt(parser_T *parser) {
    int intValue = atoi(parser->token->value);
    parserEat(parser, TOKEN_INT);

    AST_T *ast = initAst(AST_INT);
    ast->intValue = intValue;
    return ast;
}

AST_T *parserParseExpr(parser_T *parser) {
    switch (parser->token->type) {
        case TOKEN_ID: return parserParseId(parser);
        case TOKEN_LPAREN: return parserParseList(parser);
        case TOKEN_INT: return parserParseInt(parser);
        default :
            printf("[Parser]: Unexpected token %s\n", tokenToStr(parser->token));
            exit(1);
    }
}

AST_T *parserParseCompound(parser_T *parser) {
    unsigned int shouldClose = 0;
    if (parser->token->type == TOKEN_LBRACE){
        parserEat(parser, TOKEN_LBRACE);
        shouldClose = 1;
    }
    AST_T *compound = initAst(AST_COMPOUND);
    while (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_RBRACE) {
        listPush(compound->children, parserParseExpr(parser));

        if (parser->token->type == TOKEN_SEMI) {
            parserEat(parser, TOKEN_SEMI);
        }
    }

    if (shouldClose) {
        parserEat(parser, TOKEN_RBRACE);
    }

    return compound;
}
