lval* parse_eval(mpc_ast_t* t);
lval* get_eval_type(mpc_ast_t* t);
lval* get_atom_type(mpc_ast_t* t);
lval* get_literal(mpc_ast_t* t);
lval* list_retrieve(mpc_ast_t* t, bool is_literal);
list* build_list(mpc_ast_t* t, int count, int accum_count, bool is_literal);
int get_literal_list_index(mpc_ast_t* t, int index);
