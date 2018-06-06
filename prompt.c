#include <stdlib.h>
#include <stdio.h>
#include <mpc/mpc.h>
#include <editline/readline.h>

#include "common_types.h"
#include "operators.h"
#include "parsing.h"
#include "eval.h"


int main(int argc, char** argv) {
    // Version and exit information
    printf("Crispy lisp interpreter.  Type Control-c to exit,\n");

    mpc_parser_t* Bool = mpc_new("bool");
    mpc_parser_t* Integer = mpc_new("integer");
    mpc_parser_t* Float = mpc_new("float");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* String = mpc_new("string");
    mpc_parser_t* Nil = mpc_new("nil");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Atom = mpc_new("atom");
    mpc_parser_t* List = mpc_new("list");
    mpc_parser_t* Element = mpc_new("element");
    mpc_parser_t* Literal = mpc_new("literal");
    mpc_parser_t* Expr = mpc_new("expr");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                                          \
            bool:    \"true\" | \"false\"                                         ;\
            integer:  /-?[0-9]+/                                                  ;\
            float:    /-?[0-9]+\\.[0-9]+/                                         ;\
            number:   <float> | <integer>                                         ;\
            string:   /\"(\\\\.|[^\"])*\"/                                        ;\
            nil:      \"nil\"                                                     ;\
            symbol:   /[a-zA-Z:!_%=\\?\\-\\+\\*\\/]+/                              ;\
            atom:     <string> | <number> | <bool> | <nil> | <symbol>             ;\
            list:     <atom>+ |'(' <atom>+ ')' | <atom>+ <list>+ |                 \
                      '(' <element>+')'                                           ;\
            element:   <atom> | <list> | <literal>                                ;\
            literal:  '''<list>                                                   ;\
            expr:     /^/ <list>| <literal> /$/                                   ;\
        ",
        Bool, Integer, Float, Number, String, Nil, Symbol, Atom, List,
        Element, Literal, Expr
    );
    env* session_env = init_env(NULL);

    // infinite read eval print loop
    while(1) {
        char* input = readline("crispy> ");

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Expr, &r)){
            // print AST
            // mpc_ast_print(r.output);
            lval* result = parse_eval(r.output, session_env);
            print_lval(session_env, result);
            printf("\n");
            if (result->type == LVAL_TERMINATE){
                lval_del(result);
                if (session_env != NULL){
                    del_env(session_env);
                }
                free(input);
                mpc_ast_delete(r.output);
                mpc_cleanup(
                    12,
                    Bool, Integer, Float, Number, String, Nil, Symbol, Atom, List,
                    Element, Literal, Expr
                );
                return 0;
            }
                lval_del(result);
                mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);

    }
    del_env(session_env);

    // clean up our parsers from memory
    mpc_cleanup(
        13,
        Bool, Integer, Float, Number, String, Nil, Symbol, Atom, List,
        Element, Literal, Expr
    );
    return 0;
}
