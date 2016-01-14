#include "estruturas_com_cmd.h"
#include "tresEnderecos.h"
#include "mips.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------.h-------------------------------//

//Flag que controla o que e impresso
//1 - Arvore Sintatica
//2 - Codigo 3 Enderecos
//3 - Codigo Mips
int flag = 3;


int tempCount=0;
int labelCount=1;


operators changeOP(oper o){
  switch(o){
    case ADD: return add;
    case SUB: return sub;
    case MULT: return mul;
    case DIV: return divi;
    case EQUALS: return eq;
    case NOTEQUAL: return ne;
    case GREATER: return gt;
    case LOWER: return lt;
    case GREATEQUAL: return ge;
    case LOWEQUAL: return le;
    default: break;
  }
  return nil;
}

//funcao principal que vai gerar os contrutores de estruturas de 3 enderecos e imprimir
void traduzirPrograma(A_cmd prog){
  A_cmd root = prog;
  
  Compile_A_cmd compileProg = traduzirCmd(prog);
  
  if(flag==1){
    //Imprime Arvore Abstrata
    print_tree_cmd(root);
  }
  if(flag==2){ 
    //Imprime 3 Enderecos
    print_tresEnderecos_cmd(compileProg);
  }
  if(flag==3){
    //Imprime Mips
    print_mips_cmd(compileProg);
  }

}

Compile_A_cmd traduzirCmd(A_cmd e){
  if(e == NULL){
    return NULL;
  }
  Compile_A_cmd p;
  switch(e->kind){
    case AssignKind:
    {
      //Crio 2 estruturas para atribuicao esquerda e direita
      Compile_A_exp valorLeft;
      Compile_A_exp valorRight;
      operators ass_op;
      int control=0;
      
      A_exp aux = e->u.Assign.val; 

      //ASSIGN de expressao
      if(aux->kind == OpExpr){                    
      
        //analisa parte esquerda
        if(aux->u.op_Expr.left->kind != IntExpr){  //ASSIGN de mais que 2 elementos totais EX: x=1+2+3
          Compile_A_exp valor = traduzir_Aexp(aux->u.op_Expr.left);
          valorLeft = Compile_ExpArit(proximo_temp(), valor->u.cOp_Expr.op, valor->u.cOp_Expr.left, valor->u.cOp_Expr.right);
                   
        }
        else{                                     
          valorLeft = Compile_I_Num(aux->u.op_Expr.left->u.i_value);
        }
        //analisa parte direita
        if(aux->u.op_Expr.right->kind != IntExpr){
          Compile_A_exp valor = traduzir_Aexp(aux->u.op_Expr.right);
          valorRight=Compile_ExpArit(proximo_temp(), valor->u.cOp_Expr.op, valor->u.cOp_Expr.left, valor->u.cOp_Expr.right);
        }
        else{
          valorRight = Compile_I_Num(aux->u.op_Expr.right->u.i_value);
        }
        
      }
      //ASSIGN de 1 elemento apenas total EX: x=1
      else{                                       
        valorLeft = Compile_I_Num(aux->u.i_value);
        valorRight = Compile_I_Num(0);
        ass_op = add;
        control=1;
      }
      //No final e juntar as 2 partes generalizadas em cima
     
      if(control!=1){
        ass_op = changeOP(aux->u.op_Expr.op);
      }
      p = Compile_Assign(e->u.Assign.var, ass_op, valorLeft, valorRight);

      break;
    }
    case IfKind:
    {

      Compile_A_expBool test = traduzir_Bexp(e->u.IfCmd.test);
      Compile_A_cmd cmdTrue = traduzirCmd(e->u.IfCmd.comandos_se_verdadeiro);
      Compile_A_cmd cmdFalse = traduzirCmd(e->u.IfCmd.comandos_se_falso);

      //Testar se e if end   ou if else end     para nao usar mais labels 
      if(cmdFalse == NULL){
        p=Compile_IfStatement(test,proxima_label(),0,cmdTrue,cmdFalse);
      }
      else{
        p=Compile_IfStatement(test,proxima_label(),proxima_label(),cmdTrue,cmdFalse);
      }

      break;
    }
    case WhileKind:
    {
       Compile_A_expBool test = traduzir_Bexp(e->u.WhileCmd.test);
       Compile_A_cmd cmds = traduzirCmd(e->u.WhileCmd.comandos);

       p=Compile_While(test,proxima_label(),proxima_label(),cmds);

      break;
    }
    case MultipleKind:
    {
      Compile_A_cmd left = traduzirCmd(e->u.MultipleCmd.left);
      Compile_A_cmd right = traduzirCmd(e->u.MultipleCmd.right);
      p=Compile_Multiple(left,right);
      break;
    }
    case PrintKind:
    {
      Compile_A_cmd comandos = traduzirCmd(e->u.PrintCmd.comandos);
      p=Compile_Print(comandos);
      break;
    }
    default: break;
  }
  return p;
}

//Decompor expressoes mais complexas, nunca atinge o I_Num ou o F_Num
Compile_A_exp traduzir_Aexp(A_exp e){
  Compile_A_exp p;
 

  Compile_A_exp auxLeft;
  Compile_A_exp auxRight;
  Compile_A_exp valorLeft;
  Compile_A_exp valorRight;

  switch (e->kind){
    case OpExpr: 
    {

      //ainda posso decompor
      if(e->u.op_Expr.left->kind != IntExpr){
        valorLeft = traduzir_Aexp(e->u.op_Expr.left);
        //valorRight=Compile_I_Num(e->u.op_Expr.right->u.i_value);  
      }
      //parte esq e numero faco a juncao das metades, pois right e sempre numero
      else{
        valorLeft=Compile_I_Num(e->u.op_Expr.left->u.i_value);
        //valorRight=Compile_I_Num(e->u.op_Expr.right->u.i_value);  
      }

      if(e->u.op_Expr.right->kind != IntExpr){
        valorRight=traduzir_Aexp(e->u.op_Expr.right);
      }
      else{
        valorRight=Compile_I_Num(e->u.op_Expr.right->u.i_value);
      }


      p=Compile_ExpArit(proximo_temp(),changeOP(e->u.op_Expr.op),valorLeft,valorRight);
      
      break;
    }
    default : break;
  }
  return p;
}

operators changeOPBool(operators x){
switch(x){
  case eq: return ne;
  case ne: return eq;
  case gt: return le;
  case lt: return ge;
  case ge: return lt;
  case le: return gt;
  default: break;
}
return nil;

}

Compile_A_expBool traduzir_Bexp(A_expBool e){
  Compile_A_expBool p;
  switch (e->kind){
    case BoolVal: 
    {
      Compile_A_exp left=Compile_I_Num((int)e->u.bool_value);
      Compile_A_exp right=Compile_I_Num((int)e->u.bool_value);
      if(e->u.bool_value == True){
        p=Compile_ExpBool(proximo_temp(),eq,left,right);
      }
      else{
        p=Compile_ExpBool(proximo_temp(),ne,left,right);
      }
      break;
    }
    case BoolExpr: 
    {
      Compile_A_exp left;
      Compile_A_exp right;
      //Analisa parte esquerda
      if(e->u.bool_Expr.left->kind == OpExpr){
        left = traduzir_Aexp(e->u.bool_Expr.left);
      }
      else{
        left = Compile_I_Num((int)e->u.bool_Expr.left->u.i_value);
      }
      //Analisa parte direita
      if(e->u.bool_Expr.right->kind == OpExpr){
        right =traduzir_Aexp (e->u.bool_Expr.right);
      }
      else{
        right = Compile_I_Num((int)e->u.bool_Expr.right->u.i_value);
      }

      //forma estrutura
      p= Compile_ExpBool(proximo_temp(),changeOP(e->u.bool_Expr.op),left,right);

      break;
    }
    default: break;
  }

  //p->u.cBool_Exp.op=changeOPBool(p->u.cBool_Exp.op);
  return p;
}

char* compile_simbolo[] = {"add","sub","mul","div", "gt", "lt", "eq", "ne", "ge", "le", "not"};
char* compile_val_bool[]={"False", "True"};



//----------------------------------EXPRESSOES BOOLEANAS ---------------------------------

Compile_A_expBool Compile_ExpBool(int t, operators x, Compile_A_exp e1, Compile_A_exp e2){
  Compile_A_expBool p = malloc(sizeof(struct Compile_A_expBool_));
  p->ckind = CBoolExpKind;
  p->u.cBool_Exp.temp = t;
  p->u.cBool_Exp.op = x;
  p->u.cBool_Exp.left=e1;
  p->u.cBool_Exp.right=e2;
  return p;
}



//------------------------------------EXPRESSOES ARITMETICAS------------------------------//
Compile_A_exp Compile_I_Num(int i){ 
  Compile_A_exp p= malloc(sizeof(struct Compile_A_exp_));
  p->ckind = CInt;
  p->u.i_value=i;
  return p;	
}



Compile_A_exp Compile_ExpArit(int t, operators x, Compile_A_exp e1, Compile_A_exp e2)
{ Compile_A_exp p = malloc(sizeof(struct Compile_A_exp_));
  p->ckind=CAritExpr;
  p->u.cOp_Expr.temp = t;
  p->u.cOp_Expr.op = x;
  p->u.cOp_Expr.left = e1;
  p->u.cOp_Expr.right = e2;
  
  return p;
}



// ----------------------------------COMANDOS --------------------------------------------



Compile_A_cmd Compile_Assign(char* var, operators x, Compile_A_exp e1, Compile_A_exp e2){
  Compile_A_cmd p= malloc(sizeof(struct Compile_A_cmd_));
  p->ckind = CAssignKind;
  p->u.cAssign.var=var;
  p->u.cAssign.op = x;
  p->u.cAssign.left=e1;
  p->u.cAssign.right=e2;
  return p;
}

Compile_A_cmd Compile_IfStatement(Compile_A_expBool b, int l1, int l2, Compile_A_cmd c1, Compile_A_cmd c2){
  Compile_A_cmd p = malloc(sizeof(struct Compile_A_cmd_));
  p->ckind = CIfKind;
  p->u.cIfCmd.test=b;
  p->u.cIfCmd.label_if_false=l2;
  p->u.cIfCmd.label_end=l1;
  p->u.cIfCmd.comandos_se_verdadeiro=c1;
  p->u.cIfCmd.comandos_se_falso=c2;
  return p;
}

Compile_A_cmd Compile_While(Compile_A_expBool b, int l1, int l2, Compile_A_cmd c){
  Compile_A_cmd p = malloc(sizeof(struct Compile_A_cmd_));
  p->ckind = CWhileKind;
  p->u.cWhileCmd.labelStart=l2;
  p->u.cWhileCmd.labelEnd=l1;
  p->u.cWhileCmd.test=b;
  p->u.cWhileCmd.comandos=c;
  return p;
 }

 Compile_A_cmd Compile_Multiple(Compile_A_cmd c1, Compile_A_cmd c2){
  Compile_A_cmd p = malloc(sizeof(struct Compile_A_cmd_));
  p->ckind=CMultipleKind;
  p->u.cMultipleCmd.left=c1;
  p->u.cMultipleCmd.right=c2;
  return p;
 }
Compile_A_cmd Compile_Print(Compile_A_cmd c){
  Compile_A_cmd p = malloc(sizeof(struct Compile_A_cmd_));
  p->ckind=CPrintKind;
  p->u.cPrintCmd.comandos=c;
  return p;
}


//--------------------------------------------PRINTS-----------------------------------------------
void print_tresEnderecos_cmd(Compile_A_cmd e){
  if(e == NULL){
    return;
  }
  switch(e->ckind){
    case CAssignKind: 
      //parte esq e expressao e direita e numero
      if(e->u.cAssign.left->ckind == CAritExpr && e->u.cAssign.right->ckind == CInt){
        print_tresEnderecos_exp(e->u.cAssign.left);
        printf("\t%s %s t%d %d\n",imprimeOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.cOp_Expr.temp, e->u.cAssign.right->u.i_value);
      }
      //parte esquerda numero e direita exp
      else if(e->u.cAssign.left->ckind == CInt && e->u.cAssign.right->ckind == CAritExpr){
        print_tresEnderecos_exp(e->u.cAssign.right);
        printf("\t%s %s %d t%d\n",imprimeOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.i_value, e->u.cAssign.right->u.cOp_Expr.temp);
      }
      //parte esquerda exp e direita exp
      else if(e->u.cAssign.left->ckind == CAritExpr && e->u.cAssign.right->ckind == CAritExpr){
        print_tresEnderecos_exp(e->u.cAssign.left);
        print_tresEnderecos_exp(e->u.cAssign.right);
        printf("\t%s %s t%d t%d\n",imprimeOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.cOp_Expr.temp, e->u.cAssign.right->u.cOp_Expr.temp); 
      }
      //parte esquerda numero e direita numero
      else{ 
        printf("\t%s %s %d %d\n",imprimeOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.i_value, e->u.cAssign.right->u.i_value);
      }
      break;
    case CIfKind:
      //apenas if end
      if(e->u.cIfCmd.comandos_se_falso == NULL){
         print_tresEnderecos_expBool(e->u.cIfCmd.test);
         printf("\tif_false t%d goto L%d\n", e->u.cIfCmd.test->u.cBool_Exp.temp, e->u.cIfCmd.label_end);
         print_tresEnderecos_cmd(e->u.cIfCmd.comandos_se_verdadeiro);
         printf("Label L%d:\n", e->u.cIfCmd.label_end);

      }
      //if else end
      else{
        print_tresEnderecos_expBool(e->u.cIfCmd.test);
        printf("\tif_false t%d goto L%d\n", e->u.cIfCmd.test->u.cBool_Exp.temp, e->u.cIfCmd.label_if_false);
        print_tresEnderecos_cmd(e->u.cIfCmd.comandos_se_verdadeiro);
        printf("\tgoto L%d\n", e->u.cIfCmd.label_end);
        printf("Label L%d:\n", e->u.cIfCmd.label_if_false);
        print_tresEnderecos_cmd(e->u.cIfCmd.comandos_se_falso);
        printf("Label L%d:\n", e->u.cIfCmd.label_end);
      } 
      break;
    case CWhileKind:
      printf("Label L%d:\n",e->u.cWhileCmd.labelStart);
      print_tresEnderecos_expBool(e->u.cWhileCmd.test);
      printf("\tif_false t%d goto L%d\n",e->u.cWhileCmd.test->u.cBool_Exp.temp, e->u.cWhileCmd.labelEnd);
      print_tresEnderecos_cmd(e->u.cWhileCmd.comandos);
      printf("\tgoto L%d\n", e->u.cWhileCmd.labelStart);
      printf("Label L%d:\n", e->u.cWhileCmd.labelEnd);
      break;
    case CMultipleKind:
      print_tresEnderecos_cmd(e->u.cMultipleCmd.left);
      print_tresEnderecos_cmd(e->u.cMultipleCmd.right);
      break;
    case CPrintKind:
      printf("Println(");
      print_tresEnderecos_cmd(e->u.cPrintCmd.comandos);
      printf(")\n");
      break;
    default: break;
  }
}


void print_tresEnderecos_exp(Compile_A_exp e){
  
  if(e == NULL){
    return;
  }
  switch(e->ckind){
    case CAritExpr:
      //Parte esq expressao e parte direita numero
      if(e->u.cOp_Expr.left->ckind == CAritExpr && e->u.cOp_Expr.right->ckind == CInt){
        print_tresEnderecos_exp(e->u.cOp_Expr.left);
        printf("\t%s t%d t%d %d\n",imprimeOP(e->u.cOp_Expr.op), e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.i_value);
      }
      //Parte esq numero e parte direita expressao
      else if(e->u.cOp_Expr.right->ckind == CAritExpr && e->u.cOp_Expr.left->ckind == CInt){
        print_tresEnderecos_exp(e->u.cOp_Expr.right);
        printf("\t%s t%d t%d %d\n",imprimeOP(e->u.cOp_Expr.op), e->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.i_value);

      }
      //Parte esq expressao e parte direita expressao
      else if(e->u.cOp_Expr.right->ckind == CAritExpr && e->u.cOp_Expr.left->ckind == CAritExpr){
        print_tresEnderecos_exp(e->u.cOp_Expr.left);
        print_tresEnderecos_exp(e->u.cOp_Expr.right);
        printf("\t%s t%d t%d t%d\n",imprimeOP(e->u.cOp_Expr.op),e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.cOp_Expr.temp );
      }
      //apenas tenho 2 inteiros
      else{ 
        printf("\t%s t%d %d %d\n",imprimeOP(e->u.cOp_Expr.op), e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.i_value, e->u.cOp_Expr.right->u.i_value );
      }
      break;
    default: break;
  }

}

void print_tresEnderecos_expBool(Compile_A_expBool e){
  if(e==NULL){
    return;
  }
  switch(e->ckind){
    case CBoolExpKind:
      //sao 2 expressoes
      if(e->u.cBool_Exp.left->ckind == CAritExpr && e->u.cBool_Exp.right->ckind == CAritExpr){
        print_tresEnderecos_exp(e->u.cBool_Exp.left);
        print_tresEnderecos_exp(e->u.cBool_Exp.right);
        printf("\t%s t%d t%d t%d\n",imprimeOP(e->u.cBool_Exp.op),e->u.cBool_Exp.temp, e->u.cBool_Exp.left->u.cOp_Expr.temp, e->u.cBool_Exp.right->u.cOp_Expr.temp );
      }
      //parte esq expressao, parte direita numero
      else if(e->u.cBool_Exp.left->ckind == CAritExpr && e->u.cBool_Exp.right->ckind == CInt){
        print_tresEnderecos_exp(e->u.cBool_Exp.left);
        printf("\t%s t%d t%d %d\n",imprimeOP(e->u.cBool_Exp.op),e->u.cBool_Exp.temp, e->u.cBool_Exp.left->u.cOp_Expr.temp, e->u.cBool_Exp.right->u.i_value );
      }
      //parte esq numero, parte direita expressao
      else if(e->u.cBool_Exp.left->ckind == CInt && e->u.cBool_Exp.right->ckind == CAritExpr){
        print_tresEnderecos_exp(e->u.cBool_Exp.right);
        printf("\t%s t%d %d t%d\n",imprimeOP(e->u.cBool_Exp.op),e->u.cBool_Exp.temp, e->u.cBool_Exp.left->u.i_value, e->u.cBool_Exp.right->u.cOp_Expr.temp);
      }
      //2 numeros
      else{
        printf("\t%s t%d %d %d\n",imprimeOP(e->u.cBool_Exp.op),e->u.cBool_Exp.temp,e->u.cBool_Exp.left->u.i_value,e->u.cBool_Exp.right->u.i_value);
      }

      break;
    default: break;
  }
}

char* imprimeOP(operators x){
  return compile_simbolo[x];
}

int proximo_temp(){
  return tempCount++;
}

int proxima_label(){
  return labelCount++;
}
