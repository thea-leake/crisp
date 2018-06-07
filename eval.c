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
      return prepend_create(eval(e, r), NULL);
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

lval* eval(env* e, list* l){
   // the eval_lval here is what is giving the periodic mem leaks
   lval* first_expr = eval_lval(e, l->expr);
   if (first_expr->type == LVAL_FUNC ){
      lval* fn_resp = eval_func(e, l);
      list_del(l);
      return fn_resp;
   } if(first_expr->type == LVAL_LAMBDA){
      lval* lmd_resp = eval_lambda(e, l);
      list_del(l);
      return lmd_resp;
   }
   if (l->next == NULL){
      return first_expr;
   }
   print_lval(e, l->expr);
   print_list(e, l);
   list_del(l);
   return lval_err("First list expression doesn't accept params");
}


lval* eval_lambda(env* e, list * l){
   // and here
    lval* func_lval = eval_lval(e, l->expr);
    list* operands = l->next;
    // create fn env, with parent env to search for matching symbols if no matches
    // found in lambda vars
    env* fn_vars = init_env(e);
    lval* env_set = build_scoped_env(
          fn_vars, func_lval->lambda->var_expr, operands
    );
    if (env_set->type == LVAL_ERR){
       del_env(fn_vars);
       return env_set;
    }
    lval* lambda_eval = eval_func(fn_vars, func_lval->lambda->eval_expr);
    // del will cascade through all lambda vars, but not parent env.
    del_env(fn_vars);
    lval_del(env_set);
    return lambda_eval;

}

lval* eval_func(env* e, list * l){
   // and here
    lval* func_lval = eval_lval(e, l->expr);
    list* operands = l->next;
    return (func_lval->func->func)(e, operands);
}

lval* build_scoped_env(env* e, list* vars, list* vals){
   if (vars == NULL){
      if (vals != NULL){
         return lval_err("Too many arguments for procedure");
      }
      return lval_nil();
   }
   if (vals == NULL){
      return lval_err("Not enough arguments for procedure");
   }
   put_val(e, vals->expr, vars->expr->sym, ENV_SCOPED);
   return build_scoped_env(e, vars->next, vals->next);
}
