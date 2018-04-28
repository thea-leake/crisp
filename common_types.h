typedef struct {
    int type;
    int num_int;
    float num_float;
    char* str;
    char* func;
    char* err;
} lval ;

typedef struct {
    lval* v;
    struct list* next;
} list ;

enum { LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_FUNC, LVAL_ERR, LVAL_LIST };

lval* lval_num_int(int x);
lval* lval_num_float(float x);
lval* lval_str(char* x);
lval* lval_func(char* x);
lval* lval_err(char* x);
void lval_del(lval* v);
void print_lval(lval* v);
