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
    // if builtin
    if (func_lval->type == LVAL_FUNC){
       return (func_lval->func->func)(e, operands);
    }
    // create fn env, with parent env to search for matching symbols if no matches
    // found in lambda vars
    env* fn_vars = init_env(e);
    lval* env_set = build_scoped_env(e, func_lval->lambda->var_expr, operands);
    if (env_set->type == LVAL_ERR){
       del_env(fn_vars);
       return env_set;
    }
    lval_del(env_set);
    lval* lambda_eval = eval_func(fn_vars, func_lval->lambda->eval_expr);
    // del will cascade through all lambda vars, but not parent env.
    del_env(fn_vars);
    return lambda_eval;
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
