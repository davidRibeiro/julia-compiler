%{
#include <stdio.h>
#include <math.h>
#include "estruturas_com_cmd.h"
#include "tresEnderecos.h"
void yyerror(char const *);
int yylex();

%}


%union{
	int int_val;
	float float_val;
	char *string;
	A_exp exprA;
	A_expBool exprB;
	A_cmd exprCmd;
}

%token TOK_OPEN					/* TOK_OPEN	*/
%token TOK_CLOSE				/* TOK_CLOSE	*/
%token TOK_PLUS 				/* '+' */
%token TOK_MINUS				/* '-' */
%token TOK_MULT					/* '*' */
%token TOK_DIV					/* '/' */
%token TOK_INT 					/* inteiro */
%token TOK_FLOAT				/* float */
%token TOK_VAR		 			/* variavel */
%token TOK_IF 					/* 'if' */
%token TOK_ELSE 				/* 'else' */
%token TOK_ELSEIF 				/* 'elseif' */
%token TOK_WHILE				/* 'while' */
%token TOK_END_INSTRUCTION		/* 'end' */
%token TOK_PRINT				/* 'printn' */
%token TOK_NEG					/* '!' */
%token TOK_EQUAL 				/* '==' */
%token TOK_NOT_EQUAL 			/* '!=' */
%token TOK_LOWER 				/* '<' */
%token TOK_LOWER_OR_EQUAL 		/* '<=' */
%token TOK_GREAT 				/* '>' */
%token TOK_GREAT_OR_EQUAL 		/* '>=' */
%token TOK_ASSIGN 				/* '=' */
%token TOK_TRUE 				/* 'true' */
%token TOK_FALSE 				/* 'false' */
%token TOK_LINE					/* '\n'	*/

%type <int_val> TOK_INT
%type <float_val> TOK_FLOAT
%type <exprA> expA 
%type <exprB> expB bool
%type <exprCmd> input cmd cmdAux
%type <string> TOK_VAR

%right TOK_ASSIGN
%left TOK_PLUS TOK_MINUS
%left TOK_MULT TOK_DIV




%%

input : cmd 											{traduzirPrograma($1);}
;

cmd : TOK_PRINT TOK_OPEN cmd TOK_CLOSE  				{$$=Print($3);}
	| cmdAux											{$$=$1;}
	| TOK_WHILE expB cmd TOK_END_INSTRUCTION 			{$$=WhileStatment($2, $3);}
	| TOK_VAR TOK_ASSIGN expA 							{$$=Assign($1,$3);} 
	| cmd TOK_LINE cmd 									{$$=Multiple($1,$3);}
	| TOK_LINE cmd 										{$$=$2;}
	| cmd TOK_LINE 										{$$=$1;}
;

cmdAux: TOK_IF expB cmd TOK_END_INSTRUCTION 			{$$=IfStatement($2,$3,NULL);}
	| TOK_IF expB cmd TOK_ELSE cmd TOK_END_INSTRUCTION 	{$$=IfStatement($2,$3,$5);} 					
;

expA : TOK_INT											{$$=I_Num($1);}
	| TOK_FLOAT											{$$=F_Num($1);}
	| expA TOK_PLUS expA								{$$=ExpArit(ADD,$1,$3);}
	| expA TOK_MINUS expA 								{$$=ExpArit(SUB,$1,$3);}
	| expA TOK_MULT expA 								{$$=ExpArit(MULT,$1,$3);}
	| expA TOK_DIV expA 								{$$=ExpArit(DIV,$1,$3);}
	| TOK_OPEN expA TOK_CLOSE 							{$$=$2;} 	
;

bool : TOK_TRUE 										{$$=B_Num("True", 0);}
	| TOK_FALSE 										{$$=B_Num("False", 0);}
	| TOK_NEG TOK_TRUE 									{$$=B_Num("True", 1);}
	| TOK_NEG TOK_FALSE 								{$$=B_Num("False", 1);}

;

expB : bool
	| expA TOK_EQUAL expA 								{$$=ExpBool(EQUALS, $1, $3);}
	| expA TOK_NOT_EQUAL expA 							{$$=ExpBool(NOTEQUAL, $1, $3);}
	| expA TOK_GREAT expA 								{$$=ExpBool(GREATER, $1, $3);}
	| expA TOK_LOWER expA 								{$$=ExpBool(LOWER, $1, $3);}
	| expA TOK_GREAT_OR_EQUAL expA 						{$$=ExpBool(GREATEQUAL, $1, $3);}
	| expA TOK_LOWER_OR_EQUAL expA 						{$$=ExpBool(LOWEQUAL, $1, $3);}
	| TOK_OPEN expB TOK_CLOSE 							{$$=$2;}
;

%% 

#include <stdio.h>

void yyerror(char const *s){
	fprintf(stderr, "%s\n", s);
}

int main(){
	yyparse();
	return 0;
}