#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"
#include "symboltable.h"

void initLexer(FILE *file, SymbolTableHash *st);
Token getNextTokenFromLine(const char *line, int lineSize);

#endif