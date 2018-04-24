typedef struct {
    int type;
    int num_int;
    float num_float;
    char* str;
    char* func;
    char* err;
} lval ;

enum { LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_FUNC, LVAL_ERR };

lval lval_num_int(int x);
lval lval_num_float(float x);
lval lval_str(char* x);
lval lval_func(char* x);
lval lval_err(char* x);

