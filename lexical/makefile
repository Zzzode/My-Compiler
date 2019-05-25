lexer: lex.yy.o lexical.h
	gcc -o lexer lex.yy.o

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c

lex.yy.c: lexical.l  lexical.h
	lex lexical.l