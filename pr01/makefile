CC=gcc
EXECUTABLE=comp
LEXER_SOURCE=lex.yy.c
LANGUAGE_DESCRIPTION=lexicalStructure.lex
GRAMMAR=grammar.y
SYMBOLTABLE=symboltable.h
BISON=bison
GRAMMAR_TAB=grammar.tab.c
SYMBOLTABLE=sym

all:
	make clean
	make $(BISON)
	make $(LEXER_SOURCE)
	make $(SYMBOLTABLE)
	make $(EXECUTABLE)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)  *.o  *~  $(LEXER_SOURCE)
lexer:
	$(CC) runner.c $(LEXER_SOURCE) -o $(EXECUTABLE)
compiler:
	$(BISON) -d -v $(GRAMMAR)
	flex $(LANGUAGE_DESCRIPTION)
	gcc runner.c $(LEXER_SOURCE) -g -o test -lm

$(BISON):
	$(BISON) -d -v $(GRAMMAR)

$(LEXER_SOURCE): $(LANGUAGE_DESCRIPTION)
	flex $(LANGUAGE_DESCRIPTION)

$(SYMBOLTABLE):
	gcc -o test $(GRAMMAR_TAB) $(LEXER_SOURCE)

$(EXECUTABLE): $(LEXER_SOURCE) runner.c
	$(CC) runner.c $(LEXER_SOURCE) -o $(EXECUTABLE)


#alphonce order of make operations
# $(EXE): $(SRC) $(HDR) $(LEX) $(GRAMMAR)
#         @make clean
#         @bison -d -v $(GRAMMAR)
#         @flex $(LEX)
#         @gcc -g -o $(EXE) $(SRC)