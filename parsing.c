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


list* build_list(mpc_ast_t* t, int count, int accum_count){
    lval* tmp = get_eval_type(t->children[accum_count]);
    int next_count = accum_count + 1;
    if (next_count == count) {
        if (tmp->type != LVAL_NOOP){
            list* anchor = prepend_create(tmp, NULL);
            return anchor;
        }
        return NULL;
    }
    if (tmp->type != LVAL_NOOP){
        list* next_elem =  build_list(t, count, next_count);
        return prepend_create(tmp, next_elem);
    }
    lval_del(tmp);
    return build_list(t, count, next_count);
}

lval* get_eval_type(mpc_ast_t* t){
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
    if (strstr(t->tag, "builtin")){
        lval* v = lval_func(t->contents);
        return v;
    }
    if (strstr(t->tag, "list")){
        list* l = build_list(t, t->children_num, 0);
        lval* v = eval_func(l);
        return v;
    }
    if (strstr(t->tag, "literal")){
        list* l = build_list(t, t->children_num, 0);
        lval* v = lval_list(l);
        return v;
    }
    if (strstr(t->tag, "nil")){
        lval* v = lval_nil();
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
    if (strcmp("+", func->func) == 0 || strcmp("add", func->func) == 0){
        return sum_op(operands);
    } else if (strcmp("-", func->func) == 0 || strcmp("sub", func->func) == 0){
        return sub_op(operands);
    } else if (strcmp("*", func->func) == 0 || strcmp("mul", func->func) == 0){
        return mul_op(operands);
    } else if (strcmp("/", func->func) == 0 || strcmp("div", func->func) == 0){
        return div_op(operands);
    } else if (strcmp("%", func->func) == 0 || strcmp("mod", func->func) == 0){
        return mod_op(operands);
    } else {
        return lval_err("func undefined\n");
    }
}



int main(int argc, char** argv) {
    // Version and exit information
    printf("Lispy version 0.0.0.0.1, Starting args: %d %p\n", argc, argv);
    puts("To exit type ctrl-c");

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
            integer:  /-?[0-9]+/                                                    ;\
            float:    /-?[0-9]+\\.[0-9]+/                                           ;\
            number:   <float> | <integer>                                           ;\
            string:   /\"(\\\\.|[^\"])*\"/                                          ;\
            nil:      /nil/                                                         ;\
            symbols:  '+' | '-' | '*' | '/' | '%'                                   ;\
            keywords: /add/ | /sub/ | /mul/ | /div/ | /mod/                         ;\
            builtin:  <symbols> | <keywords>                                        ;\
            atom:     <builtin> | <string> | <number> | <nil>                       ;\
            list:     <atom>+ |'(' <atom>+ ')' | <atom>+ <list>+ | '(' <element>+')';\
            element:   <atom> | <list> | <literal>                                  ;\
            literal:  '''<list>                                                     ;\
            lispy:    /^/ <list>| <literal> /$/                                     ;\
        ",
        Integer, Float, Number, String, Nil, Symbols, Keywords, Builtin, Atom, List,
        Element, Literal, Lispy
    );

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)){
            // print AST
            //mpc_ast_print(r.output);
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
        Integer, Float, Number, String, Nil, Symbols, Keywords, Builtin, Atom, List,
        Element, Literal, Lispy
    );
    return 0;
}
