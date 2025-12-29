#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "symboltable.h"
#include "token.h"
#include "parser.h"

static FILE* getFile(const char *fileName){
    FILE *f = fopen(fileName, "r");
    if(!f) perror("Could not open file");
    return f;
}

int main(void) {
    char fileName[256];

    printf("Write the name of the .txt you want to compile:\n");
    if (scanf("%255s", fileName) != 1){
        printf("Invalid input.\n");
        return 1;
    }

    FILE *file = getFile(fileName);
    if (!file) return 1;

    SymbolTableHash *st = initHash(499);
    if(!st){
        printf("Could not create symbol table.\n");
        fclose(file);
        return 1;
    }

    Lexer lx;
    initLexer(&lx, file, st);

    // -------- PARSER TEST (instead of lexer dump) --------
    Token tk = getNextToken(&lx);

    if (tk.tokenType == TOKEN_ERROR) {
        printf("LEXICAL ERROR at line %d\n", lx.lineNumber);
    } else {
        program(&lx, &tk);

        // after parsing, we should be at EOF
        if (tk.tokenType == TOKEN_EOF) {
            printf("PARSE OK\n");
        } else {
            printf("PARSE ERROR: expected EOF, got type=%s value=%s lexeme='%s' (line %d)\n",
                   tokenTypeToString(tk.tokenType),
                   tokenValueToString(tk.tokenValue),
                   tk.lexeme,
                   lx.lineNumber);
        }
    }
    // ----------------------------------------------------

    freeHash(st);
    fclose(file);
    return 0;
}
