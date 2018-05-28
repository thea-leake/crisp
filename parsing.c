/*
 * Thea's Lisp
 * Thea Leake
 * https://github.com/thea-leake/build_a_lisp
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpc/mpc.h>

#include "common_types.h"
#include "operators.h"
#include "parsing.h"
#include "eval.h"
#include "environment.h"

list* build_list(mpc_ast_t* t, env* e, int count, int accum_count, bool is_literal){
    lval* tmp = list_retrieve(t->children[accum_count], e, is_literal);
    int next_count = accum_count + 1;
    if (next_count >= count) {
        if (tmp->type != LVAL_NOOP){
            return  prepend_create(tmp, NULL);
        }
        return NULL;
    }
    if (tmp->type != LVAL_NOOP){
        list* next_elem =  build_list(t, e,  count, next_count, is_literal);
        return prepend_create(tmp, next_elem);
    }
    lval_del(tmp);
    return build_list(t, e, count, next_count, is_literal);
}

lval* list_retrieve(mpc_ast_t* t, env* e, bool is_literal){
   if (is_literal == True){
      return get_literal(t, e);
   }
   return get_eval_type(t, e);
}

lval* get_eval_type(mpc_ast_t* t, env* e){
    if (strstr(t->tag, "literal")){
        return get_literal(t, e);
    }
    if (strstr(t->tag, "list")){
        list* l = build_list(t, e, t->children_num, 0, False);
        return eval_func(e, l);
    }
    if (strstr(t->tag, "nil")){
        return lval_nil();
    }
    return get_atom_type(t);
}


lval* get_literal(mpc_ast_t* t, env* e){
    if (strstr(t->tag, "literal"))  {
        int index = get_literal_list_index(t, 0);
        mpc_ast_t* lit = t->children[index];
        list* l = build_list(lit, NULL, lit->children_num, index, True);
        return lval_list(l);
    }
    if (strstr(t->tag, "list")) {
        int index = get_literal_list_index(t, 0);
        list* l = build_list(t, e, t->children_num, index, True);
        return lval_list(l);
    }
    if (strstr(t->tag, "nil")){
        return lval_nil();
    }
    return get_eval_type(t, e);
}

lval* get_atom_type(mpc_ast_t* t){
    if (strstr(t->tag, "builtin")){
        return lval_func(t->contents);
    }
    if (strstr(t->tag, "float")) {
        return lval_num_float(atof(t->contents));
    }
    if (strstr(t->tag, "integer")) {
        return lval_num_int(atoi(t->contents));
    }
    if (strstr(t->tag, "bool")){
       return lval_bool(t->contents);
    }
    if (strstr(t->tag, "symbol")){
       return lval_sym(t->contents);
    }
    if (strstr(t->tag, "string")){
        return lval_str(t->contents);
    }

    return lval_noop();
}

int get_ast_expr_index(mpc_ast_t* t, int index){
    mpc_ast_t* ti = t->children[index];
    if (
          (strstr(ti->tag, "atom")) ||
          (strstr(ti->tag, "list")) ||
          (strstr(ti->tag, "literal"))
    ){
          return index;
    }
    int n = index + 1;
    return get_ast_expr_index(t, n);
}

int get_literal_list_index(mpc_ast_t* t, int index){
    mpc_ast_t* ti = t->children[index];
    if (strstr(ti->tag, "atom") || (strstr(ti->tag, "list"))){
          return index;
    }
    int n = index + 1;
    return get_literal_list_index(t, n);

}

lval* parse_eval(mpc_ast_t* t, env* e){
    if (t->children_num > 0) {
        // t->cildren[0] is regex node, irrelevant
        int index = get_ast_expr_index(t, 0);
        mpc_ast_t* tval = t->children[index];
        return get_eval_type(tval, e);
    }
    return lval_noop();
}
