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

lval* lval_list(list* l){
   lval* v = malloc(sizeof(lval));
   v->type = LVAL_LIST;
   v->list = l;
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


lval* lval_noop(){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NOOP;
    return v;
}


list* prepend_create(lval* v, list* l){
   if (l == NULL){
      return init_list(v);
   }
   list* n = malloc(sizeof(list));
   n->expr = v;
   n->next = l;
   return n;
}

list* init_list(lval* v){
   list* n = malloc(sizeof(list));
   n->expr = v;
   n->next = NULL;
   return n;
}

list* list_from_arrary(lval* v[], int expr_index, int expr_ct){
    list* l = malloc(sizeof(list));
    l->expr = v[expr_index];
    if (expr_ct > 1) {
        int rem_ct = expr_ct - 1;
        int rest_index = expr_index + 1;
        l->next = list_from_arrary(v, rest_index, rem_ct);
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
        case LVAL_NOOP: break;
        case LVAL_LIST: list_del(v->list);
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_FUNC: free(v->func); break;
    }
    free(v);
}

void list_del(list* l){
   if (l != NULL){
      if (l->expr != NULL){
         lval_del(l->expr);
      }
    if (l->next != NULL) {
        list_del(l->next);
    }
    free(l);
   }
}

void print_list(list* l, int start){
   if (start == 1){
      printf("(");
   }
   if (l != NULL){
      printf(" ");
      print_lval(l->expr);
      printf(" ");
      if (l->next != NULL){
         print_list(l->next, 0);
      } else {
         printf(")");
      }
   } else {
      printf( "nil");
   }
}
void print_lval(lval* v){
    switch(v->type) {
    case LVAL_ERR:
        printf("%s", v->err);
        break;
    case LVAL_STR:
        printf("%s", v->str);
        break;
    case LVAL_FUNC:
        printf("%s", v->func);
        break;
    case LVAL_NUM_INT:
        printf("%i", v->num_int);
        break;
    case LVAL_NUM_FLOAT:
        printf("%f", v->num_float);
        break;
    case LVAL_LIST:
        print_list(v->list, 1);
        break;
    case LVAL_NIL:
        printf("nil");
        break;
    case LVAL_NOOP:
        printf("_NOOP\n");
        break;
    }
}