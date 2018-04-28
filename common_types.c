#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common_types.h"

lval* lval_num_int(int x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NUM_INT;
    v->num_int = x;
    return v;
}

lval* lval_num_float(float x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NUM_FLOAT;
    v->num_float = x;
    return v;
}

lval* lval_str(char* x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_STR;
    v->str = malloc(sizeof(x) + 1);
    strcpy(v->str, x);
    return v;
}

lval* lval_func(char* x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_FUNC;
    v->func = malloc(strlen(x) + 1);
    strcpy(v->func, x);
    return v;
}

lval* lval_err(char* x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(strlen(x) + 1);
    strcpy(v->err, x);
    return v;
}

void lval_del(lval* v){
    print_lval(v);
    switch (v->type){
        case LVAL_NUM_INT: break;
        case LVAL_NUM_FLOAT: break;
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_FUNC: free(v->func); break;
    }
    free(v);
}

void print_lval(lval* v){
    switch(v->type) {
    case LVAL_ERR:
        printf("%s\n", v->err);
        break;
    case LVAL_STR:
        printf("%s\n", v->str);
        break;
    case LVAL_FUNC:
        printf("%s\n", v->func);
        break;
    case LVAL_NUM_INT:
        printf("%i\n", v->num_int);
        break;
    case LVAL_NUM_FLOAT:
        printf("%f\n", v->num_float);
        break;
    }
}
