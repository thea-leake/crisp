#ifndef parsing_h
#define parsing_h

#include "common_types.h"
#include "environment.h"

lval* parse_eval(mpc_ast_t* t, env* e);
lval* get_eval_type(mpc_ast_t* t, env* e);
lval* get_atom_type(mpc_ast_t* t);
lval* get_literal(mpc_ast_t* t, env* e);
lval* list_retrieve(mpc_ast_t* t, env* e, bool is_literal);
list* build_list(mpc_ast_t* t, env* e,  int count, int accum_count, bool is_literal);
int get_literal_list_index(mpc_ast_t* t, int index);

#endif
