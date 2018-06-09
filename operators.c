#include "common_types.h"
#include "operators.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


lval* sum_fn(env* e, list* l){
    lval* expr = eval_lval(e, l->expr);
    if (l->next == NULL){
        if (expr->type == LVAL_NUM_INT){
            return lval_num_int(expr->num_int);
        } else if (expr->type == LVAL_NUM_INT){
            return lval_num_float(expr->num_float);
        } else {
        return lval_err("Invalid Type provided");
        }
    }
    lval* accum = sum_fn(e, l->next);
    float expr_val;
    float accum_val;

    if (is_numeric(expr)) {
        expr_val = get_num(expr);
    } else {
        lval_del(accum);
        return lval_err("Invalid Type provided");
    }

    if (is_numeric(accum)){
        accum_val = get_num(accum);
    } else {
        lval_del(accum);
        return lval_err("Invalid Type provided");
    }

    float sum = expr_val + accum_val;
    lval_del(accum);
    //lval_del(accum);

    int sum_int = (int) sum;
    if (sum == sum_int) {
         int s = (int) sum;
         return lval_num_int(s);
    }
    return lval_num_float(sum);
}

lval* sub_fn(env* e, list* l){
    if (l->next == NULL){
        return l->expr;
    }
    lval* expr = eval_lval(e, l->expr);
    lval* accum = sub_fn(e, l->next);
    float expr_val;
    float accum_val;


    if (expr->type == LVAL_NUM_INT){
        expr_val = expr->num_int;
    }
    else  if (expr->type == LVAL_NUM_FLOAT) {
        expr_val = expr->num_float;
    } else {
        return lval_err("Invalid Type provided");
    }
    if (accum->type == LVAL_NUM_INT){
        accum_val = accum->num_int;
    } else if (accum->type == LVAL_NUM_FLOAT){
        accum_val = accum->num_float;
    } else {
        return lval_err("Invalid Type provided");
    }

    float diff = expr_val - accum_val;
    lval_del(accum);


    int diff_int = (int) diff;
    if (diff == diff_int) {
         int s = (int) diff;
         return lval_num_int(s);
    }
    return lval_num_float(diff);
}

lval* mul_fn(env* e, list* l){
    lval* expr = eval_lval(e, l->expr);
    if (l->next == NULL){
        if (expr->type == LVAL_NUM_INT){
            return lval_num_int(expr->num_int);
        } else if (expr->type == LVAL_NUM_INT){
            return lval_num_float(expr->num_float);
        } else {
        return lval_err("Invalid Type provided");
        }
    }
    lval* accum = mul_fn(e, l->next);
    float expr_val;
    float accum_val;


    if (expr->type == LVAL_NUM_INT){
        expr_val = expr->num_int;
    }
    else  if (expr->type == LVAL_NUM_FLOAT) {
        expr_val = expr->num_float;
    } else {
        return lval_err("Invalid Type provided");
    }
    if (accum->type == LVAL_NUM_INT){
        accum_val = accum->num_int;
    } else if (accum->type == LVAL_NUM_FLOAT){
        accum_val = accum->num_float;
    } else {
        return lval_err("Invalid Type provided");
    }
    float product = expr_val * accum_val;
 //   lval_del(accum);

    int product_int = (int) product;
    if (product == product_int) {
         int s = (int) product;
         return lval_num_int(s);
    }
    return lval_num_float(product);
}

lval* div_fn(env* e, list* l){
    lval* v = eval_lval(e, l->expr);
    if (l->next == NULL){
        if (v->type == LVAL_NUM_INT){
            if (v->num_int == 0){
                return lval_err("Unable to divide by 0");
            }
            return lval_num_int(v->num_int );
        } else if (v->type == LVAL_NUM_FLOAT){
            if (v->num_float == 0){
                return lval_err("Unable to divide by 0");
            }
            return lval_num_float(v->num_float );
        } else if (v->type == LVAL_ERR) {
            return lval_err(v->err);
        } else {
            return lval_err("Invalid Type provided");
        }
    }
    lval* accum = div_fn(e, l->next);
    float expr_val;
    float accum_val;


    if (v->type == LVAL_NUM_INT){
        expr_val = v->num_int;
    }
    else  if (v->type == LVAL_NUM_FLOAT) {
        expr_val = v->num_float;
    } else {
        return lval_err("Invalid Type provided");
    }

    if (accum->type == LVAL_NUM_INT){
         if (accum->num_int == 0){
            return lval_err("Unable to divide by 0");
        }
       accum_val = accum->num_int;
    } else if (accum->type == LVAL_NUM_FLOAT){
        if (accum->num_float == 0){
            return lval_err("Unable to divide by 0");
        }
       accum_val = accum->num_float;
    } else if ( accum->type == LVAL_ERR ) {
        return lval_err(accum->err);
    } else {
        return lval_err("Invalid Type provided");
    }

    float ratio = expr_val / accum_val;
//    lval_del(accum);

    int ratio_int = (int) ratio;
    if (ratio == ratio_int) {
         int s = (int) ratio;
         return lval_num_int(s);
    }
    return lval_num_float(ratio);
}

lval* mod_fn(env* e, list* l){
    if (l->next == NULL){
        return lval_err("Only one arg provided");
    }
    lval* expr = eval_lval(e, l->expr);
    lval* accum = mod_fn(e, l->next);
    int expr_val;
    int accum_val;


    if (expr->type == LVAL_NUM_INT){
        expr_val = expr->num_int;
    } else {
        return lval_err("Invalid Type provided");
    }

    if (accum->type == LVAL_NUM_INT){
        accum_val = accum->num_int;
    } else {
        return lval_err("Invalid Type provided");
    }

    if (accum_val == 0){
        return lval_err("Unable to divide by 0");
    }

    int modulo = expr_val % accum_val;

     return lval_num_int(modulo);
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
    return lval_err("No comparison available for type");
}

lval* car_fn(env* e, list* l){
    (void) e;
    return first_expr(l->expr->list);
}

lval* cdr_fn(env* e, list* l){
    (void) e;
    return lval_list(rest_expr(l->expr->list));
}

lval* list_fn(env* e, list* l){
    (void) e;
    return lval_list(l);
}

lval* cons_fn(env* e, list* l){
    if (l->next == NULL){
        return lval_err("No list to const to");
    } if (l->next->next != NULL){
        return lval_err("Too many arguments, expect lval and list");
    } if (l->next->expr->type != LVAL_LIST){
        return lval_err("Second arg must be list");
    }
    lval* x = eval_lval(e, l->expr);
    list* y = l->next->expr->list;
    //might use copy_list, depending on ease of garbage collection
    list* n = list_prepend(y, x);
    return lval_list(n);
}

lval* eval_fn(env* e, list* l){
    list* lc = copy_list(e, l);
    lval* resp = eval(e, lc);
    return resp;
}


bool is_true(lval* v){
    int t = v->type;
    if (t == LVAL_BOOL){
        if (v->bool == True){
            return True;
        }
        return False;
    } if (t == LVAL_NIL) {
        return False;
    } if (t == LVAL_LIST){
        if (v->list->expr != NULL || v->list->next != NULL){
            return True;
        }
        return False;
    }
    return True;
}

lval* if_fn(env* e, list* l){
    if (l->next->next->next != NULL){
        return lval_err("Too many arguments passed in to if.");
    }
    lval* r = eval_lval(e, l->expr);
    if (is_true(r)) {
        return eval_lval(e, l->next->expr);
    }
    return eval_lval(e, l->next->next->expr);
}

lval* and_fn(env* e, list* l){
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
    char* key = l->expr->sym;
    lval* exists = get_val(e, key);
    if (exists->type != LVAL_NIL){
        return lval_err("already defined");
    }
    put_val(e, eval_lval(e, l->next->expr), key, ENV_SESSION);
    return lval_nil();
}

lval* lambda_fn(env* e, list* l){
    return lval_lambda(e, l->expr->list, l->next->expr->list);
}

lval* let_fn(env* e, list* l){
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
        return lval_err("No value provided for symbol");
    }
    put_val(e, l->next->expr, l->expr->sym, ENV_SCOPED);
    return put_let(e, l->next->next);
}

lval* quit_fn(env* e, list* l){
    return lval_terminate();
}
