#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"
#include "symboltable.h"

//NEED TO ADD THE INSERTION OF THE TOKEN IN THE SYMBOLTABLE
//DEFINE RESERVED WORDS
/*
typedef enum {
    TOKEN_NUM, -> DONE
    TOKEN_ID, -> LAST
    TOKEN_OPERATOR, -> DONE
    TOKEN_ASSIGN, -> DONE
    TOKEN_PAREN, -> DONE
    TOKEN_COMMA, -> DONE
    TOKEN_ERROR -> DONE
} TokenType;

*/
//

//FOR DEBUG PROPOSE
//MAYBE add a feature so the user can see the process of the compiler
const char *tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_NUM:       return "TOKEN_NUM";
        case TOKEN_ID:        return "TOKEN_ID";
        case TOKEN_OPERATOR:  return "TOKEN_OPERATOR";
        case TOKEN_ASSIGN:    return "TOKEN_ASSIGN";
        case TOKEN_PAREN:     return "TOKEN_PAREN";
        case TOKEN_COMMA:     return "TOKEN_COMMA";
        case TOKEN_ERROR:     return "TOKEN_ERROR";
        default:              return "UNKNOWN_TOKEN";
    }
}

const char *tokenValueToString(TokenValue value) {
    switch (value) {
        case NUM:             return "NUM";
        case ID:              return "ID";
        case OPERATOR_SUM:    return "OPERATOR_SUM";
        case OPERATOR_SUB:    return "OPERATOR_SUB";
        case OPERATOR_MUL:    return "OPERATOR_MUL";
        case OPERATOR_DIV:    return "OPERATOR_DIV";
        case OPERATOR_MOD:    return "OPERATOR_MOD";
        case PAREN_OPEN:      return "PAREN_OPEN";
        case PAREN_CLOSE:     return "PAREN_CLOSE";
        case ASSIGN:          return "ASSIGN";
        case COMMA:           return "COMMA";
        case NONE:            return "NONE";
        default:              return "UNKNOWN_TOKEN";
    }
}

int globalLineCursor = 0;

void lexicalError(char *errorMessage){
    printf("%s", errorMessage);
    return;
}

bool isDigit(char c){
    return(c >= '0' && c <= '9');
}

bool isLetter(char c){
    return((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static Token makeErrorToken(char *msg) {
    Token t;
    lexicalError(msg);
    t.lexeme[0] = '\0';
    t.tokenType = TOKEN_ERROR;
    t.tokenValue = NONE;
    t.variableValue = 0.0f;
    return t;
}

Token getTOKEN_PAREN(char* line){
    Token tokenParen;

    if (line[globalLineCursor] == '('){        
        tokenParen.tokenValue = PAREN_OPEN;
        tokenParen.lexeme[0] = '(';
    }
    else{
        tokenParen.tokenValue = PAREN_CLOSE;
        tokenParen.lexeme[0] = ')';
    }
    globalLineCursor++;

    tokenParen.lexeme[1] = '\0';
    tokenParen.tokenType = TOKEN_PAREN;
    tokenParen.variableValue = -1;

    return tokenParen;
}

Token getTOKEN_ASSING(){
    Token tokenAssing;

    tokenAssing.lexeme[0] = '=';
    tokenAssing.lexeme[1] = '\0';
    tokenAssing.tokenType = TOKEN_ASSIGN;
    tokenAssing.tokenValue = ASSIGN;
    tokenAssing.variableValue = -1;

    globalLineCursor++;
    return tokenAssing;
}

Token getTOKEN_COMMA(){
    Token tokenComma;

    tokenComma.lexeme[0] = ',';
    tokenComma.lexeme[1] = '\0';
    tokenComma.tokenType = TOKEN_COMMA;
    tokenComma.tokenValue = COMMA;
    tokenComma.variableValue = -1;

    globalLineCursor++;
    return tokenComma;
}

Token getTOKEN_OPERATOR(char* line){
    Token tokenOperator;
    tokenOperator.lexeme[0] = line[globalLineCursor];
    tokenOperator.lexeme[1] = '\0';

    switch(line[globalLineCursor]){
        case '+':
            tokenOperator.tokenValue = OPERATOR_SUM;
            break;
        case '-':
            tokenOperator.tokenValue = OPERATOR_SUB;
            break;
        case '*':
            tokenOperator.tokenValue = OPERATOR_MUL;
            break;
        case '/':
            tokenOperator.tokenValue = OPERATOR_DIV;
            break;
        case 37:
            tokenOperator.tokenValue = OPERATOR_MOD;
            break;
    }
    globalLineCursor++;
    tokenOperator.tokenType = TOKEN_OPERATOR;
    tokenOperator.variableValue = -1;

    return tokenOperator;
}

Token getTOKEN_ID(char *line, int lineSize){
    Token tokenID;
    char tempString[256];
    int i = 0;
    //add here, pi and e
    if (isLetter(line[globalLineCursor])){
        tempString[i] = line[globalLineCursor];
        globalLineCursor++, i++;
    }

    while(isDigit(line[globalLineCursor]) || isLetter(line[globalLineCursor])){
        tempString[i] = line[globalLineCursor];
        globalLineCursor++, i++;
    }
    tempString[i] = '\0';
    strncpy(tokenID.lexeme, tempString, sizeof(tokenID.lexeme) - 1);
    tokenID.lexeme[sizeof(tokenID.lexeme) - 1] = '\0';
    tokenID.tokenType = TOKEN_ID;
    tokenID.tokenValue = ID;
    tokenID.variableValue = -1;
    return tokenID;
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
        Token t = makeErrorToken("");
        t.lexeme[0] = '\0';
        return t;
    }
    
    char c = line[globalLineCursor];
    
    //TOKEN_NUM only positives
    if (isDigit(c))
        return getTOKEN_NUM(line, lineSize);
    else if(isLetter(c))
        return getTOKEN_ID(line, lineSize);
    //TOKEN_ASSING
    else if(c == '=')
        return getTOKEN_ASSING();
    //TOKENS NUM or OPERATOR that start with -
    else if (c == '-') {
        // lookahead
        if (globalLineCursor + 1 < lineSize && isDigit(line[globalLineCursor + 1])) {
            return getTOKEN_NUM(line, lineSize);//TOKEN_NUM
        } else {
            return getTOKEN_OPERATOR(line);//TOKEN_OPERATOR
        }
    }
    //TOKEN PAREN
    else if(line[globalLineCursor] == '(' || line[globalLineCursor] == ')')
        return getTOKEN_PAREN(line);
    else if (c == ',')
        return getTOKEN_COMMA();

    //operator characters
    char operatorCharacteres[6] = { '+', '-', '*', '/', 37, '\0' };
    //TOKEN_OPERATOR
    for(int i = 0; operatorCharacteres[i] != '\0'; i++){
        if(c == operatorCharacteres[i])
            return getTOKEN_OPERATOR(line);
    }
    
    return makeErrorToken("Unexpected character\n");
}

void initLexer(FILE *file, SymbolTableHash *st){
    
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

            if (t.tokenType == TOKEN_ERROR && t.lexeme[0] == '\0')
                break;

            if (t.tokenType == TOKEN_ERROR){
                break;
            }
            insertHash(st, t);
            printf("type=%s type='%s' lexeme='%s' value=%f\n",
                tokenTypeToString(t.tokenType),
                tokenValueToString(t.tokenValue),
                t.lexeme,
                t.variableValue);
        }

        // TODO: process the line here
    }
}