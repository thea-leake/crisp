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

lval* copy_str(lval* v){
    lval* n = malloc(sizeof(lval));
    n->type = LVAL_STR;
    n->str = malloc(sizeof(v->str));
    strcpy(n->str, v->str);
    return n;
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
    v->func = get_opr(x);
    return v;
}

lval* copy_func(lval* v){
    lval* n = malloc(sizeof(lval));
    n->type = LVAL_FUNC;
    n->func = v->func;
    return n;
}

lval* lval_list(list* l){
   lval* v = malloc(sizeof(lval));
   v->type = LVAL_LIST;
   v->list = l;
   return v;
}

lval* copy_err(lval* v){
    lval* n = malloc(sizeof(lval));
    n->type = LVAL_ERR;
    n->err = malloc(strlen(v->err));
    strcpy(n->err, v->err);
    return n;
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

lval* copy_lval(lval* v){
   switch(v->type){
      case LVAL_NUM_INT:
         return lval_num_int(v->num_int);
      case LVAL_NUM_FLOAT:
         return lval_num_float(v->num_float);
      case LVAL_STR:
         return copy_str(v);
      case LVAL_FUNC:
         return copy_func(v);
      case LVAL_ERR:
         return lval_err(v->err);
      case LVAL_LIST:
         return lval_list(v->list);
      case LVAL_NIL:
         return lval_nil();
      default:
         printf("Error copying lval ");
         print_lval(v); printf("\n");
         return lval_err("Unable to copy lval");
   }
}

list* copy_list(list* l){
   lval* v = copy_lval(l->expr);
   if (l->next == NULL){
      return prepend_create(v, NULL);
   }
   list* n = copy_list(l->next);
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
        case LVAL_FUNC: break;
        case LVAL_NIL: break;
        case LVAL_NOOP: break;
        case LVAL_LIST: list_del(v->list);
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
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

void print_list(list* l){
   printf("(");
   print_list_contents(l);
   printf(")");
}

void print_list_contents(list* l){
   if (l != NULL){
      print_lval(l->expr);
      if (l->next != NULL){
         printf(" ");
         print_list_contents(l->next);
      }
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
        print_opr(v->func);
        break;
    case LVAL_NUM_INT:
        printf("%i", v->num_int);
        break;
    case LVAL_NUM_FLOAT:
        printf("%f", v->num_float);
        break;
    case LVAL_LIST:
        print_list(v->list);
        break;
    case LVAL_NIL:
        printf("nil");
        break;
    case LVAL_NOOP:
        printf("_NOOP\n");
        break;
    }
}

int get_opr(char* x){
    if (strcmp("+", x) == 0 || strcmp("add", x) == 0){
        return SUM;
    } if (strcmp("-", x) == 0 || strcmp("sub", x) == 0){
        return DIFF;
    } if (strcmp("*", x) == 0 || strcmp("mul", x) == 0){
        return MUL;
    } if (strcmp("/", x) == 0 || strcmp("div", x) == 0){
        return DIV;
    } if (strcmp("%", x) == 0 || strcmp("mod", x) == 0){
        return MOD;
    } if (strcmp("car", x) == 0) {
       return CAR;
    } if (strcmp("cdr", x) == 0) {
       return CDR;
    } if (strcmp("list", x) == 0){
       return LIST;
    } if (strcmp("cons", x) == 0){
       return CONS;
    }
    return FUNC_UNDEF;
}

void print_opr(int x){
   switch(x){
      case SUM: printf("+"); break;
      case DIFF: printf("-"); break;
      case MUL: printf("*"); break;
      case DIV: printf("/"); break;
      case MOD: printf("%%"); break;
      case CAR: printf("car"); break;
      case CDR: printf("cdr"); break;
      case LIST: printf("cons"); break;
      default: printf("!!func undefined!!");
   }
}
