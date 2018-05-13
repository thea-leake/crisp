#include "common_types.h"
#include "operators.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>

list* eval_list(list* l, bool list_start){
    lval* v = eval_lval(l->expr);
    if (l->next == NULL){
        return prepend_create(v, NULL);
    }
    list* n = eval_list(l->next, False);

    list* r = prepend_create(v, n);
    if (list_start == True) {
      return prepend_create(eval_func(r), NULL);
   }
   return r;
}

lval* eval_lval(lval* v){
    if (v->type == LVAL_LIST){
        return eval_op(v->list);
    }
    return v;
}

lval* eval_func(list * l){
    lval* func = l->expr;
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
      case EVAL: return eval_op(operands);
      default:   return lval_err("func undefined\n");
   }
}
