#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common_types.h"
#include "operators.h"

lval* lval_num_int(int x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NUM_INT;
    v->num_int = x;
    return v;
}


lval* lval_bool(bool x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_BOOL;
    v->bool = x;
    return v;
}

lval* lval_parse_bool(char* x){
    if (strcmp("true", x) == 0){
       lval* t = malloc(sizeof(lval));
       t->type = LVAL_BOOL;
       t->bool = True;
       return t;
    } if (strcmp("false", x) == 0){
       lval* f = malloc(sizeof(lval));
       f->type = LVAL_BOOL;
       f->bool = False;
       return f;
    }
    return lval_err("Invalid bool val");
}

lval* copy_bool(lval* v){
   lval* n = malloc(sizeof(lval));
   n->type = LVAL_BOOL;
   n->bool = v->bool;
   return n;
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
    v->str = malloc(strlen(x) + 1);
    strcpy(v->str, x);
    return v;
}

lval* lval_sym(char* x){
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(strlen(x) + 1);
    strcpy(v->sym, x);
    return v;
}

lval* lval_lambda(env* e, list* var_expr, list* eval_expr){
   lval* v = malloc(sizeof(lval));
   v->type = LVAL_LAMBDA;
   v->lambda = malloc(sizeof(lambda));
   v->lambda->var_expr = copy_list(e, var_expr);
   v->lambda->eval_expr = copy_list(e, eval_expr);
   return v;
}

lval* copy_lambda(env* e, lval* v){
   lval* n = malloc(sizeof(lval));
   n->type = LVAL_LAMBDA;
   n->lambda = malloc(sizeof(lambda));
   n->lambda->var_expr = copy_list(e, v->lambda->var_expr);
   n->lambda->eval_expr = copy_list(e, v->lambda->eval_expr);
   return n;
}

lval* lval_func(bltn_ptr func_ptr, char* ident){
    lval* v = malloc(sizeof(lval));
    builtin* f = malloc(sizeof(builtin));
    f->func = func_ptr;
    f->ident = ident;
    v->type = LVAL_FUNC;
    v->func = f;
    return v;
}

lval* copy_func(lval* v){
    lval* n = malloc(sizeof(lval));
    builtin* f = malloc(sizeof(builtin));
    f->func = v->func->func;
    f->ident = malloc(strlen(v->func->ident) + 1);
    strcpy(f->ident, v->func->ident);
    n->type = LVAL_FUNC;
    n->func = f;
    return n;
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

lval* copy_lval(env* e, lval* v){
   switch(v->type){
      case LVAL_NUM_INT:
         return lval_num_int(v->num_int);
      case LVAL_BOOL:
         return copy_bool(v);
      case LVAL_NUM_FLOAT:
         return lval_num_float(v->num_float);
      case LVAL_STR:
         return lval_str(v->str);
      case LVAL_SYM:
         return lval_sym(v->sym);
      case LVAL_FUNC:
         return copy_func(v);
      case LVAL_ERR:
         return lval_err(v->err);
      case LVAL_LIST:
         return lval_list(copy_list(e, v->list));
      case LVAL_LAMBDA:
         return copy_lambda(e, v);
      case LVAL_NIL:
         return lval_nil();
      default:
         printf("Error copying lval ");
         print_lval(e, v); printf("\n");
         return lval_err("Unable to copy lval");
   }
}

list* copy_list(env* e, list* l){
   lval* v = copy_lval(e, l->expr);
   if (l->next == NULL){
      return prepend_create(v, NULL);
   }
   list* n = copy_list(e, l->next);
   return prepend_create(v, n);
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

list* list_prepend(list* l, lval* v){
    list* n = malloc(sizeof(list));
    n->expr = v;
    n->next = l;
    return n;
}

lval* first_expr(list* l){
   return l->expr;
}

list* rest_expr(list* l){
    return l->next;
}

void lval_del(lval* v){
    switch (v->type){
        case LVAL_NUM_INT: break;
        case LVAL_NUM_FLOAT: break;
        case LVAL_FUNC: free(v->func->ident); free(v->func); break;
        case LVAL_BOOL: break;
        case LVAL_NIL: break;
        case LVAL_NOOP: break;
        case LVAL_LIST: list_del(v->list);
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
        case LVAL_LAMBDA:
           list_del(v->lambda->var_expr);
           list_del(v->lambda->eval_expr);
           free(v->lambda);
           break;
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

void print_list(env* e, list* l){
   printf("(");
   print_list_contents(e, l, True);
   printf(")");
}

void print_list_symbols(env* e, list* l){
   printf("(");
   print_list_contents(e, l, False);
   printf(")");
}

void print_list_contents(env* e, list* l, bool eval_symbols){
   if (l != NULL){
      print_lval_sym_eval(e, l->expr, eval_symbols);
      if (l->next != NULL){
         printf(" ");
         print_list_contents(e, l->next, eval_symbols);
      }
   }
}

void print_bool(int b){
   if (b == True){
      printf("true");
   } else if (b == False){
      printf("false");
   } else {
      printf("Invalid bool value %d", b);
   }
}

void print_lval(env* e, lval* v){
   print_lval_sym_eval(e, v, True);
}
void print_lval_sym_eval(env* e, lval* v, bool eval){
    switch(v->type) {
    case LVAL_ERR:
        printf("%s", v->err);
        break;
    case LVAL_STR:
        printf("%s", v->str);
        break;
    case LVAL_FUNC:
        printf("%s", v->func->ident);
        break;
    case LVAL_BOOL:
        print_bool(v->bool);
        break;
    case LVAL_NUM_INT:
        printf("%i", v->num_int);
        break;
    case LVAL_NUM_FLOAT:
        printf("%f", v->num_float);
        break;
    case LVAL_LIST:
        print_list(e, v->list);
        break;
    case LVAL_SYM:
        if (eval == True){
           print_lval(e, get_val(e, v->sym));
        } else {
           printf("%s", v->sym);
        }
        break;
    case LVAL_NIL:
        printf("nil");
        break;
    case LVAL_NOOP:
        printf("_NOOP\n");
        break;
    case LVAL_LAMBDA:
        printf("#lambda args: ");
        print_list_symbols(e, v->lambda->var_expr);
        printf(" procedure: ");
        print_list_symbols(e, v->lambda->eval_expr);
        printf("#\n");
    }
}
