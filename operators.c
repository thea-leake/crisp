#include "common_types.h"
#include "operators.h"

lval sum_op(lval v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    // return additive operator as it has no operands
    lval init_val = v[1];
    if (init_val.type == LVAL_ERR || expr_ct == 2){
        // return only operative, as there is nothing given to add to it
        return (init_val);
    }

    float accum;
    if (init_val.type == LVAL_NUM_INT){
        accum = init_val.num_int;
    } else {
        accum = init_val.num_float;
    }

    for (int i=iter_start; i < expr_ct; i++){
        lval tmp = v[i];
          if (tmp.type == LVAL_NUM_INT){
            accum += tmp.num_int;
          }
          else  if (tmp.type == LVAL_NUM_FLOAT) {
            accum += tmp.num_float;
          }
    }

    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}

lval sub_op(lval v[], int expr_ct){
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval init_val = v[1];
    if (init_val.type == LVAL_ERR ){
        return (init_val);
    }
    if (expr_ct == 2) {
        if (init_val.type == LVAL_NUM_INT) {
            int negated = init_val.num_int * -1;
            return lval_num_int(negated);
        }
        if (init_val.type == LVAL_NUM_FLOAT) {
            int negated = init_val.num_float * -1;
            return lval_num_float(negated);
        }
    }

    int is_int;
    float accum;
    if (init_val.type == LVAL_NUM_INT){
        is_int = 0;
        accum = init_val.num_int;
    } else {
        is_int = 1;
        accum = init_val.num_float;
    }

    for (int i=iter_start; i < expr_ct; i++){
        lval tmp = v[i];
        if (tmp.type == LVAL_NUM_INT){
            accum = accum - tmp.num_int ;
        }
        else  if (tmp.type == LVAL_NUM_FLOAT) {
            accum = accum - tmp.num_float;
            is_int = 1;
        }
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}


lval mul_op(lval v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval init_val = v[1];
    if (init_val.type == LVAL_ERR || expr_ct == 2){
        return (init_val);
    }

    float accum;
    if (init_val.type == LVAL_NUM_INT){
        accum = init_val.num_int;
    } else {
        accum = init_val.num_float;
    }

    for (int i=iter_start; i < expr_ct; i++){
        lval tmp = v[i];
          if (tmp.type == LVAL_NUM_INT){
            accum = tmp.num_int * accum;
          }
          else  if (tmp.type == LVAL_NUM_FLOAT) {
            accum = tmp.num_float * accum;
          }
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}

lval div_op(lval v[], int expr_ct){
    // printf("Starting sum\n");
    int func_index = 0;
    int init_index = func_index + 1;
    int iter_start = init_index + 1;
    if (expr_ct == 1){
        return v[0];
    }
    lval init_val = v[1];
    if (init_val.type == LVAL_ERR || expr_ct == 2){
        return (init_val);
    }

    float accum;
    if (init_val.type == LVAL_NUM_INT){
        accum = init_val.num_int;
    } else {
        accum = init_val.num_float;
    }

    for (int i=iter_start; i < expr_ct; i++){
        lval tmp = v[i];
        if (tmp.type == LVAL_NUM_INT){
            accum =  accum / tmp.num_int;
        }
        else  if (tmp.type == LVAL_NUM_FLOAT) {
            accum = accum / tmp.num_float;
        }
    }
    int accum_int = (int) accum;
    if (accum == accum_int) {
         int s = (int) accum;
         return lval_num_int(s);
    }
    return lval_num_float(accum);
}
