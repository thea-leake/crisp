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
         return get_builtin_lval(key);
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

lval* get_builtin_lval(char* key){
   if (key == NULL){
      return lval_err("get_builtin_lval: Null key ref given");
   }
   bltn_ptr ptr = get_builtin(key);
   if (ptr == NULL){
      return lval_undef();
   }
   return lval_func(key);
}

bltn_ptr get_builtin(char* key){
   if (strcmp("+", key) == 0 || strcmp("add", key) == 0){
       return &sum_fn;
   } if (strcmp("-", key) == 0 || strcmp("sub", key) == 0){
       return &sub_fn;
   } if (strcmp("*", key) == 0 || strcmp("mul", key) == 0){
       return &mul_fn;
   } if (strcmp("/", key) == 0 || strcmp("div", key) == 0){
       return &div_fn;
   } if (strcmp("%", key) == 0 || strcmp("mod", key) == 0){
       return &mod_fn;
   }if (strcmp("=", key) == 0){
      return &eq_fn;
   } if (strcmp("car", key) == 0) {
      return &car_fn;
   } if (strcmp("cdr", key) == 0) {
      return &cdr_fn;
   } if (strcmp("list", key) == 0){
      return &list_fn;
   } if (strcmp("cons", key) == 0){
      return &cons_fn;
   } if (strcmp("eval", key) == 0){
      return &eval_fn;
   } if (strcmp("if", key) == 0){
      return &if_fn;
   } if (strcmp("and", key) == 0){
      return &and_fn;
   } if (strcmp("or", key) == 0){
      return &or_fn;
   } if (strcmp("define", key) == 0){
      return &define_fn;
   } if (strcmp("lambda", key) == 0){
      return &lambda_fn;
   } if (strcmp("let", key) == 0){
      return &let_fn;
   } if (strcmp("quit", key) == 0){
      return &quit_fn;
   } if (strcmp("atom?", key) == 0){
      return &atom_fn;
   } if (strcmp("list?", key) == 0){
      return &is_list_fn;
   }
   return NULL;
}
