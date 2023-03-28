.PHONY: all
all: lextester

.PHONY: test
test: ltests/mytest.out

lextester: lex.yy.o lex.yy.h test.o
	gcc -Wall -o lextester lex.yy.o test.o lex.yy.h lex_extras.h

test.o: test.c lex.yy.h lex_extras.h
	gcc -Wall -c test.c -o test.o

lex.yy.c: lex.yy.h
lex.yy.h: c.lex lex_extras.h tokens.h
	flex --header-file=lex.yy.h c.lex

lex.yy.o: lex.yy.c lex.yy.h lex_extras.h
	gcc -Wall -c lex.yy.c -o lex.yy.o

tokens.h: ../parser/c.y
	make -C "../parser" c.tab.h
	cp ../parser/c.tab.h tokens.h

ltests/mytest.out: ltests/mytest.err
ltests/mytest.err: lextester
	gcc -E ltests/*.c | ./lextester > ltests/mytest.out 2> ltests/mytest.err

.PHONY: tidy
tidy:
	clang-format -i *.c

.PHONY: clean
clean:
	rm lex.yy.c lex.yy.h *.o ltests/mytest.out ltests/mytest.err
