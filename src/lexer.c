#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "symboltable.h"

const char *tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_NUM:            return "TOKEN_NUM";
        case TOKEN_ID:             return "TOKEN_ID";
        case TOKEN_OPERATOR:       return "TOKEN_OPERATOR";
        case TOKEN_ASSIGN:         return "TOKEN_ASSIGN";
        case TOKEN_PAREN:          return "TOKEN_PAREN";
        case TOKEN_COMMA:          return "TOKEN_COMMA";
        case TOKEN_ERROR:          return "TOKEN_ERROR";
        case TOKEN_END_EXPRESSION: return "TOKEN_END_EXPRESSION";
        case TOKEN_EOF:            return "TOKEN_EOF";
        default:                   return "UNKNOWN_TOKEN";
    }
}

const char *tokenValueToString(TokenValue value) {
    switch (value) {
        case NUM:             return "NUM";
        case ID_IMMUTABLE:    return "ID_IMMUTABLE";
        case ID:              return "ID";
        case ID_FUNCTION:     return "ID_FUNCTION";
        case OPERATOR_SUM:    return "OPERATOR_SUM";
        case OPERATOR_SUB:    return "OPERATOR_SUB";
        case OPERATOR_MUL:    return "OPERATOR_MUL";
        case OPERATOR_DIV:    return "OPERATOR_DIV";
        case OPERATOR_POT:    return "OPERATOR_POT";
        case OPERATOR_MOD:    return "OPERATOR_MOD";
        case PAREN_OPEN:      return "PAREN_OPEN";
        case PAREN_CLOSE:     return "PAREN_CLOSE";
        case ASSIGN:          return "ASSIGN";
        case COMMA:           return "COMMA";
        case END:             return "END";
        case NONE:            return "NONE";
        default:              return "UNKNOWN_TOKEN";
    }
}

static Token makeFunctionIdToken(Lexer *lx, const char *name, int len) {
    Token t;
    strncpy(t.lexeme, name, sizeof(t.lexeme) - 1);
    t.lexeme[sizeof(t.lexeme) - 1] = '\0';
    t.tokenType = TOKEN_ID;
    t.tokenValue = ID_FUNCTION;
    t.variableValue = -1.0f;
    lx->cursor += len;
    return t;
}

void initTokenNumPi(Lexer* lx){
    Token t;
    char temp[] = "pi";
    strncpy(t.lexeme, temp, sizeof(t.lexeme) - 1);
    t.lexeme[sizeof(t.lexeme) - 1] = '\0';
    t.tokenType = TOKEN_ID;
    t.tokenValue = ID_IMMUTABLE;
    t.variableValue = 3.14159f;
    insertHash(lx->st, t);
}

void initTokenNumEuler(Lexer* lx){
    Token t;
    char temp[] = "e";
    strncpy(t.lexeme, temp, sizeof(t.lexeme) - 1);
    t.lexeme[sizeof(t.lexeme) - 1] = '\0';
    t.tokenType = TOKEN_ID;
    t.tokenValue = ID_IMMUTABLE;
    t.variableValue = 2.71828f;
    insertHash(lx->st, t);
}

void initReservedNums(Lexer *lx){
    initTokenNumPi(lx);
    initTokenNumEuler(lx);
}

static void lexicalError(const char *errorMessage){
    if (errorMessage && errorMessage[0] != '\0')
        printf("%s", errorMessage);
}

static bool isDigit(char c){
    return (c >= '0' && c <= '9');
}

static bool isLetter(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool matchKeyword(Lexer *lx, const char *kw, int kwLen) {
    if (lx->cursor + kwLen > lx->lineSize) return false;
    if (strncmp(&lx->line[lx->cursor], kw, kwLen) != 0) return false;

    char next = (lx->cursor + kwLen < lx->lineSize) ? lx->line[lx->cursor + kwLen] : '\0';
    if (isLetter(next) || isDigit(next)) return false;
    return true;
}

static Token makeErrorToken(const char *msg) {
    Token t;
    lexicalError(msg);
    t.lexeme[0] = '\0';
    t.tokenType = TOKEN_ERROR;
    t.tokenValue = NONE;
    t.variableValue = 0.0f;
    return t;
}

static Token makeEofToken(void){
    Token t;
    t.lexeme[0] = '\0';
    t.tokenType = TOKEN_EOF;
    t.tokenValue = ENDOFFILE;
    t.variableValue = -1.0f;
    return t;
}

void initLexer(Lexer *lx, FILE *file, SymbolTableHash *st) {
    lx->file = file;
    lx->st = st;
    lx->line[0] = '\0';
    lx->lineSize = 0;
    lx->cursor = 0;
    lx->lineNumber = 0;
    lx->eof = false;
    lx->hasLine = false;
    initReservedNums(lx);
}

static bool readNextLine(Lexer *lx) {
    if (lx == NULL || lx->file == NULL) return false;

    if (fgets(lx->line, (int)sizeof(lx->line), lx->file) == NULL) {
        lx->eof = true;
        lx->hasLine = false;
        lx->line[0] = '\0';
        lx->lineSize = 0;
        return false;
    }

    lx->lineSize = (int)strlen(lx->line);
    lx->cursor = 0;
    lx->lineNumber++;
    lx->hasLine = true;
    return true;
}

static void skipSpaces(Lexer *lx){
    while (lx->cursor < lx->lineSize){
        char c = lx->line[lx->cursor];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') lx->cursor++;
        else break;
    }
}

static Token getTOKEN_PAREN(Lexer *lx){
    Token t;

    if (lx->line[lx->cursor] == '('){
        t.tokenValue = PAREN_OPEN;
        t.lexeme[0] = '(';
    } else {
        t.tokenValue = PAREN_CLOSE;
        t.lexeme[0] = ')';
    }

    lx->cursor++;
    t.lexeme[1] = '\0';
    t.tokenType = TOKEN_PAREN;
    t.variableValue = -1.0f;
    return t;
}

static Token getTOKEN_END_EXPRESSION(Lexer *lx){
    Token t;
    t.lexeme[0] = ';';
    t.lexeme[1] = '\0';
    t.tokenType = TOKEN_END_EXPRESSION;
    t.tokenValue = END;
    t.variableValue = -1.0f;
    lx->cursor++;
    return t;
}

static Token getTOKEN_ASSIGN(Lexer *lx){
    Token t;
    t.lexeme[0] = '=';
    t.lexeme[1] = '\0';
    t.tokenType = TOKEN_ASSIGN;
    t.tokenValue = ASSIGN;
    t.variableValue = -1.0f;
    lx->cursor++;
    return t;
}

static Token getTOKEN_COMMA(Lexer *lx){
    Token t;
    t.lexeme[0] = ',';
    t.lexeme[1] = '\0';
    t.tokenType = TOKEN_COMMA;
    t.tokenValue = COMMA;
    t.variableValue = -1.0f;
    lx->cursor++;
    return t;
}

static Token getTOKEN_OPERATOR(Lexer *lx){
    Token t;
    char c = lx->line[lx->cursor];

    t.lexeme[0] = c;
    t.lexeme[1] = '\0';
    t.tokenType = TOKEN_OPERATOR;
    t.variableValue = -1.0f;

    switch(c){
        case '+': t.tokenValue = OPERATOR_SUM; break;
        case '-': t.tokenValue = OPERATOR_SUB; break;
        case '*': t.tokenValue = OPERATOR_MUL; break;
        case '/': t.tokenValue = OPERATOR_DIV; break;
        case '^': t.tokenValue = OPERATOR_POT; break;
        case '%': t.tokenValue = OPERATOR_MOD; break;
        default:  t.tokenValue = NONE; break;
    }

    lx->cursor++;
    return t;
}

static Token getTOKEN_ID(Lexer *lx){
    Token t;
    t.tokenType = TOKEN_ERROR;
    t.tokenValue = NONE;
    t.variableValue = -1.0f;
    t.lexeme[0] = '\0';

    char temp[256];
    int i = 0;

    while (lx->cursor < lx->lineSize && (isLetter(lx->line[lx->cursor]) || isDigit(lx->line[lx->cursor]))) {
        if (i < (int)sizeof(temp) - 1)
            temp[i++] = lx->line[lx->cursor];
        lx->cursor++;
    }
    temp[i] = '\0';

    strncpy(t.lexeme, temp, sizeof(t.lexeme) - 1);
    Token* found = searchHash(lx->st, temp);
    t.lexeme[sizeof(t.lexeme) - 1] = '\0';
    if (found && found->tokenValue == ID_IMMUTABLE) {
        return *found;
    }
    t.tokenType = TOKEN_ID;
    t.tokenValue = ID;
    t.variableValue = -1.0f;
    return t;
}

static Token getTOKEN_NUM(Lexer *lx){
    Token t;
    bool hasDot = false;
    char temp[256];
    int i = 0;

    if (lx->line[lx->cursor] == '-') {
        temp[i++] = '-';
        lx->cursor++;
        if (lx->cursor >= lx->lineSize || !isDigit(lx->line[lx->cursor])) {
            return makeErrorToken("A negative NUM needs a digit after the '-'\n");
        }
    }

    if (!isDigit(lx->line[lx->cursor])) {
        return makeErrorToken("NUM needs a digit\n");
    }

    while (lx->cursor < lx->lineSize && i < (int)sizeof(temp) - 1) {
        char c = lx->line[lx->cursor];
        if (isDigit(c)) {
            temp[i++] = c;
            lx->cursor++;
        } else if (c == '.' && !hasDot) {
            hasDot = true;
            temp[i++] = '.';
            lx->cursor++;
        } else {
            break;
        }
    }

    temp[i] = '\0';
    strncpy(t.lexeme, temp, sizeof(t.lexeme) - 1);
    t.lexeme[sizeof(t.lexeme) - 1] = '\0';
    t.tokenType = TOKEN_NUM;
    t.tokenValue = NUM;
    t.variableValue = strtof(temp, NULL);
    return t;
}

static Token scanToken(Lexer *lx){
    skipSpaces(lx);

    if (lx->cursor >= lx->lineSize) {
        Token t = makeErrorToken("");
        t.lexeme[0] = '\0';
        return t;
    }

    char c = lx->line[lx->cursor];

    if (c == ';') return getTOKEN_END_EXPRESSION(lx);
    if (c == '=') return getTOKEN_ASSIGN(lx);
    if (c == ',') return getTOKEN_COMMA(lx);
    if (c == '(' || c == ')') return getTOKEN_PAREN(lx);

    if (isDigit(c)) return getTOKEN_NUM(lx);

    if (isLetter(c)) {
        if (matchKeyword(lx, "print", 5)) return makeFunctionIdToken(lx, "print", 5);
        if (matchKeyword(lx, "log",   3)) return makeFunctionIdToken(lx, "log",   3);
        if (matchKeyword(lx, "max",   3)) return makeFunctionIdToken(lx, "max",   3);
        if (matchKeyword(lx, "min",   3)) return makeFunctionIdToken(lx, "min",   3);

        return getTOKEN_ID(lx);
    }

    if (c == '-') {
        if (lx->cursor + 1 < lx->lineSize && isDigit(lx->line[lx->cursor + 1])) {
            return getTOKEN_NUM(lx);
        }
        return getTOKEN_OPERATOR(lx);
    }

    if (c == '+' || c == '*' || c == '/' || c == '^' || c == '%') {
        return getTOKEN_OPERATOR(lx);
    }

    return makeErrorToken("Unexpected character\n");
}

Token getNextToken(Lexer *lx){
    while (1) {
        if (lx == NULL) return makeErrorToken("Lexer is NULL\n");

        if (lx->eof) {
            return makeEofToken();
        }

        if (!lx->hasLine || lx->cursor >= lx->lineSize) {
            lx->hasLine = false;
            if (!readNextLine(lx)) {
                return makeEofToken();
            }
        }
        
        Token t = scanToken(lx);

        if (t.tokenType == TOKEN_ERROR && t.lexeme[0] == '\0') {
            lx->hasLine = false;
            continue;
        }
        return t;
    }
}