#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

void initLexer(FILE *file);
Token getNextTokenFromLine(const char *line, int lineSize);

#endif