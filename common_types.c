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

lval* lval_bool(char* x){
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
    v->str = malloc(strlen(x) + 1);
    strcpy(v->sym, x);
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
        case LVAL_FUNC: break;
        case LVAL_BOOL: break;
        case LVAL_NIL: break;
        case LVAL_NOOP: break;
        case LVAL_LIST: list_del(v->list);
        case LVAL_STR: free(v->str); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
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
   print_list_contents(e, l);
   printf(")");
}

void print_list_contents(env* e, list* l){
   if (l != NULL){
      print_lval(e, l->expr);
      if (l->next != NULL){
         printf(" ");
         print_list_contents(e, l->next);
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
        print_lval(e, get_val(e, v->sym));
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
    } if (strcmp("eval", x) == 0){
       return EVAL;
    } if (strcmp("if", x) == 0){
       return IF;
    } if (strcmp("and", x) == 0){
       return AND;
    } if (strcmp("or", x) == 0){
       return OR;
    } if (strcmp("define", x) == 0){
       return DEFINE;
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
      case EVAL: printf("eval"); break;
      case IF: printf("if"); break;
      default: printf("!!func undefined!!");
   }
}
