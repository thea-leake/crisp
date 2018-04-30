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

lval* lval_nil(){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NIL;
    return v;
}


list* list_create(lval* v[], int expr_index, int expr_ct){
    list* l = malloc(sizeof(list));
    l->expr = v[expr_index];
    if (expr_ct > 1) {
        int rem_ct = expr_ct - 1;
        int rest_index = expr_index + 1;
        l->next = list_create(v, rest_index, rem_ct);
    } else {
        l->next = NULL;
    }
    return l;
}

list* list_prepend(list* l, lval* v){
    list* n = malloc(sizeof(list));
    n->expr = v;
    n->next = l;
    return n;
}

lval* first_expr(list* l){
   return l->expr;
}

lval* pop(list* l){
    lval* expr = l->expr;
    list* new_dest = l->next;
    free(l);
    l = new_dest;
    return expr;
}

list* rest_expr(list* l){
    return l->next;
}

void lval_del(lval* v){
    switch (v->type){
        case LVAL_NUM_INT: break;
        case LVAL_NUM_FLOAT: break;
        case LVAL_NIL: break;
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_FUNC: free(v->func); break;
    }
    free(v);
}

void list_del(list* l){
    lval_del(l->expr);
    list* n = l->next;
    free(l);
    if (n != NULL) {
        list_del(n);
    }
}

void print_list(list* l ){
   print_lval(l->expr);
   if (l->next != NULL){
      print_list(l->next);
   }
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
    case LVAL_NIL:
        printf("nil\n");
        break;
    }
}
