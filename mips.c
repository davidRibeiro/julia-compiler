#include "tresEnderecos.h"
#include "mips.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* imprimeMipsOP(operators x){
	switch(x){
    case add: return "addi";
    case sub: return "subi";
    case mul: return "mult";
    case divi: return "div";
    case gt: return "ble";
    case lt: return "bge";
    case ge: return "blt";
    case le: return "bgt";
    case eq: return "bne";
    case ne: return "beq";
    default: break;
  }
  return "not";
}



//--------------------------------------------PRINTS-----------------------------------------------
void print_mips_cmd(Compile_A_cmd e){
  if(e == NULL){
    return;
  }
  switch(e->ckind){
    case CAssignKind: 
      //parte esq e expressao e direita e numero
      if(e->u.cAssign.left->ckind == CAritExpr && e->u.cAssign.right->ckind == CInt){
        print_mips_exp(e->u.cAssign.left);
        printf("\t%s $%s , $t%d , %d\n",imprimeMipsOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.cOp_Expr.temp, e->u.cAssign.right->u.i_value);
      }
      //parte esquerda numero e direita exp
      else if(e->u.cAssign.left->ckind == CInt && e->u.cAssign.right->ckind == CAritExpr){
        print_mips_exp(e->u.cAssign.right);
        printf("\t%s $%s , $t%d , %d\n",imprimeMipsOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.i_value, e->u.cAssign.right->u.cOp_Expr.temp);
      }
      //parte esquerda exp e direita exp
      else if(e->u.cAssign.left->ckind == CAritExpr && e->u.cAssign.right->ckind == CAritExpr){
        print_mips_exp(e->u.cAssign.left);
        print_mips_exp(e->u.cAssign.right);
        printf("\t%s $%s , $t%d , $t%d\n",imprimeMipsOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.cOp_Expr.temp, e->u.cAssign.right->u.cOp_Expr.temp); 
      }
      //parte esquerda numero e direita numero
      else{ 
        printf("\t%s $%s , %d , %d\n",imprimeMipsOP(e->u.cAssign.op), e->u.cAssign.var, e->u.cAssign.left->u.i_value, e->u.cAssign.right->u.i_value);
      }
      break;
    case CIfKind:
      //apenas if end
      if(e->u.cIfCmd.comandos_se_falso == NULL){

//-----------------------------------------------MODELO-----------------------------
        int comparaLeft=-1;
        int comparaRight=-1;
        //PARTE ESQUERDA DO TESTE
        if(e->u.cIfCmd.test->u.cBool_Exp.left->ckind == CAritExpr){
          print_mips_exp(e->u.cIfCmd.test->u.cBool_Exp.left);
          comparaLeft=e->u.cIfCmd.test->u.cBool_Exp.left->u.cOp_Expr.temp;
        }
        //caso seja apenas um numero crio um temp para usar na comparacao
        else{
          comparaLeft=proximo_temp();
          printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaLeft, e->u.cIfCmd.test->u.cBool_Exp.left->u.i_value);
        }
        //PARTE DIREITA DO TESTE
        if(e->u.cIfCmd.test->u.cBool_Exp.right->ckind == CAritExpr){
          print_mips_exp(e->u.cIfCmd.test->u.cBool_Exp.right);
          comparaRight=e->u.cIfCmd.test->u.cBool_Exp.right->u.cOp_Expr.temp;
        }
        //caso seja apenas um numero crio um temp para usar na comparacao
        else{
          comparaRight=proximo_temp();
          printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaRight, e->u.cIfCmd.test->u.cBool_Exp.right->u.i_value);
        
        }
        //Imprimir o caso de teste com o salto
        printf("\t%s $t%d , $t%d , L%d\n", imprimeMipsOP(e->u.cIfCmd.test->u.cBool_Exp.op),comparaLeft, comparaRight,e->u.cIfCmd.label_end );
      
//----------------------------------------------------------------------------------

        //print_mips_expBool(e->u.cIfCmd.test);
        //printf("\t $t%d goto L%d\n", e->u.cIfCmd.test->u.cBool_Exp.temp, e->u.cIfCmd.label_end);
        print_mips_cmd(e->u.cIfCmd.comandos_se_verdadeiro);
        printf("L%d:\n", e->u.cIfCmd.label_end);

      }
      //if else end
      else{
        //print_mips_expBool(e->u.cIfCmd.test);
        //printf("\tif_false $t%d goto L%d\n", e->u.cIfCmd.test->u.cBool_Exp.temp, e->u.cIfCmd.label_if_false);
        
        int comparaLeft=-1;
        int comparaRight=-1;
        //PARTE ESQUERDA DO TESTE
        if(e->u.cIfCmd.test->u.cBool_Exp.left->ckind == CAritExpr){
          print_mips_exp(e->u.cIfCmd.test->u.cBool_Exp.left);
          comparaLeft=e->u.cIfCmd.test->u.cBool_Exp.left->u.cOp_Expr.temp;
        }
        //caso seja apenas um numero crio um temp para usar na comparacao
        else{
          comparaLeft=proximo_temp();
          printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaLeft, e->u.cIfCmd.test->u.cBool_Exp.left->u.i_value);
        }
        //PARTE DIREITA DO TESTE
        if(e->u.cIfCmd.test->u.cBool_Exp.right->ckind == CAritExpr){
          print_mips_exp(e->u.cIfCmd.test->u.cBool_Exp.right);
          comparaRight=e->u.cIfCmd.test->u.cBool_Exp.right->u.cOp_Expr.temp;
        }
        //caso seja apenas um numero crio um temp para usar na comparacao
        else{
          comparaRight=proximo_temp();
          printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaRight, e->u.cIfCmd.test->u.cBool_Exp.right->u.i_value);
        
        }
        //Imprimir o caso de teste com o salto
        printf("\t%s $t%d , $t%d , L%d\n", imprimeMipsOP(e->u.cIfCmd.test->u.cBool_Exp.op),comparaLeft, comparaRight,e->u.cIfCmd.label_if_false );
     


        print_mips_cmd(e->u.cIfCmd.comandos_se_verdadeiro);
        printf("\tj L%d\n", e->u.cIfCmd.label_end);
        printf("L%d:\n", e->u.cIfCmd.label_if_false);
        print_mips_cmd(e->u.cIfCmd.comandos_se_falso);
        printf("L%d:\n", e->u.cIfCmd.label_end);
      } 
      break;
    case CWhileKind:
      printf("L%d:\n",e->u.cWhileCmd.labelStart);
      
      int comparaLeft=-1;
      int comparaRight=-1;
          //PARTE ESQUERDA DO TESTE
          if(e->u.cWhileCmd.test->u.cBool_Exp.left->ckind == CAritExpr){
            print_mips_exp(e->u.cWhileCmd.test->u.cBool_Exp.left);
            comparaLeft=e->u.cWhileCmd.test->u.cBool_Exp.left->u.cOp_Expr.temp;
          }
          //caso seja apenas um numero crio um temp para usar na comparacao
          else{
            comparaLeft=proximo_temp();
            printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaLeft, e->u.cWhileCmd.test->u.cBool_Exp.left->u.i_value);
          }
          //PARTE DIREITA DO TESTE
          if(e->u.cWhileCmd.test->u.cBool_Exp.right->ckind == CAritExpr){
            print_mips_exp(e->u.cWhileCmd.test->u.cBool_Exp.right);
            comparaRight=e->u.cWhileCmd.test->u.cBool_Exp.right->u.cOp_Expr.temp;
          }
          //caso seja apenas um numero crio um temp para usar na comparacao
          else{
            comparaRight=proximo_temp();
            printf("\t%s $t%d , $zero , %d\n",imprimeMipsOP(add),comparaRight, e->u.cWhileCmd.test->u.cBool_Exp.right->u.i_value);
          
          }
          //Imprimir o caso de teste com o salto
          printf("\t%s $t%d , $t%d , L%d\n", imprimeMipsOP(e->u.cWhileCmd.test->u.cBool_Exp.op),comparaLeft, comparaRight,e->u.cWhileCmd.labelEnd );
       

      //print_mips_expBool(e->u.cWhileCmd.test);
      //printf("\tif_false $t%d goto L%d\n",e->u.cWhileCmd.test->u.cBool_Exp.temp, e->u.cWhileCmd.labelEnd);
      print_mips_cmd(e->u.cWhileCmd.comandos);
      printf("\tj L%d\n", e->u.cWhileCmd.labelStart);
      printf("L%d:\n", e->u.cWhileCmd.labelEnd);
      break;
    case CMultipleKind:
      print_mips_cmd(e->u.cMultipleCmd.left);
      print_mips_cmd(e->u.cMultipleCmd.right);
      break;
	case CPrintKind:
      print_mips_cmd(e->u.cPrintCmd.comandos);
      printf("\tmove $a0, $Valor_Cmd_Print\n");
      printf("\tli $v0, 1\n");
      printf("\tsyscall\n");
      break;
    default: break;
  }
}


void print_mips_exp(Compile_A_exp e){
  
  if(e == NULL){
    return;
  }
  switch(e->ckind){
    case CAritExpr:
      //Parte esq expressao e parte direita numero
      if(e->u.cOp_Expr.left->ckind == CAritExpr && e->u.cOp_Expr.right->ckind == CInt){
        print_mips_exp(e->u.cOp_Expr.left);
        printf("\t%s $t%d , $t%d , %d\n",imprimeMipsOP(e->u.cOp_Expr.op), e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.i_value);
      }
      //Parte esq numero e parte direita expressao
      else if(e->u.cOp_Expr.right->ckind == CAritExpr && e->u.cOp_Expr.left->ckind == CInt){
        print_mips_exp(e->u.cOp_Expr.right);
        printf("\t%s $t%d , $t%d , %d\n",imprimeMipsOP(e->u.cOp_Expr.op), e->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.i_value);

      }
      //Parte esq expressao e parte direita expressao
      else if(e->u.cOp_Expr.right->ckind == CAritExpr && e->u.cOp_Expr.left->ckind == CAritExpr){
        print_mips_exp(e->u.cOp_Expr.left);
        print_mips_exp(e->u.cOp_Expr.right);
        printf("\t%s $t%d , $t%d , $t%d\n",imprimeMipsOP(e->u.cOp_Expr.op),e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.cOp_Expr.temp, e->u.cOp_Expr.right->u.cOp_Expr.temp );
      }
      //apenas tenho 2 inteiros
      else{
        printf("\t%s $t%d , %d , %d\n",imprimeMipsOP(e->u.cOp_Expr.op),e->u.cOp_Expr.temp, e->u.cOp_Expr.left->u.i_value, e->u.cOp_Expr.right->u.i_value );
      }
      break;
    default: break;
  }

}
