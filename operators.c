#include "common_types.h"
#include "operators.h"
#include <stdlib.h>


lval* sum_op(list* l){
    if (l->next == NULL){
        // return additive operator as it has no operands
        lval* v = l->expr;
        list_del(l)
        return v;
    }
    lval* expr = l->expr;
    lval* accum = sum_op(rest_expr(l));
    float expr_val;
    float accum_val;


    if (expr->type == LVAL_NUM_INT){
        expr_val = expr->num_int;
    }
    else  if (expr->type == LVAL_NUM_FLOAT) {
        expr_val = expr->num_float;
    }
    if (accum->type == LVAL_NUM_INT){
        accum_val = accum->num_int;
    } else if (accum->type == LVAL_NUM_FLOAT){
        accum_val = accum->num_float;
    }

    list_del(l);
    float sum = expr_val + accum_val;
    int sum_int = (int) sum;
    if (sum == sum_int) {
         int s = (int) sum;
         return lval_num_int(s);
    }
    return lval_num_float(sum);
}

lval* sub_op(lval* v[], int expr_ct){
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval_del(v[func_index]);

    lval* init_val = v[init_index];
    if (init_val->type == LVAL_ERR ){
        return init_val;
    }
    if (expr_ct == iter_start) {
        if (init_val->type == LVAL_NUM_INT) {
            int negated = init_val->num_int * -1;
            return lval_num_int(negated);
        }
        if (init_val->type == LVAL_NUM_FLOAT) {
            int negated = init_val->num_float * -1;
            return lval_num_float(negated);
        }
    }

    float accum;
    if (init_val->type == LVAL_NUM_INT){
        accum = init_val->num_int;
    } else {
        accum = init_val->num_float;
    }
    lval_del(init_val);


    for (int i=iter_start; i < expr_ct; i++){
        lval* tmp = v[i];
        if (tmp->type == LVAL_NUM_INT){
            accum = accum - tmp->num_int ;
        }
        else  if (tmp->type == LVAL_NUM_FLOAT) {
            accum = accum - tmp->num_float;
        }
        lval_del(tmp);
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}


lval* mul_op(lval* v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval_del(v[func_index]);

    lval* init_val = v[init_index];
    if (init_val->type == LVAL_ERR || expr_ct == iter_start){
        return init_val;
    }

    float accum;
    if (init_val->type == LVAL_NUM_INT){
        accum = init_val->num_int;
    } else {
        accum = init_val->num_float;
    }
    lval_del(init_val);

    for (int i=iter_start; i < expr_ct; i++){
        lval* tmp = v[i];
          if (tmp->type == LVAL_NUM_INT){
            accum = tmp->num_int * accum;
          }
          else  if (tmp->type == LVAL_NUM_FLOAT) {
            accum = tmp->num_float * accum;
          }
        lval_del(tmp);
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}

lval* div_op(lval* v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval_del(v[func_index]);

    lval* init_val = v[init_index];
    if (init_val->type == LVAL_ERR || expr_ct == 2){
        return init_val;
    }

    float accum;
    if (init_val->type == LVAL_NUM_INT){
        accum = init_val->num_int;
    } else {
        accum = init_val->num_float;
    }
    lval_del(init_val);

    for (int i=iter_start; i < expr_ct; i++){
        lval* tmp = v[i];
          if (tmp->type == LVAL_NUM_INT){
              if (tmp->num_int == 0){
                  return lval_err("Unable to divide by 0");
              }
              accum = accum / tmp->num_int;
          }
          else  if (tmp->type == LVAL_NUM_FLOAT) {
              if (tmp->num_float == 0){
                  return lval_err("Unable to divide by 0");
              }
              accum = accum / tmp->num_float;
          }
        lval_del(tmp);
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}

lval* mod_op(lval* v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval_del(v[func_index]);

    lval* init_val = v[init_index];
    if (init_val->type == LVAL_ERR || expr_ct == iter_start){
        return init_val;
    }
    lval_del(init_val);

    int accum = init_val->num_int;

    for (int i=iter_start; i < expr_ct; i++){
        lval* tmp = v[i];
        if (tmp->type != LVAL_NUM_INT) {
            return lval_err("Non-int passed into modulo");
        }
        accum =  accum % tmp->num_int;
        lval_del(tmp);
    }
    return lval_num_int(accum);
}
