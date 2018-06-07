#ifndef eval_h
#define eval_h

#include "common_types.h"
#include "environment.h"

list* eval_list(env* e, list* l, bool list_start);
lval* eval_lval(env* e, lval* l);
lval* eval_func(env* e, list * l);
lval* eval(env* e, list* l);
lval* eval_lambda(env* e, list* l);

lval* build_scoped_env(env* e, list* vars, list* vals);

#endif
