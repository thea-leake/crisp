#include "common_types.h"
#include "operators.h"
#include "environment.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


lval* sum_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:+: missing operands");
    }
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr)){
        return sum_numeric(e, l, 0);
    }
    return lval_err("#builtin:+: sum for type not implemented");
}

lval* sum_numeric(env* e, list* l, float accum){
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:+: incompatible types for sum operation");
    }
    float sum = get_num(expr) + accum;
    if (l->next == NULL){
        return get_lval_num(sum);
    }
    return sum_numeric(e, l->next, sum);
}

lval* sub_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:-: missing operands");
    }
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:-: difference for type not implemented");
    }
    if (l->next == NULL){
        return get_lval_num( -1 * get_num(expr));
    }
    return sub_numeric(e, l->next, get_num(expr));
}

lval* sub_numeric(env* e, list* l, float accum){
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:-: incompatible types for difference operation");
    }
    float diff = accum - get_num(expr);
    if (l->next == NULL){
        return get_lval_num(diff);
    }
    return sub_numeric(e, l->next, diff);
}

lval* mul_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:*: missing operands");
    }
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:*: product for type unavailable");
    }
    if (l->next == NULL){
        // auto downcasting if available for consistency
        return get_lval_num(get_num(expr));
    }
    return mul_numeric(e, l->next, get_num(expr));
}

lval* mul_numeric(env* e, list* l, float accum){
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:*:numeric: product for type unavailable");
    }
    float product = accum * get_num(expr);
    if (l->next == NULL){
        return get_lval_num(product);
    }
    return mul_numeric(e, l->next, product);
}

lval* div_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:/: missing operands");
    }
    lval* expr = eval_lval(e, l->expr);
    if (is_numeric(expr) == False){
        return lval_err("#builtin:/: quotient for type unavailable");
    } if (l->next == NULL){
        return get_lval_num(1 / get_num(expr));
    }
    return div_numeric(e, l->next, get_num(expr));
}

lval* div_numeric(env* e, list* l, float accum){
    lval* expr = eval_lval(e, l->expr);
    if (get_num(expr) == 0){
        return lval_err("#builtin:/:numeric: unable to divide by 0");
    }
    float quotient = accum / get_num(expr);
    if (l->next == NULL){
        return get_lval_num(quotient);
    }
    return div_numeric(e, l->next, quotient);
}

lval* mod_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:%: missing operands");
    }
    if (l->next == NULL){
        return lval_err("#builtin:%: missing operand");
    }
    if (l->next->next != NULL){
        return lval_err("#builtin:%: too many operands");
    }
    lval* expr = eval_lval(e, l->expr);
    lval* accum = eval_lval(e,l->next->expr);
    if ((int_compat(expr) == False)||(int_compat(accum) == False)){
        return lval_err("Operand is not int compatible");
    }

    int expr_val = (int) get_num(expr);
    int accum_val = (int) get_num(accum);

    int modulo = expr_val % accum_val;

    return lval_num_int(modulo);
}

lval* get_lval_num(float n){
    int n_int = (int) n;
    if (n == n_int){
        return lval_num_int(n_int);
    }
    return lval_num_float(n);
}

float get_num(lval* v){
    if (v->type == LVAL_NUM_INT){
        return (float) v->num_int;
    }
    return v->num_float;
}

bool is_numeric(lval* l){
    switch (l->type){
        case LVAL_NUM_INT: return True;
        case LVAL_NUM_FLOAT: return True;
    }
    return False;
}

lval* check_next_eq(env* e, list* l){
    if (l->next == NULL){
        return lval_bool(True);
    }
    return eq_fn(e, l);
}

lval* eq_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:=: missing operands");
    }
    if (l->expr == NULL){
        return lval_err("#builtin:=: first argument is missing");
    }
    if (l->next == NULL){
        return lval_err("#builtin:=: unmatched operand for equality comparison");
    }
    lval* first = eval_lval(e, first_expr(l));
    lval* next = eval_lval(e, first_expr(rest_expr(l)));
    if (is_numeric(first) && is_numeric(next)) {
        if (get_num(first) == get_num(next)) {
            if (rest_expr(l)->next == NULL){
                return lval_bool(True);
            }
            return check_next_eq(e, rest_expr(l));
        }
        return lval_bool(False);
    }
    if (first->type != next->type){
        return lval_bool(False);
    }
    if (first->type == LVAL_STR) {
        if (strcmp(first->str, next->str) == 0){
            return check_next_eq(e, rest_expr(l));
        }
        return lval_bool(False);
    }
    if (first->type == LVAL_BOOL){
        if (first->bool == next->bool) {
            return check_next_eq(e, rest_expr(l));
        }
        return lval_bool(False);
    }
    if (first->type == LVAL_NIL && next->type == LVAL_NIL){
        return check_next_eq(e, rest_expr(l));
    }
    return lval_err("#builtin:=: no comparison available for type");
}

lval* car_fn(env* e, list* l){
    if (l == NULL ){
        return lval_err("#builtin:car: received no operands");
    }
    if (l->next != NULL){
        return lval_err("#builtin:car: Too many operands, only takes one list");
    }
    if (l->expr->type == LVAL_SYM){
        lval* v = get_val(e, l->expr->sym);
        return car_fn(e, v->list);
    }
    if (l->expr->type != LVAL_LIST){
        return lval_err("#builtin:car: expected list operand");
    }
    if (l->expr->list == NULL){
        return lval_nil();
    }
    return first_expr(l->expr->list);
}


lval* cdr_fn(env* e, list* l){
    if (l == NULL ){
        return lval_err("#builtin:cdr: received no operands");
    }
    if (l->next != NULL){
        return lval_err("#builtin:cdr: Too many operands, only takes one list");
    }
    if (l->expr->type == LVAL_SYM){
        lval* v = get_val(e, l->expr->sym);
        return cdr_fn(e, v->list);
    }
    if (l->expr->type != LVAL_LIST){
        return lval_err("#builtin:cdr: expected list arg");
    }
    if (l->expr->list == NULL){
        return lval_nil();
    }
    if (l->expr->list->next == NULL){
        return lval_nil();
    }
    return lval_list(rest_expr(l->expr->list));
}


lval* list_fn(env* e, list* l){
    (void) e;
    return lval_list(l);
}

lval* cons_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:cons: missing operand list");
    }
    if (l->next == NULL){
        return lval_err("#builtin:cons: no list to cons to");
    } if (l->next->next != NULL){
        return lval_err("#builtin:cons: too many arguments, expect lval and list");
    }
    lval* cons_expr = cons_list(e, l->next->expr);
    if (cons_expr->type == LVAL_ERR){
        return cons_expr;
    }
    lval* x = copy_lval(e, eval_lval(e, l->expr));
    list* y = copy_list(e, cons_expr->list);
    //might use copy_list, depending on ease of garbage collection
    list* n = list_prepend(y, x);
    return lval_list(n);
}

lval* eval_fn(env* e, list* l){
    if (l == NULL){
        return lval_func("eval");
    }
    list* lc = copy_list(e, l);
    lval* resp = eval(e, lc);
    list_del(lc);
    return resp;
}

lval* cons_list(env* e, lval* v){
    if (v->type == LVAL_LIST) {
        return v;
    } if (v->type == LVAL_SYM){
        lval* val = get_val(e, v->sym);
            if (val->type == LVAL_LIST){
                return val;
            }
    }
    return lval_err("#builtin:cons:cons_list: second param not a list");
}

bool is_true(lval* v){
    int t = v->type;
    if (t == LVAL_BOOL){
        if (v->bool == True){
            return True;
        }
        return False;
    }
    if (t == LVAL_NIL) {
        return False;
    }
    if (t == LVAL_ERR) {
        return False;
    }
    if (t == LVAL_LIST){
        if (v->list->expr != NULL || v->list->next != NULL){
            return True;
        }
        return False;
    }
    return True;
}

lval* if_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:if: missing operand list");
    } if (l->next == NULL){
        return lval_err("#builtin:if: missing true cond");
    }
    if (l->next->next != NULL){
        if (l->next->next->next != NULL){
            return lval_err("#builtin:if: too many operands");
        }
    }
    lval* r = eval_lval(e, l->expr);
    if (is_true(r)) {
        return eval_lval(e, l->next->expr);
    }
    if (l->next->next == NULL){
        return lval_nil();
    }
    return eval_lval(e, l->next->next->expr);
}

lval* and_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:and: missing operand list");
    }
    lval* n = eval_lval(e, l->expr);
    if (l->next == NULL){
        return n;
    }
    if (is_true(n) == False){
        return n;
    }
    return and_fn(e, l->next);
}

lval* or_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:or: missing operand list");
    }
    lval* n = eval_lval(e, l->expr);
    if (l->next == NULL){
        return n;
    }
    if (is_true(n) == True){
        return n;
    }
    return or_fn(e, l->next);
}

lval* define_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:define: no list passed in to define");
    } if(l->expr->type != LVAL_SYM){
        return lval_err("#builtin:define: define key must be symbol");
    } if(l->next == NULL){
        return lval_err("#builtin:define define value missing");
    }
    char* key = l->expr->sym;
    lval* exists = get_val(e, key);
    if (exists->type != LVAL_UNDEF){
        return lval_err("#builtin:define: value already defined");
    }
    lval_del(exists);
    lval* put_expr = eval_lval(e, l->next->expr);
    if (put_expr->type ==LVAL_ERR){
        return put_expr;
    }
    put_val(e, put_expr, key, ENV_SESSION);
    return lval_nil();
}

lval* lambda_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:lambda: missing operands");
    } if (l->expr == NULL){
        return lval_err("#builtin:lambda: missing var list");
    } if (l->expr->type != LVAL_LIST){
        return lval_err("#builtin:lambda: first arg is var list");
    } if (l->next == NULL){
        return lval_err("#builtin:lambda: missing procedure list");
    } if (l->next->expr == NULL){
        return lval_err("#builtin:lambda: missing procedure list");
    } if (l->next->expr->type != LVAL_LIST){
        return lval_err("#builtin:lambda: second/procedure arg needs to be list");
    } if (l->next->expr->list == NULL){
        return lval_err("#builtin:lambda: arg list can't be empty");
    } if (l->next->next == NULL){
        return lval_lambda(e, l->expr->list, l->next->expr->list);
    }
    return lval_err("#builtin:lambda: too many operands");
}

lval* let_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:let: missing operands");
    }
    env* list_env = init_env(e);
    lval* list_put = put_let(list_env, l->expr->list);
    if (list_put->type == LVAL_ERR){
        return list_put;
    }
    lval_del(list_put);
    lval* eval_resp = eval(list_env, l->next);
    del_env(list_env);
    return eval_resp;
}

lval* put_let(env* e, list* l){
    if (l == NULL ){
        return lval_nil();
    }
    if (l->next == NULL){
        return lval_err("#builtin:let: no value provided for symbol");
    }
    lval* val = eval_lval(e, l->next->expr);
    put_val(e, val, l->expr->sym, ENV_SCOPED);
    if (l->next->next != NULL){
        return put_let(e, l->next->next);
    }
    return lval_nil();
}

lval* quit_fn(env* e, list* l){
    return lval_terminate();
}

lval* atom_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:atom?: no operand provided");
    }
    if (l->next == NULL) {
        if (l->expr->type == LVAL_SYM){
            lval* v = get_val(e, l->expr->sym);
            if (v->type == LVAL_LIST){
                if (v->list != NULL){
                    return lval_bool(False);
                }
            }
            return lval_bool(True);
        }
        if (l->expr->type != LVAL_LIST){
            return lval_bool(True);
        }
        if( l->expr->list == NULL){
            return lval_bool(True);
        }
    }
    if (l->expr == NULL) {
        return lval_err("builtin:atom?: operand is null");
    }
    return lval_bool(False);
}

lval* is_list_fn(env* e, list* l){
    if (l == NULL){
        return lval_err("#builtin:list?: no operand provided");
    }

    if (l->next != NULL){
        return lval_err("#builtin:list?: Too many operands, only takes one");
    }

    if (l->expr->type == LVAL_SYM){
        lval* v = get_val(e, l->expr->sym);
        if (v->type == LVAL_LIST){
            return lval_bool(True);
        }
    }

    if (l->expr->type == LVAL_LIST){
        return lval_bool(True);
    }

    return lval_bool(False);
}


bool int_compat(lval* l){
    if ((l->type == LVAL_NUM_INT) || (l->type == LVAL_NUM_FLOAT)){
        float val = get_num(l);
        int int_val = (int) val;
        if (int_val == val){
            return True;
        }
    }
    return False;
}

lval* print_fn(env* e, list* l){
  if (l == NULL){
    return lval_nil();
  }
  print_lval(e, l->expr);
  printf("\n");
  return print_fn(e, l->next);
}
