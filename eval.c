#include <stdlib.h>
#include <stdio.h>
#include "common_types.h"
#include "environment.h"
#include "operators.h"
#include "eval.h"

list* eval_list(env* e, list* l, bool list_start){
    lval* v = eval_lval(e, l->expr);
    if (l->next == NULL){
        return prepend_create(v, NULL);
    }
    list* n = eval_list(e, l->next, False);

    list* r = prepend_create(v, n);
    if (list_start == True) {
      return prepend_create(eval_func(e, r), NULL);
   }
   return r;
}

lval* eval_lval(env* e, lval* v){
    if (v->type == LVAL_LIST){
        return eval_op(e, v->list);
    }
    if (v->type == LVAL_SYM){
       return get_val(e, v->sym);
    }
    return v;
}

lval* eval_func(env* e, list * l){
    lval* func = eval_lval(e, l->expr);
    list* operands = l->next;
    switch(func->func){
      case SUM:  return sum_op(operands);
      case DIFF:  return sub_op(operands);
      case MUL:  return mul_op(operands);
      case DIV:  return div_op(operands);
      case MOD: return mod_op(operands);
      case CAR:return car_op(operands);
      case CDR: return cdr_op(operands);
      case LIST: return list_op(operands);
      case CONS: return cons_op(operands);
      case EVAL: return eval_op(e, operands);
      case IF: return if_op(e, operands);
      case AND: return and_op(e, operands);
      case OR: return or_op(e, operands);
      default:   return lval_err("func undefined\n");
   }
}
