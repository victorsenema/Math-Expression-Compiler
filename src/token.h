#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUM,
    TOKEN_ID,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_PAREN,
    TOKEN_COMMA,
    TOKEN_ERROR,
    TOKEN_END_EXPRESSION,
    TOKEN_EOF
} TokenType;

typedef enum{
    NUM,
    ID,
    ID_IMMUTABLE,
    ID_FUNCTION,
    OPERATOR_SUM,
    OPERATOR_SUB,
    OPERATOR_MUL,
    OPERATOR_DIV,
    OPERATOR_POT,
    OPERATOR_MOD,
    PAREN_OPEN,
    PAREN_CLOSE,
    ASSIGN,
    COMMA,
    END,
    ENDOFFILE,
    NONE
} TokenValue;

typedef struct Token{
    char lexeme[32];
    TokenType tokenType;
    TokenValue tokenValue;
    float variableValue;
} Token;

#endif