.PHONY: all
all: c.tab.c

c.tab.h: c.tab.c
c.tab.c: c.y
	bison c.y --output=c.tab.c --header=c.tab.h --html=report.html

.PHONY: clean
clean:
	rm c.tab.c c.tab.h report.html || true

