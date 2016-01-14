#include "estruturas_com_cmd.h"
#include "tresEnderecos.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------.h-------------------------------//


const char* simbolo[] = {"+","-","*","/"};
const char* operacao[] = { "ADD", "SUB", "MULT", "DIV", "GREATER", "LOWER", "EQUALS", "NOTEQUAL", "GREATEQUAL", "LOWEQUAL", "NEGATE"};
const char* val_bool[]={"False", "True"};
const char* comandos[] = {"IF", "ELSE", "WHILE", "END"};



//------------------------------------EXPRESSOES ARITMETICAS------------------------------//
A_exp I_Num(int i){ 
  A_exp p= malloc(sizeof(struct A_exp_));
  p->kind = IntExpr;
  p->u.i_value=i;
  return p;	
}

A_exp F_Num(float i){
  A_exp p = malloc(sizeof(struct A_exp_));
  p->kind = FloatExpr;
  p->u.f_value = i;
  return p;
}



A_exp Var(char* v){
  A_exp p = malloc(sizeof(struct A_exp_));
  p->kind = Variavel;
  p->u.var = v;
  return p;
}



A_exp ExpArit(oper x, A_exp e1, A_exp e2)
{ A_exp p = malloc(sizeof(struct A_exp_));
  p->kind=OpExpr;
  p->u.op_Expr.op = x;
  p->u.op_Expr.left = e1;
  p->u.op_Expr.right = e2;
  return p;
}



//----------------------------------EXPRESSOES BOOLEANAS--------------------------------

A_expBool B_Num(char* b, int change){
  A_expBool p = malloc(sizeof(struct A_expBool_));
  p->kind = BoolVal;
  if(strcmp(b,"True")==0)
  p->u.bool_value = True;
  if(strcmp(b,"False")==0)
  p->u.bool_value = False;
  return p;
}

A_expBool ExpBool(oper x, A_exp e1, A_exp e2){
  A_expBool p = malloc(sizeof(struct A_exp_));
  p->kind=BoolExpr;
  p->u.bool_Expr.op = x;
  p->u.bool_Expr.left = e1;
  p->u.bool_Expr.right = e2;
  return p;
}

// ----------------------------------COMANDOS --------------------------------------------

A_cmd IfStatement(A_expBool t, A_cmd cmd_true, A_cmd cmd_false){
  A_cmd p = malloc(sizeof (struct A_cmd_));
  p->kind = IfKind;
  p->u.IfCmd.test = t;
  p->u.IfCmd.comandos_se_verdadeiro = cmd_true;
  p->u.IfCmd.comandos_se_falso = cmd_false;

  return p;
}

A_cmd WhileStatment(A_expBool t, A_cmd cmd){
  A_cmd p = malloc(sizeof (struct A_cmd_));
  p->kind = WhileKind;
  p->u.WhileCmd.test = t;
  p->u.WhileCmd.comandos = cmd;
  
  return p;
}

A_cmd Assign(char* var, A_exp val){
  A_cmd p= malloc(sizeof(struct A_cmd_));
  p->kind = AssignKind;
  p->u.Assign.var=var;
  p->u.Assign.val=val;
  return p;
}

A_cmd Print(A_cmd c){
  A_cmd p= malloc(sizeof(struct A_cmd_));
  p->kind = PrintKind;
  p->u.PrintCmd.comandos=c;
  return p;
}

A_cmd Multiple(A_cmd c1, A_cmd c2){
  A_cmd p = malloc(sizeof(struct A_cmd_));
  p->kind= MultipleKind;
  p->u.MultipleCmd.left = c1;
  p->u.MultipleCmd.right = c2;
  return p;
}


//--------------------------------------------PRINTS-----------------------------------------------
void print_tree_cmd(A_cmd e){
  if (e==NULL){
    return;
  }
  switch(e->kind){
    case IfKind:
      printf("If (");
      print_tree_expbool(e->u.IfCmd.test);
      printf(") then{ \n\t");
      print_tree_cmd(e->u.IfCmd.comandos_se_verdadeiro);
      printf("}\n");
      if(e->u.IfCmd.comandos_se_falso!=NULL){
        printf("Else{\n\t");
        print_tree_cmd(e->u.IfCmd.comandos_se_falso);
        printf("}\n");
      }
      break;
    case WhileKind:
      printf("While (");
      print_tree_expbool(e->u.WhileCmd.test);
      printf("){\n\t");
      print_tree_cmd(e->u.WhileCmd.comandos);
      printf("}\n");
      break;
    case AssignKind:
      printf("Assign (Var %s)",e->u.Assign.var);
      print_tree_exp(e->u.Assign.val);
      break;
    case MultipleKind:
      print_tree_cmd(e->u.MultipleCmd.left);
      print_tree_cmd(e->u.MultipleCmd.right);
      break;
    case PrintKind:
      printf("PRINTLN(");
      print_tree_cmd(e->u.PrintCmd.comandos);
      printf(")\n");
      break;
    default : break;
  }
}

//PRINT IGUAL AO INPUT
void print_tree_exp(A_exp e){
  
  if (e==NULL){
    printf(" NULL ");
    return;
  }
  switch(e->kind){
    case IntExpr: 
      printf("(NUM %d)",e->u.i_value); 
      break;
    case FloatExpr: 
      printf("(FLOAT %f)",e->u.f_value);
      break;
    case OpExpr: 
      printf("(%s", operacao[e->u.op_Expr.op]);
      print_tree_exp(e->u.op_Expr.left);
      print_tree_exp(e->u.op_Expr.right);
      printf(")");
      break;
    default : break;
  }

}

void print_tree_expbool(A_expBool e){
  if (e==NULL){
    printf(" NULL ");
    return;
  }
  switch(e->kind){
    case BoolExpr:
      printf("(%s", operacao[e->u.bool_Expr.op]);
      print_tree_exp(e->u.bool_Expr.left);
      print_tree_exp(e->u.bool_Expr.right);
      printf(")");
      break;
    case BoolVal:
      printf("BOOL %s",  val_bool[e->u.bool_value]);
    default : break;   
  }

}
  
