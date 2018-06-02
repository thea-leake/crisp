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
        return eval_fn(e, v->list);
    }
    if (v->type == LVAL_SYM){
       return get_val(e, v->sym);
    }
    return v;
}

lval* eval_func(env* e, list * l){
    lval* func_lval = eval_lval(e, l->expr);
    list* operands = l->next;
    return (func_lval->func->func)(e, operands);
}
