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

list* build_list(mpc_ast_t* t, int count, int accum_count, bool is_literal){
    lval* tmp = list_retrieve(t->children[accum_count], is_literal);
    int next_count = accum_count + 1;
    if (next_count >= count) {
        if (tmp->type != LVAL_NOOP){
            list* anchor = prepend_create(tmp, NULL);
            return anchor;
        }
        return NULL;
    }
    if (tmp->type != LVAL_NOOP){
        list* next_elem =  build_list(t, count, next_count, is_literal);
        list* tmp_lst =  prepend_create(tmp, next_elem);
        return tmp_lst;
    }
    lval_del(tmp);
    list* tmp_lst = build_list(t, count, next_count, is_literal);
    return tmp_lst;
}

lval* list_retrieve(mpc_ast_t* t, bool is_literal){
   if (is_literal == True){
      return get_literal(t);
   }
   return get_eval_type(t);
}

lval* get_eval_type(mpc_ast_t* t){
    if (strstr(t->tag, "literal")){
        return get_literal(t);
    }
    if (strstr(t->tag, "list")){
        list* l = build_list(t, t->children_num, 0, False);
        // segfault happening after here
        lval* v = eval_func(l);
        return v;
    }
    if (strstr(t->tag, "nil")){
        lval* v = lval_nil();
        return v;
    }
   lval* v = get_atom_type(t);
    return v;
}


lval* get_literal(mpc_ast_t* t){
    if (strstr(t->tag, "literal"))  {
        int index = get_literal_list_index(t, 0);
        mpc_ast_t* lit = t->children[index];
        list* l = build_list(lit, lit->children_num, index, True);
        lval* v = lval_list(l);
        return v;
    }
    if (strstr(t->tag, "list")) {
        int index = get_literal_list_index(t, 0);
        list* l = build_list(t, t->children_num, index, True);
        lval* v = lval_list(l);
        return v;
    }
    if (strstr(t->tag, "nil")){
        lval* v = lval_nil();
        return v;
    }
   lval* v = get_eval_type(t);
    return v;
}

lval* get_atom_type(mpc_ast_t* t){
    if (strstr(t->tag, "builtin")){
        lval* v = lval_func(t->contents);
        return v;
    }
    if (strstr(t->tag, "string")){
        lval* v = lval_str(t->contents);
        return v;
    }
    if (strstr(t->tag, "float")) {
        lval* v = lval_num_float(atof(t->contents));
        return v;
    }
    if (strstr(t->tag, "integer")) {
        lval* v = lval_num_int(atoi(t->contents));
        return v;
    }
    if (strstr(t->tag, "bool")){
       lval* v = lval_bool(atoi(t->contents));
       return v;
    }
   lval* v = lval_noop();
    return v;
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

lval* parse_eval(mpc_ast_t* t){
    if (t->children_num > 0) {
        // t->cildren[0] is regex node, irrelevant
        int index = get_ast_expr_index(t, 0);
        mpc_ast_t* tval = t->children[index];
        lval* expr = get_eval_type(tval);
        return expr;
    }
    return lval_noop();
}
