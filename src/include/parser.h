#ifndef CHR_PARSER_H
#define CHR_PARSER_H

#include "lexer.h"
#include "AST.h"
#include "token.h"

typedef struct PARSER_STRUCT {
    lexer_T *lexer;
    token_T *token;
} parser_T;

parser_T *initParser(lexer_T *lexer);

const char *tokenTypeToStr(int type);

token_T *parserEat(parser_T *parser, int type);

AST_T *parserParse(parser_T *parser);

AST_T *parserParseId(parser_T *parser);

AST_T *parserParseExpr(parser_T *parser);

AST_T *parserParseCompound(parser_T *parser);


#endif // CHR_PARSER_H
