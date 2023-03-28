.PHONY: all
all: lexical

.PHONY: lexical
lexical: parser
	make -C lexical

.PHONY: parser
parser:
	make -C parser



