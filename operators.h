enum {SUM, DIFF, MUL, DIV, MOD, CAR, CDR, LIST, CONS, EVAL, IF, AND, OR, FUNC_UNDEF};

lval* sum_op(list* l);
lval* sub_op(list* l);
lval* mul_op(list* l);
lval* div_op(list* l);
lval* mod_op(list* l);
lval* car_op(list* l);
lval* cdr_op(list* l);
lval* list_op(list* l);
lval* cons_op(list* l);
lval* eval_op(list* l);
lval* if_op(list* l);
lval* and_op(list* l);
lval* or_op(list* l);
bool is_true(lval* v);
