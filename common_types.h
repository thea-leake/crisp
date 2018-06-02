#ifndef common_types_h
#define common_types_h

typedef int bool;
enum {False, True};

enum { LVAL_BOOL, LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_SYM, LVAL_FUNC, LVAL_ERR, LVAL_LIST, LVAL_LAMBDA, LVAL_NIL, LVAL_NOOP};

typedef struct list list;
typedef struct lval lval;
typedef struct builtin builtin;
typedef struct env env;
typedef struct lambda lambda;
typedef  lval*(*bltn_ptr) (env* e, list* l);


struct builtin {
    bltn_ptr func;
    char* ident;
};

struct lambda {
    list* var_expr;
    list* eval_expr;
};

struct lval {
    int type;
    int num_int;
    float num_float;
    char* str;
    char* sym;
    builtin* func;
    char* err;
    bool bool;
    list* list;
    lambda* lambda;
};

struct list {
    lval* expr;
    list* next;
};

struct env {
    char* key;
    lval* val;
    env* next;
    env* parent;
};


#include  "environment.h"

lval* lval_num_int(int x);
lval* lval_bool(char*  x);
lval* lval_num_float(float x);
lval* lval_str(char* x);
lval* lval_sym(char* x);
lval* lval_func(bltn_ptr func_ptr, char* ident);
lval* lval_err(char* x);
lval* lval_list(list* l);
lval* lval_lambda(env* e, list* vars, list* expr);
lval* lval_nil();
lval* lval_noop();
lval* copy_lval(env* e, lval* v);
lval* copy_func(lval* v);
lval* copy_bool(lval* v);
lval* copy_lambda(env* e, lval* v);
list* prepend_create(lval* v, list* l);
list* init_list(lval* v);
list* list_from_array(lval* v[], int expr_index, int expr_ct);
list* list_prepend(list* l, lval* v);
lval* first_expr(list* l);
list* rest_expr(list* l);
list* copy_list(env* e, list* l);
void lval_del(lval* v);
void list_del(list* l);
void print_lval(env* e, lval* v);
void print_list(env* e, list* l);
void print_list_contents(env* e, list* l);
void print_bool(int b);
void print_opr(int x);

#endif
