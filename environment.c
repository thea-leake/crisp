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
      if (e->parent == NULL){
         return get_builtin(key);
      }
      return get_val(e->parent, key);
   }
   if (e->key != NULL){
      if (strcmp(e->key, key) == 0) {
         return e->val;
      }
   }
   return get_val(e->next, key);
}

bool put_val(env* e, lval* l, char* key, int env_type){
   if (e->next == NULL){
      env* nv = new_val(e->parent, l, key);
      e->next = nv;
      return True;
   }
   return put_val(e->next, l, key,  env_type);
}

env* init_env(env* parent){
   env* e = malloc(sizeof(env));
   e->next = NULL;
   e->key = NULL;
   e->val = NULL;
   e->parent = parent;
   return e;
}

env* new_val(env* p, lval* l, char* key){
   env* n = malloc(sizeof(env));
   char* nk = malloc(strlen(key) + 1);
   strcpy(nk, key);
   n->key = nk;
   n->val = copy_lval(n, l);
   n->next = NULL;
   n->parent = p;
   return n;
}

void del_env(env* e){
   if (e->key != NULL){
      free(e->key);
   } if (e->val != NULL){
      lval_del(e->val);
   }
   env* n = e->next;
   free(e);
   if (n != NULL){
      del_env(n);
   }
}

lval* get_builtin(char* key){
   if (strcmp("+", key) == 0 || strcmp("add", key) == 0){
       return lval_func(&sum_fn, key);
   } if (strcmp("-", key) == 0 || strcmp("sub", key) == 0){
       return lval_func(&sub_fn, key);
   } if (strcmp("*", key) == 0 || strcmp("mul", key) == 0){
       return lval_func(&mul_fn, key);
   } if (strcmp("/", key) == 0 || strcmp("div", key) == 0){
       return lval_func(&div_fn, key);
   } if (strcmp("%", key) == 0 || strcmp("mod", key) == 0){
       return lval_func(&mod_fn, key);
   } if (strcmp("car", key) == 0) {
      return lval_func(&car_fn, key);
   } if (strcmp("cdr", key) == 0) {
      return lval_func(&cdr_fn, key);
   } if (strcmp("list", key) == 0){
      return lval_func(&list_fn, key);
   } if (strcmp("cons", key) == 0){
      return lval_func(&cons_fn, key);
   } if (strcmp("eval", key) == 0){
      return lval_func(&eval_fn, key);
   } if (strcmp("if", key) == 0){
      return lval_func(&if_fn, key);
   } if (strcmp("and", key) == 0){
      return lval_func(&and_fn, key);
   } if (strcmp("or", key) == 0){
      return lval_func(&or_fn, key);
   } if (strcmp("define", key) == 0){
      return lval_func(&define_fn, key);
   } if (strcmp("lambda", key) == 0){
      return lval_func(&lambda_fn, key);
   } if (strcmp("let", key) == 0){
      return lval_func(&let_fn, key);
   }
   return lval_nil();
}
