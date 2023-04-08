INCLUDEDIRS=-I../parser -I../ast -I../lexical
CFLAGS=--std="gnu17" -Wall

.PHONY: all
all: lextester

.PHONY: test
test: ltests/mytest.out

lextester: lex.yy.o lex.yy.h test.o
	gcc ${CFLAGS} -o lextester lex.yy.o test.o ${INCLUDEDIRS}

test.o: test.c lex.yy.h lex_extras.h
	gcc ${CFLAGS} -c test.c -o test.o ${INCLUDEDIRS}

lex.yy.c: lex.yy.h
lex.yy.h: c.lex lex_extras.h ../parser/c.tab.h
	flex --header-file=lex.yy.h c.lex

lex.yy.o: lex.yy.c lex.yy.h lex_extras.h
	gcc ${CFLAGS} -c lex.yy.c -o lex.yy.o ${INCLUDEDIRS}

ltests/mytest.out: ltests/mytest.err
ltests/mytest.err: lextester
	gcc -E ltests/*.c | ./lextester > ltests/mytest.out 2> ltests/mytest.err

.PHONY: tidy
tidy:
	clang-format -i *.c

.PHONY: clean
clean:
	rm lex.yy.c lex.yy.h *.o ltests/mytest.out ltests/mytest.err
