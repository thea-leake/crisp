#ifndef operators_h
#define operators_h

#include "common_types.h"
#include "environment.h"

lval* sum_fn(env* e, list* l);
lval* sub_fn(env* e, list* l);
lval* mul_fn(env* e, list* l);
lval* div_fn(env* e, list* l);
lval* mod_fn(env* e, list* l);
lval* car_fn(env* e, list* l);
lval* cdr_fn(env* e, list* l);
lval* list_fn(env* e, list* l);
lval* cons_fn(env* e, list* l);
lval* eval_fn(env* e, list* l);
lval* if_fn(env* e,list* l);
lval* and_fn(env* e, list* l);
lval* or_fn(env* e, list* l);
lval* define_fn(env* e, list* l);
lval* lambda_fn(env* e, list* l);

bool is_true(lval* v);

#endif
