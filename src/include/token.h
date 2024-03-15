#ifndef CHR_TOKEN_H
#define CHR_TOKEN_H

typedef struct TOKEN_STRUCT {
    char *value;
    enum {
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_ARROW_RIGHT,
    TOKEN_INT,
    TOKEN_SEMI,
    TOKEN_EOF,
} type;
} token_T;

token_T *initToken(char* value, int type);

char *tokenToStr(token_T *token);

#endif // CHR_TOKEN_H
