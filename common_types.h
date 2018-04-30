typedef struct {
    int type;
    int num_int;
    float num_float;
    char* str;
    char* func;
    char* err;
} lval ;

typedef struct list list;

struct list {
    lval* expr;
    list* next;
} ;


enum { LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_FUNC, LVAL_ERR, LVAL_LIST, LVAL_NIL };

lval* lval_num_int(int x);
lval* lval_num_float(float x);
lval* lval_str(char* x);
lval* lval_func(char* x);
lval* lval_err(char* x);
lval* lval_nil();
list* list_create(lval* v[], int expr_index, int expr_ct);
list* list_preprend(list* l, lval* v);
lval* first_expr(list* l);
list* rest_expr(list* l);
void lval_del(lval* v);
void list_del(list* l);
void print_lval(lval* v);
void print_list(list* l);
