#include "common_types.h"
#include "operators.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


lval* sum_op(list* l){
    if (l->next == NULL){
        lval* expr = l->expr;
        if (expr->type == LVAL_NUM_INT){
            return lval_num_int(expr->num_int);
        } else if (expr->type == LVAL_NUM_INT){
            return lval_num_float(expr->num_float);
        } else {
        return lval_err("Invalid Type provided");
        }
    }
    lval* expr = l->expr;
    lval* accum = sum_op(l->next);
    float expr_val;
    float accum_val;


    if (expr->type == LVAL_NUM_INT){
        expr_val = expr->num_int;
    }
    else  if (expr->type == LVAL_NUM_FLOAT) {
        expr_val = expr->num_float;
    } else {
        lval_del(accum);
        return lval_err("Invalid Type provided");
    }
    if (accum->type == LVAL_NUM_INT){
        accum_val = accum->num_int;
    } else if (accum->type == LVAL_NUM_FLOAT){
        accum_val = accum->num_float;
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

lval* sub_op(list* l){
    if (l->next == NULL){
        return l->expr;
    }
    lval* expr = l->expr;
    lval* accum = sub_op(l->next);
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

lval* mul_op(list* l){
    if (l->next == NULL){
        lval* expr = l->expr;
        if (expr->type == LVAL_NUM_INT){
            return lval_num_int(expr->num_int);
        } else if (expr->type == LVAL_NUM_INT){
            return lval_num_float(expr->num_float);
        } else {
        return lval_err("Invalid Type provided");
        }
    }
    lval* expr = l->expr;
    lval* accum = mul_op(l->next);
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

lval* div_op(list* l){
    if (l->next == NULL){
        lval* v = l->expr;
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
    lval* expr = l->expr;
    lval* accum = div_op(l->next);
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

lval* mod_op(list* l){
    if (l->next == NULL){
        return lval_err("Only one arg provided");
    }
    lval* expr = l->expr;
    lval* accum = mod_op(l->next);
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

lval* car_op(list* l){
    return first_expr(l->expr->list);
}

lval* cdr_op(list* l){
    return lval_list(rest_expr(l->expr->list));
}

lval* list_op(list* l){
    return lval_list(l);
}

lval* cons_op(list* l){
    if (l->next == NULL){
        return lval_err("No list to const to");
    } if (l->next->next != NULL){
        return lval_err("Too many arguments, expect lval and list");
    } if (l->next->expr->type != LVAL_LIST){
        return lval_err("Second arg must be list");
    }
    lval* x = l->expr;
    list* y = l->next->expr->list;
    //might use copy_list, depending on ease of garbage collection
    list* n = list_prepend(y, x);
    return lval_list(n);
}

lval* eval_op(env* e, list* l){
    list* r = eval_list(e, l, True);
    if (r->next != NULL){
        // lists should be contained in an lval list
        return lval_err("Eval returned too many values");
    }
    return r->expr;
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

lval* if_op(env* e, list* l){
    if (l->next->next->next != NULL){
        return lval_err("Too many arguments passed in to if.");
    }
    lval* r = eval_lval(e, l->expr);
    if (is_true(r)) {
        return eval_lval(e, l->next->expr);
    }
    return eval_lval(e, l->next->next->expr);
}

lval* and_op(env* e, list* l){
    lval* n = eval_lval(e, l->expr);
    if (l->next == NULL){
        return n;
    }
    if (is_true(n) == False){
        return n;
    }
    return and_op(e, l->next);
}

lval* or_op(env* e, list* l){
    lval* n = eval_lval(e, l->expr);
    if (l->next == NULL){
        return n;
    }
    if (is_true(n) == True){
        return n;
    }
    return or_op(e, l->next);
}
