all:
	flex -i Tradutor/analise_tokens.l
	bison -v Tradutor/gramatica.y
	gcc -o prolini gramatica.tab.c Tradutor/*.c Resolvedor_Programacao_Linear/*.c -lglpk -lm
	
clean:
	rm gramatica.output gramatica.tab.c lex.yy.c prolini
