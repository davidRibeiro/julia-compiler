#ifndef ESTRUTURAS_COM_CMD_
#define ESTRUTURAS_COM_CMD_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct A_exp_ *A_exp;
typedef struct A_cmd_ *A_cmd;
typedef struct A_expBool_ *A_expBool;
typedef enum { ADD, SUB, MULT, DIV, GREATER, LOWER, EQUALS, NOTEQUAL, GREATEQUAL, LOWEQUAL, NEGATE} oper;
typedef enum {False,True} boolean;


//Estrutura Avaliadora de Expressoes Aritmeticas e atribuicoes
struct A_exp_
{ enum {IntExpr, FloatExpr, Variavel, OpExpr} kind;
  union{
    int i_value;          //Valor de um Int

    float f_value;        //Valor de um Float

    char* var;

    struct{               //Expressao Aritmetica
      oper op;
	    A_exp left;
      A_exp right;
    } op_Expr;

    

  }u;
};

//Estrutura Avaliadora de Expressoes Booleanas e Booleanos
struct A_expBool_
{ enum {BoolVal, BoolExpr} kind;
  union{
    boolean bool_value;   //Valor de um booleano

    struct{               //Expressoes Booleanas (maior, igual, menor)
      oper op;
      A_exp left;
      A_exp right;
    }bool_Expr;
    
  }u;

};

struct A_cmd_
{ enum {IfKind, WhileKind, AssignKind, PrintKind, MultipleKind} kind;
  union{

    struct{
      A_expBool test; 
      A_cmd comandos_se_verdadeiro;
      A_cmd comandos_se_falso;
    }IfCmd;

    struct{
      A_expBool test;
      A_cmd comandos;
    }WhileCmd;

    struct                //Atribuicao de um valor a uma variavel
    {
      char* var;
      A_exp val;
    }Assign;

    struct
    {
      A_cmd comandos;
    }PrintCmd;

    struct
    {
      A_cmd left;
      A_cmd right;
    }MultipleCmd;

  }u;
};


A_exp I_Num(int i);							//Inserir valor de inteiro
A_exp F_Num(float i);						//Inserir valor de float
A_exp Var(char* v);
A_exp ExpArit(oper x, A_exp e1, A_exp e2);  //Inserir Operacao aritmetica na estrutura

A_expBool B_Num(char* b, int change);           //Inserir valor booleano
A_expBool ExpBool(oper x, A_exp e1, A_exp e2);	//Inserir Operacao booleana na estrutura

A_cmd IfStatement(A_expBool t, A_cmd cmd_true, A_cmd cmd_false);
A_cmd WhileStatment(A_expBool t, A_cmd cmd);
A_cmd Assign(char* var, A_exp val);     //Associar uma expressao a uma variavel
A_cmd Print(A_cmd c);
A_cmd Multiple(A_cmd c1, A_cmd c2);

void print_tree_cmd(A_cmd e);
void print_tree_exp(A_exp e);
void print_tree_expbool(A_expBool e);

#endif