//
/*
FOR THE FUTURE ME:
To make the parser it need to analyse the LL1 production, the first and the follow:
if my production generate only other productions, withou looking for any terminal whe just execute the other productions:
Example: <expressionE>  → <expressionT> <expressionE'>
         This productions will only call the 2 other ones, no need to check first and follow
But if the productions uses a terminal it needs to check the first and them call the other production.
Example: (every time a FIRST matches THE TOKEN NEEDS TO BE CONSUMED)
<expressionF>  → '(' <expressionE> ')'
              | id
              | num
              | function '(' <expressionE> ',' <expressionE> ')'
FIRST(<expressionF>) = { '(', id, num, function }
chekc for '(', call expression E and check for ')':
or check for id | num | function, (, call expressionE, , , call expression E, );
Follow is just used when I have an ε:
Example:
<expressionP'> → '^' <expressionP> | ε
FIRST(<expressionP'>) = { '^', ε }
FOLLOW(<expressionP'>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
check the first and call expressionP, them if the production dindt return, check for every follow, if no one is nextToken we have an error.
*/
/*
LL1:
<program> → <statement_list>
<statement_list> → <statement> <statement_list>
                 | ε
<statement> → <declaration_variable>
            | <print>
<print> → function_print '(' 'expression ')' ';'
<declaration_variable> → id '=' <expressionE> ';'
<expressionE>  → <expressionT> <expressionE'>
<expressionE'> → '+' <expressionT> <expressionE'>
              | '-' <expressionT> <expressionE'>
              | ε
<expressionT>  → <expressionP> <expressionT'>
<expressionT'> → '*' <expressionP> <expressionT'>
              | '/' <expressionP> <expressionT'>
              | '%' <expressionP> <expressionT'>
              | ε
<expressionP>  → <expressionF> <expressionP'>
<expressionP'> → '^' <expressionP> | ε
<expressionF>  → '(' <expressionE> ')'
              | id
              | num
              | function '(' <expressionE> ',' <expressionE> ')'
*/
/*
FIRST(<program>) = { id, function_print, ε }
FIRST(<statement_list>) = { id, function_print, ε }
FIRST(<statement>) = { id, function_print }
FIRST(<print>) = { function_print }
FIRST(<declaration_variable>) = { id }
FIRST(<expressionE>) = { '(', id, num, function }
FIRST(<expressionE'>) = { '+', '-', ε }
FIRST(<expressionT>) = { '(', id, num, function }
FIRST(<expressionT'>) = { '*', '/', '%', ε }
FIRST(<expressionP>) = { '(', id, num, function }
FIRST(<expressionP'>) = { '^', ε }
FIRST(<expressionF>) = { '(', id, num, function }
*/
/*
FOLLOW(<program>) = { $ }
FOLLOW(<statement_list>) = { $ }
FOLLOW(<statement>) = { id, function_print, $ }
FOLLOW(<print>) = { id, function_print, $ }
FOLLOW(<declaration_variable>) = { id, function_print, $ }
FOLLOW(<expressionE>) = { ';', ')', ',' }
FOLLOW(<expressionE'>) = { ';', ')', ',' }
FOLLOW(<expressionT>) = { '+', '-', ';', ')', ',' }
FOLLOW(<expressionT'>) = { '+', '-', ';', ')', ',' }
FOLLOW(<expressionP>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
FOLLOW(<expressionP'>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
FOLLOW(<expressionF>) = { '^', '*', '/', '%', '+', '-', ';', ')', ',' }
*/
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "token.h"
#include "lexer.h"
#include "symboltable.h"

static void parserError(const char *msg);
static void match(Lexer *lexer, Token *token, TokenType tt, TokenValue tv, const char *err);

float logFunction(float num, float base);
float maxFuntion(float num1, float num2);
float minFunction(float num1, float num2);

static bool isFunctionID(Token *token);
static bool isPrint(Token *token);
static bool callFunction(const char* name, float num1, float num2, bool haveSecondArg, float* output);

float executeOpration(TokenType type, TokenValue value, float num1, float num2);

void program(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
void statement_list(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
void statement(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
void declaration_variable(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
void printProcedure(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);

float expressionE(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
float expressionE1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float acc);

float expressionT(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
float expressionT1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float acc);

float expressionP(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);
float expressionP1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float left);

float expressionF(Lexer* lexer, Token* token, SymbolTableHash* symbolTable);

static void parserError(const char *errorMessage){
        if (errorMessage && errorMessage[0] != '\0')
                printf("%s", errorMessage);
}

static void match(Lexer *lexer, Token *token, TokenType tt, TokenValue tv, const char *err){
        if (token->tokenType == tt && (tv == NONE || token->tokenValue == tv)) {
                *token = getNextToken(lexer);
                return;
        }

        parserError(err);

        //PANIC MODE:
        if (token->tokenType != TOKEN_EOF) {
                *token = getNextToken(lexer);
        }
}

float logFunction(float num, float base){
        if (num <= 0.0 || base <= 0.0 || base == 1.0){
                parserError("ERROR: Invalid input for log function");
                return NAN;
        }
        return log(num) / log(base);
}

float maxFuntion(float num1, float num2){
        if(num1 >= num2) return num1;
        return num2;
}

float minFunction(float num1, float num2){
        if(num1 >= num2) return num2;
        return num1;
}

static bool isFunctionID(Token *token){
        return token-> tokenType == TOKEN_ID && token->tokenValue == ID_FUNCTION;
}

static bool isPrint(Token *token){
    return isFunctionID(token) && strcmp(token->lexeme, "print") == 0;
}

static bool callFunction(const char* name, float num1, float num2, bool haveSecondArg, float* output) {
    if (strcmp(name, "log") == 0) {
        if (!haveSecondArg) return false;
        *output = logFunction(num1,num2);
        return true;
    }
    if (strcmp(name, "max") == 0) {
        if (!haveSecondArg) return false;
        *output = maxFuntion(num1,num2);
        return true;
    }
    if (strcmp(name, "min") == 0) {
        if (!haveSecondArg) return false;
        *output = minFunction(num1,num2);
        return true;
    }
    return false;
}

float executeOpration(TokenType type, TokenValue value, float num1, float num2){
        if(type == TOKEN_OPERATOR){
                switch(value){
                        case OPERATOR_SUM:
                                return num1 + num2;
                        case OPERATOR_SUB:
                                return num1 - num2;
                        case OPERATOR_MUL:
                                return num1 * num2;
                        case OPERATOR_DIV:
                                if(num2 == 0) {
                                        parserError("ERROR: division by 0\n");
                                        return NAN;
                                }
                                return num1 / num2;
                        case OPERATOR_POT:
                                return pow(num1,num2);
                        case OPERATOR_MOD:
                                int tempNum2 = (int)num2;
                                if(num2 == 0) {
                                        parserError("ERROR: modulo by 0\n");
                                        return NAN;
                                }
                                return (float)((int)num1 % tempNum2);
                        default:
                                parserError("ERROR: invalid operator\n");
                                return NAN;
                }
        }
        return NAN;
}

float expressionF(Lexer *lexer, Token *token, SymbolTableHash* symbolTable){
        /*<expressionF>  → '(' <expressionE> ')'
              | id
              | num
              | function '(' <expressionE> ',' <expressionE> ')'*/
        //FIRST(<expressionF>) = { '(', id, num, function }
        //FOLLOW(<expressionF>) = { '^', '*', '/', '%', '+', '-', ';', ')', ',' }
        // '(' <expressionE> ')'
        float value;

        // '(' <expressionE> ')'
        if (token->tokenType == TOKEN_PAREN && token->tokenValue == PAREN_OPEN) {
                match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");
                value = expressionE(lexer, token, symbolTable);
                match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
                return value;
        }

        // NUM
        if (token->tokenType == TOKEN_NUM) {
                value = token->variableValue;
                match(lexer, token, TOKEN_NUM, NONE, "Expected NUM\n");
                return value;
        }

        // Variable ID
        if (token->tokenType == TOKEN_ID && token->tokenValue != ID_FUNCTION) {
                Token* var = searchHash(symbolTable, token->lexeme);
                if (token->tokenType == TOKEN_ID && token->tokenValue != ID_FUNCTION) {
                        Token* var = searchHash(symbolTable, token->lexeme);
                        if (!var) {
                                parserError("Undefined variable\n");
                                match(lexer, token, TOKEN_ID, NONE, "");  // consome o ID mesmo assim
                                return NAN;
                        }
                        float value = var->variableValue;
                        match(lexer, token, TOKEN_ID, NONE, "Expected ID\n");
                        return value;
                }
                value = var->variableValue;
                match(lexer, token, TOKEN_ID, NONE, "Expected ID\n");
                return value;
        }

        //ID_FUNCTION '(' expr [',' expr] ')'
        if (isFunctionID(token)) {
                char fname[32];
                strncpy(fname, token->lexeme, sizeof(fname)-1);
                fname[sizeof(fname)-1] = '\0';

                match(lexer, token, TOKEN_ID, ID_FUNCTION, "Expected function name\n");
                match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");

                float a = expressionE(lexer, token, symbolTable);

                bool hasArg2 = false;
                float b = 0.0f;

                if (token->tokenType == TOKEN_COMMA) {
                        hasArg2 = true;
                        match(lexer, token, TOKEN_COMMA, COMMA, "Expected ','\n");
                    b = expressionE(lexer, token, symbolTable);
                }
                match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
                float out;
                if (!callFunction(fname, a, b, hasArg2, &out)) {
                        parserError("Unknown function or wrong number of arguments\n");
                        return NAN;
                }
                return out;
        }
        parserError("Unexpected token in <ExpressionF>. Expected '(', id, num or function\n");
        return NAN;
}

float expressionP1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float left){
        /*<expressionP'> → '^' <expressionP> | ε*/
        //FIRST(<expressionP'>) = { '^', ε }
        //FOLLOW(<expressionP'>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
        if (token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_POT){
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_POT, "Expected '^'");
                float right = expressionP(lexer, token, symbolTable); 
                return pow(left, right);
    }
    return left;
}

float expressionP(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<expressionP>  → <expressionF> <expressionP'>*/
        //FIRST(<expressionP>) = { '(', id, num, function }
        //FOLLOW(<expressionP>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
        float left = expressionF(lexer, token, symbolTable);
        return expressionP1(lexer, token, symbolTable, left);
}

float expressionT1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float acumulator){
        /*<expressionT'> → '*' <expressionP> <expressionT'>
                      | '/' <expressionP> <expressionT'>
                      | '%' <expressionP> <expressionT'>
                      | ε*/
        //FIRST(<expressionT'>) = { '*', '/', '%', ε }
        //FOLLOW(<expressionT'>) = { '+', '-', ';', ')', ',' }
        if (token->tokenType == TOKEN_OPERATOR) {
                TokenValue op = token->tokenValue;
                if (op == OPERATOR_MUL || op == OPERATOR_DIV || op == OPERATOR_MOD) {
                        match(lexer, token, TOKEN_OPERATOR, op, "Expected operator");
                        float right = expressionP(lexer, token, symbolTable);
                        acumulator = executeOpration(TOKEN_OPERATOR, op, acumulator, right);
                        return expressionT1(lexer, token, symbolTable, acumulator);
                }
        }
    return acumulator;
}

float expressionT(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<expressionT>  → <expressionP> <expressionT'>*/
        //FIRST(<expressionT>) = { '(', id, num, function }
        //FOLLOW(<expressionT>) = { '+', '-', ';', ')', ',' }
        float left = expressionP(lexer, token, symbolTable);
        return expressionT1(lexer, token, symbolTable, left);
}

float expressionE1(Lexer* lexer, Token* token, SymbolTableHash* symbolTable, float acumulator){
        /*<expressionE'> → '+' <expressionT> <expressionE'>
                      | '-' <expressionT> <expressionE'>
                      | ε*/
        //FIRST(<expressionE'>) = { '+', '-', ε }
        //FOLLOW(<expressionE'>) = { ';', ')', ',' }
        if (token->tokenType == TOKEN_OPERATOR) {
                TokenValue op = token->tokenValue;
                if (op == OPERATOR_SUM || op == OPERATOR_SUB) {
                        match(lexer, token, TOKEN_OPERATOR, op, "Expected operator");
                        float right = expressionT(lexer, token, symbolTable);
                        acumulator = executeOpration(TOKEN_OPERATOR, op, acumulator, right);
                        return expressionE1(lexer, token, symbolTable, acumulator);
                }
        }
    return acumulator;
}

float expressionE(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<expressionE>  → <expressionT> <expressionE'>*/
        //FIRST(<expressionE>) = { '(', id, num, function }
        //FOLLOW(<expressionE>) = { ';', ')', ',' }
        float left = expressionT(lexer, token, symbolTable);
        return expressionE1(lexer,token,symbolTable, left);
}

void declaration_variable(Lexer* lexer, Token* token, SymbolTableHash *symbolTable){
        /*<declaration_variable> → id '=' <expressionE> ';'*/
        //FIRST(<declaration_variable>) = { id }
        //FOLLOW(<declaration_variable>) = { id, function_print, $ }

        if (token->tokenType == TOKEN_ID && token->tokenValue == ID_FUNCTION) {
                parserError("ERROR: cannot assign to function name\n");
                return;
        }

        if(token->tokenType != TOKEN_ID){
                parserError("Unexpected token in <declaration_variable>. Expected id\n");
                return;
        }
        char varName[32];
        strcpy(varName, token->lexeme);
        TokenValue idKind = token->tokenValue;
        match(lexer, token, TOKEN_ID, NONE, "Expected id\n");

        if(idKind == ID_IMMUTABLE){
                parserError("ERROR: cannot assign to immutable identifier\n");
                while (token->tokenType != TOKEN_END_EXPRESSION &&
                token->tokenType != TOKEN_EOF) {
                *token = getNextToken(lexer);
                }
                if (token->tokenType == TOKEN_END_EXPRESSION)
                        *token = getNextToken(lexer);
        return;
    }

    match(lexer, token, TOKEN_ASSIGN, ASSIGN, "Expected '='\n");
    float value = expressionE(lexer, token, symbolTable);
    Token* var = searchHash(symbolTable, varName);
    if (var) {
        var->variableValue = value;
    } else {
        Token newVar;
        strcpy(newVar.lexeme, varName);
        newVar.tokenType = TOKEN_ID;
        newVar.variableValue = value;
        insertHash(symbolTable, newVar);
    }

        match(lexer, token, TOKEN_END_EXPRESSION, END, "Expected ';'\n");
}

void printProcedure(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<print> → function_print '(' expressionE ')' ';'*/
        //FIRST(<print>) = { function_print }
        //FOLLOW(<print>) = { id, function_print, $ }
        if(isPrint(token)){
                match(lexer, token, TOKEN_ID, ID_FUNCTION, "Expected print\n");
                match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");
                float result = expressionE(lexer, token, symbolTable);
                match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
                match(lexer, token, TOKEN_END_EXPRESSION, END, "Expected ';'\n");
                printf("%f\n", result);
                return;
        }
        parserError("Unexpected token in <print>. Expected print\n");
        return;
}

void statement(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<statement> → <declaration_variable>
                    | <print>*/
        //FIRST(<statement>) = { id, function_print }
        //FOLLOW(<statement>) = { id, function_print, $ }
        if (token->tokenType == TOKEN_ID) {
                if (isPrint(token)) {
                        printProcedure(lexer, token, symbolTable);
                } else {
                        declaration_variable(lexer, token, symbolTable);
                }
                        return;
                }
        parserError("Unexpected token in <statement>. Expected id\n");
}

void statement_list(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<statement_list> → <statement> <statement_list>
                         | ε*/
        //FIRST(<statement_list>) = { id, function_print, ε }
        //FOLLOW(<statement_list>) = { $ }
        if (token->tokenType == TOKEN_ID) {
                statement(lexer, token, symbolTable);
                statement_list(lexer, token, symbolTable);
                return;
        }
        if (token->tokenType == TOKEN_EOF) return;
        parserError("Unexpected token in <statement_list>. Expected id or EOF\n");
}

void program(Lexer* lexer, Token* token, SymbolTableHash* symbolTable){
        /*<program> → <statement_list>*/
        //FIRST(<program>) = { id, function_print, ε }
        //FOLLOW(<program>) = { $ }
        statement_list(lexer, token, symbolTable);
}