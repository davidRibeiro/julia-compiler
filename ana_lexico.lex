%option noyywrap
%option nounput
%option noinput

%{
#include <math.h>	
#include <string.h>
#include "estruturas_com_cmd.h"
#include "y.tab.h"
%}

digit		[0-9]
int 		{digit}+
float 		({digit}+)?\.{digit}+(e[+\-]?{digit}+)?
letter 		[a-z][A-Z]
tab			[ \t]+
identifier 	[a-z][a-zA-Z0-9]*
line 		[ \n]+

%%

"+"				{return TOK_PLUS;}
"-"				{return TOK_MINUS;}
"*"				{return TOK_MULT;}
"/"				{return TOK_DIV;}
">="			{return TOK_GREAT_OR_EQUAL;}
"<="			{return TOK_LOWER_OR_EQUAL;}
"<"				{return TOK_LOWER;}
">"				{return TOK_GREAT;}
"!="			{return TOK_NOT_EQUAL;}
"=="			{return TOK_EQUAL;}
"=" 			{return TOK_ASSIGN;}
"!"				{return TOK_NEG;}
"println"		{return TOK_PRINT;}
"if"			{return TOK_IF;}
"elseif"		{return TOK_ELSEIF;}
"else"			{return TOK_ELSE;}
"while"			{return TOK_WHILE;}
"end"			{return TOK_END_INSTRUCTION;}
"true"			{return TOK_TRUE;}
"false"			{return TOK_FALSE;}
"("				{return TOK_OPEN;}
")"				{return TOK_CLOSE;}
{identifier}	{yylval.string = strdup(yytext); return TOK_VAR;}
{int}			{yylval.int_val = atoi(yytext); return TOK_INT;}
{float}			{yylval.float_val = atof(yytext); return TOK_FLOAT;}
{tab}			;		
{line}			{return TOK_LINE;}		

%%
