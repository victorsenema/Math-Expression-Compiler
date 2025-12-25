#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "symboltable.h"

// This code was adapted from a GitHub repository.
// https://github.com/arbackes/Estrutura-de-Dados-em-C/blob/main/Tabela%20Hash/TabelaHash.c#L9

typedef struct SymbolTableHash{
    int qtd, tableSize;
    Token **tokens;
}SymbolTableHash;

SymbolTableHash* initHash(int tableSize){
    SymbolTableHash *hash = (SymbolTableHash*) malloc(sizeof(SymbolTableHash));
    if (hash){
        hash -> tableSize = tableSize;
        hash -> tokens = (Token **) malloc (tableSize * sizeof(Token*));
        if(!hash->tokens){
            free(hash);
            return NULL;
        }
        hash -> qtd = 0;
        for (int i = 0; i < hash->tableSize; i++){
            hash -> tokens[i] = NULL;
        }
    }
    return hash;
}

void freeHash(SymbolTableHash* hash){
    if (hash){
        for(int i = 0; i < hash -> tableSize; i++){
            free(hash->tokens[i]);
        }
        free(hash->tokens);
        free(hash);
    }
}

int lexemeValue(char *lexeme){
    int value = 7;
    int lexemeSize = strlen(lexeme);
    for(int i = 0; i < lexemeSize; i++){
        value = 31 * value + (int) lexeme[i];
    }
    return (value & 0x7FFFFFFF);
}

int divisionKey(int key, int tableSize){
    printf("%d: ", (key & 0x7FFFFFFF) % tableSize);
    return (key & 0x7FFFFFFF) % tableSize;
}

int linearProbing(int position, int i, int tableSize){
    return ((position+i) & 0x7FFFFFFF) % tableSize;
}

int insertHash(SymbolTableHash* hash, Token token){
    if (!hash || hash -> qtd == hash -> tableSize)
        return 0;
    
    int key = lexemeValue(token.lexeme);

    int position = divisionKey(key,hash->tableSize);
    int newPosition;
    for (int i = 0; i < hash -> tableSize; i++){
        newPosition = linearProbing(position, i, hash -> tableSize);

        if(!hash -> tokens[newPosition]){
            Token *newToken;
            newToken = (Token*) malloc(sizeof(Token));
            if(!newToken)
                return 0;
            *newToken = token;
            hash -> tokens[newPosition] = newToken;
            hash -> qtd++;
            return 1;
        }
    }
    return 0;
}