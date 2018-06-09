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
lval* let_fn(env* e, list* l);
lval* eq_fn(env* e, list* l);
lval* put_let(env* e, list* l);
lval* quit_fn(env* e, list* l);

lval* sum_numeric(env* e, list* l, float n);
lval* sub_numeric(env* e, list* l, float n);
lval* check_next_eq(env* e, list* l);
lval* get_lval_num(float n);
lval* num_anchor(lval* v);

float get_num(lval* v);
bool is_numeric(lval* l);
bool is_true(lval* v);

#endif
