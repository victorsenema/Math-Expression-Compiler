#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "symboltable.h"

static FILE* getFile(const char *fileName){
    FILE *f = fopen(fileName, "r");
    if(!f) perror("Could not open file");
    return f;
}

int main() {
    char fileName[256];

    printf("Write the name of the .txt you want to compile:\n");
    if (scanf("%255s", fileName) != 1){
        printf("Invalid input.\n");
        return 1;
    }

    FILE *file = getFile(fileName);
    if (!file) return 1;

    // if you want a symbol table:
    SymbolTableHash *st = initHash(499);
    if(!st){
        printf("Could not create symbol table.\n");
        fclose(file);
        return 1;
    }

    // Quick test: lex & print tokens (current initLexer prints)
    initLexer(file, st);

    // Later: you can modify initLexer to call insertHash(st, token) for NUM tokens.

    freeHash(st);
    fclose(file);
    return 0;
}
