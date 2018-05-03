/*
 * Thea's Lisp
 * Implemented foloowing Build Your Own Lisp - http://www.buildyourownlisp.com/
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


list* build_list(mpc_ast_t* t, int count, int accum_count){
    lval* tmp = eval(t->children[accum_count]);
    int next_count = accum_count + 1;
    if (next_count == count) {
        printf("Hit anchor\n");
        if (tmp->type != LVAL_NOOP){
            list* anchor = prepend_create(tmp, NULL);
            return anchor;
        }
        printf("Returning NULL\n");
        return NULL;
    }
    if (tmp->type != LVAL_NOOP){
        list* next_elem =  build_list(t, count, next_count);
        return prepend_create(tmp, next_elem);
    }
    lval_del(tmp);
    return build_list(t, count, next_count);
}

lval* eval(mpc_ast_t* t){
    // printf("Num children is %d\n", t->children_num);
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
    if ( t->children_num > 0){
        list* expr_list = build_list(t, t->children_num, 0);
        lval* eval_result = eval_func(expr_list);
        list_del(expr_list);
        return eval_result;
    }
    return lval_noop();
}

lval* eval_func(list * l){
    lval* func = l->expr;
    list* operands = rest_expr(l);
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
    mpc_parser_t* Symbols = mpc_new("symbols");
    mpc_parser_t* Keywords = mpc_new("keywords");
    mpc_parser_t* Builtin = mpc_new("builtin");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* List = mpc_new("list");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                                             \
            integer:  /-?[0-9]+/                                                    ; \
            float:    /-?[0-9]+\\.[0-9]+/                                           ; \
            number:   <float> | <integer>                                           ; \
            string:   /\"(\\\\.|[^\"])*\"/                                          ; \
            symbols:  '+' | '-' | '*' | '/' | '%'                                   ; \
            keywords: /add/ | /sub/ | /mul/ | /div/ | /mod/                         ; \
            builtin:  <symbols> | <keywords>                                        ; \
            expr:     <number> | <string> | <builtin> | '('<expr>+ ')'              ; \
            list:     '''<expr>                                                       ; \
            lispy:    /^/ <builtin> <expr>+/$/ | /^/<expr> | <builtin> | <list> /$/ ; \
        ",
        Integer, Float, Number, String,  Symbols, Keywords, Builtin, Expr, List, Lispy
    );

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)){
            // print AST
            mpc_ast_print(r.output);
            lval* result = eval(r.output);
            print_lval(result);
            lval_del(result);
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
        Integer, Float, Number, String,  Symbols, Keywords, Builtin, Expr, List, Lispy
    );
    return 0;
}
