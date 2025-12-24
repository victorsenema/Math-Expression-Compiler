CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

OBJ = main.o lexer.o symboltable.o
TARGET = compiler.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c lexer.h token.h symboltable.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h token.h symboltable.h
	$(CC) $(CFLAGS) -c lexer.c

symboltable.o: symboltable.c symboltable.h token.h
	$(CC) $(CFLAGS) -c symboltable.c

clean:
	-del /Q *.o $(TARGET) 2>NUL
