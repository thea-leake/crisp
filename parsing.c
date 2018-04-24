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


lval eval(mpc_ast_t* t);
lval eval_func(lval v[], int expr_ct);
void print_lval_str(lval *v);
void print_lval_err(lval* v);
void print_lval_fun(lval* v);
void print_lval_int(lval* v);
void print_lval_float(lval* v);
void print_lval(lval* v);


lval lval_num_int(int x){
    lval v;
    v.type = LVAL_NUM_INT;
    v.num_int = x;
    return v;
}

lval lval_num_float(float x){
    lval v;
    v.type = LVAL_NUM_FLOAT;
    v.num_float = x;
    return v;
}

lval lval_str(char* x){
    lval v;
    v.type = LVAL_STR;
    v.str = x;
    return v;
}

lval lval_func(char* x){
    lval v;
    v.type = LVAL_FUNC;
    v.func = x;
    return v;
}

lval lval_err(char* x){
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

void print_str(char* s) {
    char* escaped = malloc(strlen(s)+1);
    strcpy(escaped, s);
    escaped = mpcf_escape(escaped);
    printf("\"%s\"\n", escaped);
    free(escaped);
}

void print_lval_str(lval *v) {
    print_str(v->str);
}

void print_lval_err(lval* v) {
    print_str(v->err);
}

void print_lval_func(lval* v){
    print_str(v->func);
}

void print_lval_int(lval* v) {
    printf("%i\n", v->num_int);
}

void print_lval_float(lval* v) {
    printf("%f\n", v->num_float);
}

void print_lval(lval* v){
    switch(v->type) {
    case LVAL_ERR:
        print_lval_err(v);
        break;
    case LVAL_STR:
        print_lval_str(v);
        break;
    case LVAL_FUNC:
        print_lval_func(v);
        break;
    case LVAL_NUM_INT:
        print_lval_int(v);
        break;
    case LVAL_NUM_FLOAT:
        print_lval_float(v);
        break;
    }
}


lval eval(mpc_ast_t* t){
    // printf("Num children is %d\n", t->children_num);
    if (strstr(t->tag, "string")){
        lval v = lval_str(t->contents);
        return v;
    }
    if (strstr(t->tag, "float")) {
        lval v = lval_num_float(atof(t->contents));
        return v;
    }
    if (strstr(t->tag, "integer")) {
        lval v = lval_num_int(atoi(t->contents));
        return v;
    }
    if (strstr(t->tag, "builtin")){
        lval v = lval_func(t->contents);
        return v;
    }
    if ( t->children_num > 0){
        lval accum[t->children_num];
        int accum_count = 0;
        for (int i = 0; i < t->children_num; i++){
            // printf("Iterating through node %d\n", i);
            lval tmp = eval(t->children[i]);
            if (tmp.type != LVAL_ERR) {
                // printf("adding to expr accum\n");
                accum[accum_count] = tmp;
                accum_count++;
            }
        }
        if (accum_count <= 1){
            return accum[0];
        }
        return eval_func(accum, accum_count);
    }
    return lval_err("undefined type");
}

lval eval_func(lval v[], int expr_ct){
    lval func = v[0];
    if (strcmp("+", func.func) == 0 || strcmp("add", func.func) == 0){
        return sum_op(v, expr_ct);
    } else if (strcmp("-", func.func) == 0 || strcmp("sub", func.func) == 0){
        return sub_op(v, expr_ct);
    } else if (strcmp("*", func.func) == 0 || strcmp("mul", func.func) == 0){
        return mul_op(v, expr_ct);
    } else if (strcmp("/", func.func) == 0 || strcmp("div", func.func) == 0){
        return div_op(v, expr_ct);
    } else if (strcmp("%", func.func) == 0 || strcmp("mod", func.func) == 0){
        return mod_op(v, expr_ct);
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
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                                    \
            integer:  /-?[0-9]+/                                           ; \
            float:    /-?[0-9]+\\.[0-9]+/                                  ; \
            number:   <float> | <integer>                                  ; \
            string:   /\"(\\\\.|[^\"])*\"/                                 ; \
            symbols:  '+' | '-' | '*' | '/' | '%'                          ; \
            keywords: /add/ | /sub/ | /mul/ | /div/ | /mod/                ; \
            builtin:  <symbols> | <keywords>                               ; \
            expr:     <number> | <string> | '('<builtin> <expr>+ ')'       ; \
            lispy:    /^/ <builtin> <expr>+/$/ | /^/<expr> | <builtin> /$/ ; \
        ",
        Integer, Float, Number, String,  Symbols, Keywords, Builtin, Expr, Lispy
    );

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)){
            // print AST
            // mpc_ast_print(r.output);
            lval result = eval(r.output);
            print_lval(&result);
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
        Integer, Float, Number, String,  Symbols, Keywords, Builtin, Expr, Lispy
    );
    return 0;
}
