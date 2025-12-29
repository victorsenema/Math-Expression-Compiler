#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>
#include "token.h"
#include "symboltable.h"

typedef struct Lexer {
    FILE *file;
    SymbolTableHash *st;

    char line[1024];
    int lineSize;
    int cursor;
    int lineNumber;

    bool eof;
    bool hasLine;
} Lexer;

void initLexer(Lexer *lx, FILE *file, SymbolTableHash *st);
Token getNextToken(Lexer *lx);
const char *tokenTypeToString(TokenType type);
const char *tokenValueToString(TokenValue value);

#endif