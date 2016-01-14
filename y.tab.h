/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_OPEN = 258,
     TOK_CLOSE = 259,
     TOK_PLUS = 260,
     TOK_MINUS = 261,
     TOK_MULT = 262,
     TOK_DIV = 263,
     TOK_INT = 264,
     TOK_FLOAT = 265,
     TOK_VAR = 266,
     TOK_IF = 267,
     TOK_ELSE = 268,
     TOK_ELSEIF = 269,
     TOK_WHILE = 270,
     TOK_END_INSTRUCTION = 271,
     TOK_PRINT = 272,
     TOK_NEG = 273,
     TOK_EQUAL = 274,
     TOK_NOT_EQUAL = 275,
     TOK_LOWER = 276,
     TOK_LOWER_OR_EQUAL = 277,
     TOK_GREAT = 278,
     TOK_GREAT_OR_EQUAL = 279,
     TOK_ASSIGN = 280,
     TOK_TRUE = 281,
     TOK_FALSE = 282,
     TOK_LINE = 283
   };
#endif
/* Tokens.  */
#define TOK_OPEN 258
#define TOK_CLOSE 259
#define TOK_PLUS 260
#define TOK_MINUS 261
#define TOK_MULT 262
#define TOK_DIV 263
#define TOK_INT 264
#define TOK_FLOAT 265
#define TOK_VAR 266
#define TOK_IF 267
#define TOK_ELSE 268
#define TOK_ELSEIF 269
#define TOK_WHILE 270
#define TOK_END_INSTRUCTION 271
#define TOK_PRINT 272
#define TOK_NEG 273
#define TOK_EQUAL 274
#define TOK_NOT_EQUAL 275
#define TOK_LOWER 276
#define TOK_LOWER_OR_EQUAL 277
#define TOK_GREAT 278
#define TOK_GREAT_OR_EQUAL 279
#define TOK_ASSIGN 280
#define TOK_TRUE 281
#define TOK_FALSE 282
#define TOK_LINE 283



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2053 of yacc.c  */
#line 12 "anal_sintatico.y"

	int int_val;
	float float_val;
	char *string;
	A_exp exprA;
	A_expBool exprB;
	A_cmd exprCmd;


/* Line 2053 of yacc.c  */
#line 123 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
