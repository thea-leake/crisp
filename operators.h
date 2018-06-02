#ifndef operators_h
#define operators_h

#include "common_types.h"
#include "environment.h"

lval* sum_op(env* e, list* l);
lval* sub_op(env* e, list* l);
lval* mul_op(env* e, list* l);
lval* div_op(env* e, list* l);
lval* mod_op(env* e, list* l);
lval* car_op(env* e, list* l);
lval* cdr_op(env* e, list* l);
lval* list_op(env* e, list* l);
lval* cons_op(env* e, list* l);
lval* eval_op(env* e, list* l);
lval* if_op(env* e,list* l);
lval* and_op(env* e, list* l);
lval* or_op(env* e, list* l);
lval* define_op(env* e, list* l);

bool is_true(lval* v);

#endif
