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

typedef struct {
    int type;
    int num_int;
    float num_float;
    char* str;
    char* sym;
    char* err;
} lval ;

enum { LVAL_NUM_INT, LVAL_NUM_FLOAT, LVAL_STR, LVAL_SYMBOL, LVAL_ERR };

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

lval lval_sym(char* x){
    lval v;
    v.type = LVAL_SYMBOL;
    v.sym = x;
    return v;
}

lval lval_err(char* x){
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

lval eval(mpc_ast_t* t){
    if (strstr(t->tag, "string")){
        return lval_str(t->contents);
    }
    if (strstr(t->tag, "float")) {
        return lval_num_int(atof(t->contents));
    }
    if (strstr(t->tag, "integer")) {
        return lval_num_int(atoi(t->contents));
    }
    return lval_err("undefined type");
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
        "                                                                  \
            integer: /-?[0-9]+/                                          ; \
            float: /-?[0-9]+\\.[0-9]+/                                   ; \
            number: <float> | <integer>                                  ; \
            string: /\"(\\\\.|[^\"])*\"/                                 ; \
            symbols: '+' | '-' | '*' | '/' | '%'                         ; \
            keywords: /add/ | /sub/ | /div/ | /mod/                      ; \
            builtin: <symbols> | <keywords>                              ; \
            expr:     <number> | <string> | '('<builtin> <expr>+ ')'     ; \
            lispy:    /^/<builtin> <expr>+/$/ | /^/<expr>/$/             ; \
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
            lval result = eval(r.output);
            printf("%d \n", result.num_int);
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
