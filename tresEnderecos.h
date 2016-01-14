#ifndef TRES_ENDERECOS_
#define TRES_ENDERECOS_

#include "estruturas_com_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Compile_A_exp_ *Compile_A_exp;
typedef struct Compile_A_cmd_ *Compile_A_cmd;
typedef struct Compile_A_expBool_ *Compile_A_expBool;
typedef enum { add, sub, mul, divi , gt, lt, eq, ne, ge, le, neg,nil} operators;



//Estrutura Avaliadora de Expressoes Aritmeticas e atribuicoes
struct Compile_A_exp_
{ enum {CInt, CFloat, CAritExpr} ckind;
  union{
    int i_value;          //Valor de um Int

    float f_value;        //Valor de um Float

    struct{               //Expressao Aritmetica
      int temp;
      operators op;
	    Compile_A_exp left;
      Compile_A_exp right;
    } cOp_Expr;

  }u;
};


//Assign compilado para 3 enderecos


struct Compile_A_expBool_
{ enum {CBoolExpKind} ckind;
  union{
    struct{
      int temp;
      operators op;
      Compile_A_exp left;
      Compile_A_exp right;
    }cBool_Exp;
  }u;
};

struct Compile_A_cmd_
{ enum {CAssignKind, CIfKind, CWhileKind,CMultipleKind, CPrintKind} ckind;
  union{
    struct                //Atribuicao de um valor a uma variavel
    {
      char* var;
      operators op;
      Compile_A_exp left;
      Compile_A_exp right;
    }cAssign;
    struct
    {
      Compile_A_expBool test; 
      int label_if_false;
      int label_end;
      Compile_A_cmd comandos_se_verdadeiro;
      Compile_A_cmd comandos_se_falso;      
    }cIfCmd;
    struct
    {
      int labelStart;
      int labelEnd;
      Compile_A_expBool test;
      Compile_A_cmd comandos;      
    }cWhileCmd;
    struct 
    {
      Compile_A_cmd right;
      Compile_A_cmd left;
    }cMultipleCmd;
    struct 
    {
      Compile_A_cmd comandos;
    }cPrintCmd;
  }u;

};


Compile_A_exp Compile_I_Num(int i);							//Inserir valor de inteiro
Compile_A_exp Compile_ExpArit(int temp, operators x, Compile_A_exp e1, Compile_A_exp e2);  //Inserir Operacao aritmetica na estrutura

Compile_A_cmd Compile_Assign(char* var,operators x, Compile_A_exp e1, Compile_A_exp e2);     //Associar uma expressao a uma variavel
Compile_A_cmd Compile_IfStatement(Compile_A_expBool b, int l1, int l2, Compile_A_cmd c1, Compile_A_cmd c2);
Compile_A_cmd Compile_While(Compile_A_expBool b, int l1, int l2, Compile_A_cmd c);
Compile_A_cmd Compile_Multiple(Compile_A_cmd c1, Compile_A_cmd c2);
Compile_A_cmd Compile_Print(Compile_A_cmd c);

Compile_A_expBool Compile_ExpBool(int t, operators x, Compile_A_exp e1, Compile_A_exp e2);

void traduzirPrograma(A_cmd prog);

Compile_A_cmd traduzirCmd(A_cmd e);
Compile_A_exp traduzir_Aexp(A_exp e);
Compile_A_expBool traduzir_Bexp(A_expBool e);

int proximo_temp();
int proxima_label();
char* imprimeOP(operators x);

void print_tresEnderecos_cmd(Compile_A_cmd e);
void print_tresEnderecos_exp(Compile_A_exp e);
void print_tresEnderecos_expBool(Compile_A_expBool e);


#endif