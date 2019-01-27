#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common_types.h"
#include "environment.h"
#include "operators.h"
#include "eval.h"

lval* eval_lval(env* e, lval* v){
    if (v->type == LVAL_LIST){
        if (v->list == NULL){
          return copy_lval(e, v);
        }
        list* lc = copy_list(e, v->list);
        lval* resp = eval(e, lc);
        list_del(lc);
        return resp;
    }
    if (v->type == LVAL_SYM){
       return copy_lval(e, get_val(e, v->sym));
    }
    return copy_lval(e,v);
}

lval* eval(env* e, list* l){
   if ( l == NULL ){
      return lval_err("EVAL:Evaluating empty list");
   }
   if (l->expr == NULL){
      return lval_err("EVAL: Operator expression NULL");
   }
   lval* first_expr = eval_lval(e, l->expr);
   if (first_expr->type == LVAL_FUNC ){
      lval* fn_resp = eval_func(e, l);
      return fn_resp;
   } if(first_expr->type == LVAL_LAMBDA){
      lval* lmd_resp = eval_lambda(e, l);
      return lmd_resp;
   }
   if (l->next == NULL){
      return copy_lval(e, eval_lval(e, first_expr));
   }
   return lval_list(copy_list(e, l));
}


lval* eval_lambda(env* e, list * l){
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
    lval* lambda_eval = eval(fn_vars, func_lval->lambda->eval_expr);
    // del will cascade through all lambda vars, but not parent env.
    del_env(fn_vars);
    lval_del(env_set);
    return lambda_eval;
}

lval* eval_func(env* e, list * l){
    lval* func_lval = eval_lval(e, l->expr);
    bltn_ptr func_ptr = get_builtin(func_lval->sym);
    return (func_ptr)(e, l->next);
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
   lval* val = eval_lval(e, vals->expr);
   put_val(e, val, vars->expr->sym, ENV_SCOPED);
   return build_scoped_env(e, vars->next, vals->next);
}
