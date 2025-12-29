//LTODO: OOK FOR EVERY % IN STRING
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
#include "token.h"
#include "lexer.h"

static void parserError(const char *msg);
static void match(Lexer *lexer, Token *token, TokenType tt, TokenValue tv, const char *err);

void program(Lexer* lexer, Token* token);
void statement_list(Lexer* lexer, Token* token);
void statement(Lexer* lexer, Token* token);
void declaration_variable(Lexer* lexer, Token* token);
void print(Lexer* lexer, Token* token);

void expressionE(Lexer* lexer, Token* token);
void expressionE1(Lexer* lexer, Token* token);
void expressionT(Lexer* lexer, Token* token);
void expressionT1(Lexer* lexer, Token* token);
void expressionP(Lexer* lexer, Token* token);
void expressionP1(Lexer* lexer, Token* token);
void expressionF(Lexer* lexer, Token* token);

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

        // PANIC MODE:
        // if (token->tokenType != TOKEN_EOF) {
        //         *token = getNextToken(lexer);
        // }
}

static bool isMathFunction(Token *t){
    return t->tokenType == TOKEN_FUNCTION &&
           (t->tokenValue == FUNCTION_LOG
            /* || t->tokenValue == FUNCTION_SIN
               || t->tokenValue == FUNCTION_COS
               || ... */);
}


void expressionF(Lexer *lexer, Token *token){
    if (token->tokenType == TOKEN_PAREN && token->tokenValue == PAREN_OPEN) {
        match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");
        expressionE(lexer, token);
        match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
        return;
    }
    if (token->tokenType == TOKEN_ID) {
        match(lexer, token, TOKEN_ID, NONE, "Expected ID\n");
        return;
    }
    if (token->tokenType == TOKEN_NUM) {
        match(lexer, token, TOKEN_NUM, NONE, "Expected NUM\n");
        return;
    }
    if (isMathFunction(token)) {
        TokenValue fn = token->tokenValue;
        match(lexer, token, TOKEN_FUNCTION, fn, "Expected math function\n");
        match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");
        expressionE(lexer, token);
        if (fn == FUNCTION_LOG) {
            match(lexer, token, TOKEN_COMMA, COMMA, "Expected ','\n");
            expressionE(lexer, token);
        }

        match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
        return;
    }

    parserError("Unexpected token in <ExpressionF>. Expected '(', id, num or math function\n");
}

void expressionP1(Lexer* lexer, Token* token){
        /*<expressionP'> → '^' <expressionP> | ε*/
        //FIRST(<expressionP'>) = { '^', ε }
        //FOLLOW(<expressionP'>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
        if (token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_POT) {
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_POT, "Expected '^'\n");
                expressionP(lexer, token);
                return;
        }

        //FOLLOW: lookahed that analysises
        if (token->tokenType == TOKEN_OPERATOR &&
                (token->tokenValue == OPERATOR_MUL ||
                token->tokenValue == OPERATOR_DIV ||
                token->tokenValue == OPERATOR_MOD ||
                token->tokenValue == OPERATOR_SUM ||
                token->tokenValue == OPERATOR_SUB)) {
                return;
        }

        if (token->tokenType == TOKEN_END_EXPRESSION && token->tokenValue == END) return; // ';'
        if (token->tokenType == TOKEN_PAREN && token->tokenValue == PAREN_CLOSE) return;  // ')'
        if (token->tokenType == TOKEN_COMMA && token->tokenValue == COMMA) return;        // ','
        
        parserError("Unexpected token in <ExpressionP1>. Expected {operators,';',')',',' }\n");
        return;
}

void expressionP(Lexer* lexer, Token* token){
        /*<expressionP>  → <expressionF> <expressionP'>*/
        //FIRST(<expressionP>) = { '(', id, num, function }
        //FOLLOW(<expressionP>) = { '*', '/', '%', '+', '-', ';', ')', ',' }
        expressionF(lexer, token);
        expressionP1(lexer, token);
        return;
}

void expressionT1(Lexer* lexer, Token* token){
        /*<expressionT'> → '*' <expressionP> <expressionT'>
                      | '/' <expressionP> <expressionT'>
                      | '%' <expressionP> <expressionT'>
                      | ε*/
        //FIRST(<expressionT'>) = { '*', '/', '%', ε }
        //FOLLOW(<expressionT'>) = { '+', '-', ';', ')', ',' }

        if (token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_MUL) {
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_MUL, "Expected '*'\n");
                expressionP(lexer, token);
                expressionT1(lexer, token);
                return;
        }

        if (token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_DIV) {
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_DIV, "Expected '/'\n");
                expressionP(lexer, token);
                expressionT1(lexer, token);
                return;
        }

        if (token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_MOD) {
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_MOD, "Expected '%'\n");
                expressionP(lexer, token);
                expressionT1(lexer, token);
                return;
        }

        if (token->tokenType == TOKEN_OPERATOR &&
            (token->tokenValue == OPERATOR_SUM || token->tokenValue == OPERATOR_SUB)) {
                return;
        }

        if (token->tokenType == TOKEN_END_EXPRESSION && token->tokenValue == END) return; // ';'
        if (token->tokenType == TOKEN_PAREN && token->tokenValue == PAREN_CLOSE) return;  // ')'
        if (token->tokenType == TOKEN_COMMA && token->tokenValue == COMMA) return;        // ','
        parserError("Unexpected token in <ExpressionT1>. Expected {operators,';',')',',' }\n");
}

void expressionT(Lexer* lexer, Token* token){
        /*<expressionT>  → <expressionP> <expressionT'>*/
        //FIRST(<expressionT>) = { '(', id, num, function }
        //FOLLOW(<expressionT>) = { '+', '-', ';', ')', ',' }
        expressionP(lexer, token);
        expressionT1(lexer, token);
        return;
}

void expressionE1(Lexer* lexer, Token* token){
        /*<expressionE'> → '+' <expressionT> <expressionE'>
                      | '-' <expressionT> <expressionE'>
                      | ε*/
        //FIRST(<expressionE'>) = { '+', '-', ε }
        //FOLLOW(<expressionE'>) = { ';', ')', ',' }
        if(token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_SUB){
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_SUB, "Expected '-'\n");
                expressionT(lexer, token);
                expressionE1(lexer, token);
                return;
        }
        if(token->tokenType == TOKEN_OPERATOR && token->tokenValue == OPERATOR_SUM){
                match(lexer, token, TOKEN_OPERATOR, OPERATOR_SUM, "Expected '+'\n");
                expressionT(lexer, token);
                expressionE1(lexer, token);
                return;
        }

        if (token->tokenType == TOKEN_END_EXPRESSION && token->tokenValue == END) return; // ';'
        if (token->tokenType == TOKEN_PAREN && token->tokenValue == PAREN_CLOSE) return;  // ')'
        if (token->tokenType == TOKEN_COMMA && token->tokenValue == COMMA) return;        // ','

        parserError("Unexpected token in <ExpressionE1>. Expected {'+','-',';',')',','}\n");
        return;
}

void expressionE(Lexer* lexer, Token* token){
        /*<expressionE>  → <expressionT> <expressionE'>*/
        //FIRST(<expressionE>) = { '(', id, num, function }
        //FOLLOW(<expressionE>) = { ';', ')', ',' }
        expressionT(lexer, token);
        expressionE1(lexer,token);
        return;
}

void declaration_variable(Lexer* lexer, Token* token){
        /*<declaration_variable> → id '=' <expressionE> ';'*/
        //FIRST(<declaration_variable>) = { id }
        //FOLLOW(<declaration_variable>) = { id, function_print, $ }
        if(token->tokenType == TOKEN_ID){
                match(lexer, token, TOKEN_ID, NONE, "Expected id\n");
                match(lexer, token, TOKEN_ASSIGN, ASSIGN, "Expected '='\n");
                expressionE(lexer, token);
                match(lexer, token, TOKEN_END_EXPRESSION, END, "Expected ';'\n");
                return;
        }
        parserError("Expected id\n");
}

void print(Lexer* lexer, Token* token){
        /*<print> → function_print '(' expressionE ')' ';'*/
        //FIRST(<print>) = { function_print }
        //FOLLOW(<print>) = { id, function_print, $ }
        if(token->tokenType == TOKEN_FUNCTION && token->tokenValue == FUNCTION_PRINT){
                match(lexer, token, TOKEN_FUNCTION, FUNCTION_PRINT, "Expected print\n");
                match(lexer, token, TOKEN_PAREN, PAREN_OPEN, "Expected '('\n");
                expressionE(lexer, token);
                match(lexer, token, TOKEN_PAREN, PAREN_CLOSE, "Expected ')'\n");
                match(lexer, token, TOKEN_END_EXPRESSION, END, "Expected ';'\n");
                return;
        }
        parserError("Unexpected token in <print>. Expected print\n");
        return;
}

void statement(Lexer* lexer, Token* token){
        /*<statement> → <declaration_variable>
                    | <print>*/
        //FIRST(<statement>) = { id, function_print }
        //FOLLOW(<statement>) = { id, function_print, $ }
        if(token->tokenType == TOKEN_ID){
                declaration_variable(lexer,token);
                return;
        }
        if(token->tokenType == TOKEN_FUNCTION && token->tokenValue == FUNCTION_PRINT){
                print(lexer, token);
                return;
        }
        parserError("Unexpected token in <statement>. Expected id or print\n");
        return;
}

void statement_list(Lexer* lexer, Token* token){
        /*<statement_list> → <statement> <statement_list>
                         | ε*/
        //FIRST(<statement_list>) = { id, function_print, ε }
        //FOLLOW(<statement_list>) = { $ }
        if (token->tokenType == TOKEN_ID ||
            (token->tokenType == TOKEN_FUNCTION && token->tokenValue == FUNCTION_PRINT)) {
                statement(lexer, token);
                statement_list(lexer, token);
                return;
        }
        if (token->tokenType == TOKEN_EOF) return;

        parserError("Unexpected token in <statement_list>. Expected id, print, or EOF\n");
}

void program(Lexer* lexer, Token* token){
        /*<program> → <statement_list>*/
        //FIRST(<program>) = { id, function_print, ε }
        //FOLLOW(<program>) = { $ }
        statement_list(lexer, token);
}