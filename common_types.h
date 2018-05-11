typedef struct list list;

typedef struct {
    int type;
    int num_int;
    float num_float;
    char* str;
    int func;
    char* err;
    list* list;
    list* literal;
} lval ;

struct list {
    lval* expr;
    list* next;
} ;


enum { LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_FUNC, LVAL_ERR, LVAL_LIST, LVAL_LITERAL, LVAL_NIL, LVAL_NOOP};
enum {False, True};
enum {SUM, DIFF, MUL, DIV, MOD, CAR, CDR, FUNC_UNDEF};

lval* lval_num_int(int x);
lval* lval_num_float(float x);
lval* lval_str(char* x);
lval* lval_func(char* x);
lval* lval_err(char* x);
lval* lval_list(list* l);
lval* lval_nil();
lval* lval_noop();
list* prepend_create(lval* v, list* l);
list* init_list(lval* v);
list* list_from_array(lval* v[], int expr_index, int expr_ct);
list* list_preprend(list* l, lval* v);
lval* first_expr(list* l);
list* rest_expr(list* l);
int get_opr(char* x);
void lval_del(lval* v);
void list_del(list* l);
void print_lval(lval* v);
void print_list(list* l, int start);
void print_opr(int x);
