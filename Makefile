.PHONY: all
all: lexical

.PHONY: lexical
lexical:
	make -C lexical

.PHONY: parser
	make -C parser

