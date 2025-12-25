#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUM,
    TOKEN_ID,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_PAREN,
    TOKEN_COMMA,
    TOKEN_ERROR
} TokenType;

typedef enum{
    NUM,
    ID,
    OPERATOR_SUM,
    OPERATOR_SUB,
    OPERATOR_MUL,
    OPERATOR_DIV,
    OPERATOR_MOD,
    PAREN_OPEN,
    PAREN_CLOSE,
    ASSIGN,
    COMMA,
    NONE
} TokenValue;

typedef struct Token{
    char lexeme[32];
    TokenType tokenType;
    TokenValue tokenValue;
    float variableValue;
} Token;

#endif