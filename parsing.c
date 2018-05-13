/*
 * Thea's Lisp
 * Thea Leake
 * https://github.com/thea-leake/build_a_lisp
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpc/mpc.h>
#include <editline/readline.h>

#include "common_types.h"
#include "operators.h"


lval* eval(mpc_ast_t* t);
lval* eval_func(list * l);
lval* get_eval_type(mpc_ast_t* t);
lval* get_atom_type(mpc_ast_t* t);
lval* get_literal(mpc_ast_t* t);
lval* list_retrieve(mpc_ast_t* t, int is_literal);
list* build_list(mpc_ast_t* t, int count, int accum_count, int is_literal);
int get_literal_list_index(mpc_ast_t* t, int index);


list* build_list(mpc_ast_t* t, int count, int accum_count, int is_literal){
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

lval* list_retrieve(mpc_ast_t* t, int is_literal){
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
    if (strstr(ti->tag, "atom") || (strstr(ti->tag, "list")) || (strstr(ti->tag, "literal"))){
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

lval* eval(mpc_ast_t* t){
    if (t->children_num > 0) {
        // t->cildren[0] is regex node, irrelevant
        int index = get_ast_expr_index(t, 0);
        mpc_ast_t* tval = t->children[index];
        lval* expr = get_eval_type(tval);
        return expr;
    }
    return lval_noop();
}

lval* eval_func(list * l){
    lval* func = l->expr;
    list* operands = l->next;
    switch(func->func){
      case SUM:  return sum_op(operands);
      case DIFF:  return sub_op(operands);
      case MUL:  return mul_op(operands);
      case DIV:  return div_op(operands);
      case MOD: return mod_op(operands);
      case CAR:return car_op(operands);
      case CDR: return cdr_op(operands);
      case LIST: return list_op(operands);
      case CONS: return cons_op(operands);
      default:   return lval_err("func undefined\n");
   }
}



int main(int argc, char** argv) {
    // Version and exit information
    printf("Lispy version 0.0.0.0.1, Starting args: %d %p\n", argc, argv);
    puts("To exit type ctrl-c");

    mpc_parser_t* Bool = mpc_new("bool");
    mpc_parser_t* Integer = mpc_new("integer");
    mpc_parser_t* Float = mpc_new("float");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* String = mpc_new("string");
    mpc_parser_t* Nil = mpc_new("nil");
    mpc_parser_t* Symbols = mpc_new("symbols");
    mpc_parser_t* Keywords = mpc_new("keywords");
    mpc_parser_t* Builtin = mpc_new("builtin");
    mpc_parser_t* Atom = mpc_new("atom");
    mpc_parser_t* List = mpc_new("list");
    mpc_parser_t* Element = mpc_new("element");
    mpc_parser_t* Literal = mpc_new("literal");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                                            \
            bool:  \"true\" | \"false\"                                             ;\
            integer:  /-?[0-9]+/                                                    ;\
            float:    /-?[0-9]+\\.[0-9]+/                                           ;\
            number:   <float> | <integer>                                           ;\
            string:   /\"(\\\\.|[^\"])*\"/                                          ;\
            nil:      \"nil\"                                                       ;\
            symbols:  '+' | '-' | '*' | '/' | '%'                                   ;\
            keywords: \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\" | \"car\" |    \
            \"cdr\" |  \"list\" | \"eval\" | \"list\" | \"cons\"                    ;\
            builtin:  <symbols> | <keywords>                                        ;\
            atom:     <builtin> | <string> | <number> | <bool> | <nil>                       ;\
            list:     <atom>+ |'(' <atom>+ ')' | <atom>+ <list>+ | '(' <element>+')';\
            element:   <atom> | <list> | <literal>                                  ;\
            literal:  '''<list>                                                     ;\
            lispy:    /^/ <list>| <literal> /$/                                     ;\
        ",
        Bool, Integer, Float, Number, String, Nil, Symbols, Keywords, Builtin, Atom, List,
        Element, Literal, Lispy
    );

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)){
            // print AST
            // mpc_ast_print(r.output);
            lval* result = eval(r.output);
            print_lval(result);
            printf("\n");
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);

    }

    // clean up our parsers from memory
    mpc_cleanup(
        9,

        Bool, Integer, Float, Number, String, Nil, Symbols, Keywords, Builtin, Atom, List,
        Element, Literal, Lispy
    );
    return 0;
}
