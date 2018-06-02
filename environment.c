#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common_types.h"
#include "environment.h"
#include "operators.h"

lval* get_val(env* e, char* key){
   if (e->next == NULL){
      if (e->key != NULL){
         if (strcmp(e->key, key) == 0) {
            return e->val;
         }
      }
      return get_builtin(key);
   }
   if (e->key != NULL){
      if (strcmp(e->key, key) == 0) {
         return e->val;
      }
   }
   return get_val(e->next, key);
}

bool put_val(env* e, lval* l, char* key){
   if (e->next == NULL){
      env* nv = new_val(l, key);
      e->next = nv;
      return True;
   }
   return put_val(e->next, l, key);
}

env* init_env(){
   env* e = malloc(sizeof(env));
   e->next = NULL;
   e->key = NULL;
   e->val = NULL;
   return e;
}

env* new_val(lval* l, char* key){
   env* n = malloc(sizeof(env));
   char* nk = malloc(strlen(key) + 1);
   strcpy(nk, key);
   n->key = nk;
   n->val = copy_lval(n, l);
   n->next = NULL;
   return n;
}

void del_env(env* e){
   free(e->key);
   lval_del(e->val);
   env* n = e->next;
   free(e);
   if (n != NULL){
      del_env(n);
   }
}

lval* get_builtin(char* key){
   if (strcmp("+", key) == 0 || strcmp("add", key) == 0){
       return lval_func(SUM, &sum_op, key);
   } if (strcmp("-", key) == 0 || strcmp("sub", key) == 0){
       return lval_func(DIFF, &sub_op, key);
   } if (strcmp("*", key) == 0 || strcmp("mul", key) == 0){
       return lval_func(MUL, &mul_op, key);
   } if (strcmp("/", key) == 0 || strcmp("div", key) == 0){
       return lval_func(DIV, &div_op, key);
   } if (strcmp("%", key) == 0 || strcmp("mod", key) == 0){
       return lval_func(MOD, &mod_op, key);
   } if (strcmp("car", key) == 0) {
      return lval_func(CAR, &car_op, key);
   } if (strcmp("cdr", key) == 0) {
      return lval_func(CDR, &cdr_op, key);
   } if (strcmp("list", key) == 0){
      return lval_func(LIST, &list_op, key);
   } if (strcmp("cons", key) == 0){
      return lval_func(CONS, &cons_op, key);
   } if (strcmp("eval", key) == 0){
      return lval_func(EVAL, &eval_op, key);
   } if (strcmp("if", key) == 0){
      return lval_func(IF, &if_op, key);
   } if (strcmp("and", key) == 0){
      return lval_func(AND, &and_op, key);
   } if (strcmp("or", key) == 0){
      return lval_func(OR, &or_op, key);
   } if (strcmp("define", key) == 0){
      return lval_func(DEFINE, &define_op, key);
   }
   return lval_nil();
}
