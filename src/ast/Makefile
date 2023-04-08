INCLUDEDIRS=-I../lexical -I../parser -I../ast
CFLAGS=--std="gnu17" -Wall

.PHONY: all
all: ast.o

ast.o: ast.c ast.h
	gcc ${CFLAGS} -c -o ast.o ast.c ${INCLUDEDIRS}
