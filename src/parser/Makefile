INCLUDEDIRS=-I../lexical -I../parser -I../ast

.PHONY: all
all: parsetester

.PHONY: ast
ast:
	make -C ../ast

c.tab.h: c.tab.c
c.tab.c: c.y
	bison c.y --debug --output=c.tab.c --header=c.tab.h --html=report.html --report=lookaheads -v -Wall

c.tab.o: c.tab.c c.tab.h ast
	gcc -c c.tab.c -o c.tab.o ${INCLUDEDIRS}

parsetester.o: parsetester.c parsetester.h c.tab.h
	gcc -c parsetester.c -o parsetester.o ${INCLUDEDIRS}

parsetester: parsetester.o c.tab.o ast
	gcc -o parsetester parsetester.o c.tab.o ../ast/ast.o ../lexical/lex.yy.o ${INCLUDEDIRS}

.PHONY: clean
clean:
	rm c.tab.c c.tab.h report.html || true

