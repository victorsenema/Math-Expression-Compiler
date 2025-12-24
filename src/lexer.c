#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

//NEED TO ADD THE INSERTION OF THE TOKEN IN THE SYMBOLTABLE

int globalLineCursor = 0;

void lexicalError(char *errorMessage){
    printf("%s", errorMessage);
    return;
}

bool isDigit(char c){
    return(c >= '0' && c <= '9');
}

static Token makeErrorToken(char *msg) {
    Token t;
    lexicalError(msg);
    t.lexeme[0] = '\0';
    t.tokenType = TOKEN_ERROR;
    t.tokenValue = NONE;
    t.tablePosition = -1;
    t.variableValue = 0.0f;
    return t;
}

Token getTOKEN_NUM(char* line, int lineSize){
    Token numToken;
    bool hasDot = false;
    char tempString[256];

    int i = 0;

    if(line[globalLineCursor] == '-'){
        tempString[i] = '-';
        globalLineCursor++, i++;
        if (globalLineCursor >= lineSize || !isDigit(line[globalLineCursor])) {
            return makeErrorToken("A negative NUM needs a digit after the '-'\n");
        }
    }

    if(isDigit(line[globalLineCursor])){
        tempString[i] = line[globalLineCursor];
        i++, globalLineCursor++;
    }else{
        return makeErrorToken("NUM needs a digit\n");
    }

    while(globalLineCursor < lineSize  && i < (int)sizeof(tempString) - 1){
        char c = line[globalLineCursor];
        if(isDigit(c)){
            tempString[i] = c;
        }else if (c == '.' && !hasDot){
            hasDot = true;
            tempString[i] = '.';
        }else{
            break;
        }
        globalLineCursor++;
        i++;
    }
    tempString[i] = '\0';

    strncpy(numToken.lexeme, tempString, sizeof(numToken.lexeme) - 1);
    numToken.lexeme[sizeof(numToken.lexeme) - 1] = '\0';
    numToken.tokenType = TOKEN_NUM;
    numToken.tokenValue = NUM;
    numToken.variableValue = strtof(tempString, NULL);
    numToken.tablePosition = -1;//Por enquanto
    return numToken;
}

static void skipSpaces(char *line, int lineSize){
    while (globalLineCursor < lineSize){
        char c = line[globalLineCursor];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') globalLineCursor++;
        else break;
    }
}

Token getToken(char *line, int lineSize){
    skipSpaces(line, lineSize);

    if (globalLineCursor >= lineSize){
        Token t = makeErrorToken("");   // signal end-of-line with empty error token
        t.lexeme[0] = '\0';
        return t;
    }

    char c = line[globalLineCursor];

    if(isDigit(c) || c == '-'){
        return getTOKEN_NUM(line, lineSize);
    }

    return makeErrorToken("Unexpected character\n");
}

void initLexer(FILE *file){
    
    if (file == NULL) {
        perror("File is NULL");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int lineSize = strlen(line);
        globalLineCursor = 0;

        while (globalLineCursor < lineSize){
            Token t = getToken(line, lineSize);
            getToken(line, lineSize);

            if (t.tokenType == TOKEN_ERROR && t.lexeme[0] == '\0')
                break;

            if (t.tokenType == TOKEN_ERROR){
                // error already printed by makeErrorToken
                break;
            }
            printf("TOKEN_NUM lexeme='%s' value=%f\n", t.lexeme, t.variableValue);
        }

        // TODO: process the line here
    }
}