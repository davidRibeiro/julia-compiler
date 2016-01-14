programa:  y.tab.o lex.yy.o estruturas_com_cmd.o tresEnderecos.o mips.o
	gcc -g y.tab.o lex.yy.o estruturas_com_cmd.o tresEnderecos.o mips.o -o programa

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

y.tab.c: anal_sintatico.y
	bison -dy anal_sintatico.y

lex.yy.o: lex.yy.c y.tab.h
	gcc -g -c lex.yy.c

lex.yy.c: ana_lexico.lex
	flex ana_lexico.lex

estruturas_com_cmd.o: estruturas_com_cmd.c
	gcc -g -c estruturas_com_cmd.c

tresEnderecos.o: tresEnderecos.c
	gcc -g -c tresEnderecos.c 

mips.o: mips.c
	gcc -g -c mips.c