#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common_types.h"
#include "environment.h"

lval* get_val(env* e, char* key){
   if (e == NULL){
      return lval_nil();
   }
   if (e->key == NULL){
      return get_val(e->next, key);
   }
   if (strcmp(key, e->key) == 0) {
      return e->val;
   }
   return get_val(e->next, key);
}

bool put_val(env* e, lval* l, char* key){
   if (e->next == NULL){
      env* nv = new_val(l, key);
      e->next = nv;
      return True;
   }
   return put_val(e, l, key);
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
