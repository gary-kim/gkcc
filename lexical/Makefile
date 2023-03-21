.PHONY: all
all: lextester

lextester: lex.yy.o lex.yy.h test.o
	gcc -o lextester lex.yy.o test.o lex.yy.h lex_extras.h

test.o: test.c lex.yy.h lex_extras.h
	gcc -c test.c -o test.o

lex.yy.c: c.lex ltests/tokens-manual.h lex_extras.h
	flex --header-file=lex.yy.h c.lex

lex.yy.h: c.lex ltests/tokens-manual.h lex_extras.h
	flex --header-file=lex.yy.h c.lex

lex.yy.o: lex.yy.c lex.yy.h lex_extras.h
	gcc -c lex.yy.c -o lex.yy.o

.PHONY: clean
clean:
	rm lex.yy.c lex.yy.h *.o
