typedef struct SymbolTableHash SymbolTableHash;

SymbolTableHash* initHash(int tableSize);
void freeHash(SymbolTableHash* hash);
int lexemeValue(char *lexeme);
int divisionKey(int key, int tableSize);
int linearProbing(int position, int i, int tableSize);
int insertHash(SymbolTableHash* hash, Token token);