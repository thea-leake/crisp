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
    printf("Crispy lisp interpreter.  Type (quit) to exit.\n");

    mpc_parser_t* Integer = mpc_new("integer");
    mpc_parser_t* Float = mpc_new("float");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* String = mpc_new("string");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Atom = mpc_new("atom");
    mpc_parser_t* List = mpc_new("list");
    mpc_parser_t* Element = mpc_new("element");
    mpc_parser_t* Literal = mpc_new("literal");
    mpc_parser_t* Expr = mpc_new("expr");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                                          \
            integer:  /-?[0-9]+/                                                  ;\
            float:    /-?[0-9]+\\.[0-9]+/                                         ;\
            number:   <float> | <integer>                                         ;\
            string:   /\"(\\\\.|[^\"])*\"/                                        ;\
            symbol:   /[a-zA-Z:!_%=\\?\\-\\+\\*\\/]+/                             ;\
            atom:     <string> | <number> | <symbol>                              ;\
            list:     <atom>+ |'(' <atom>+ ')' | <atom>+ <list>+ |                 \
                      '(' <element>+')' | '('')'                                  ;\
            element:   <atom> | <list> | <literal>                                ;\
            literal:  '''<list>                                                   ;\
            expr:     /^/ <list>| <literal> /$/                                   ;\
        ",
        Integer, Float, Number, String, Symbol, Atom, List,
        Element, Literal, Expr
    );
    env* session_env = init_env(NULL);

    // infinite read eval print loop
    while(1) {
        char* input = readline("crispy> ");

        if (input == NULL){
            free(input);
            printf("\n");
            break;
        }

        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Expr, &r)){
            // print AST
            // mpc_ast_print(r.output);
            lval* result = parse_eval(r.output, session_env);
            if (result->type == LVAL_TERMINATE){
                lval_del(result);
                free(input);
                mpc_ast_delete(r.output);
                break;
            }
            print_lval_sym_eval(session_env, result, False);
            printf("\n");
            lval_del(result);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);

    }
    printf("Ending session.\nGoodbye.\n");
    del_env(session_env);

    // clean up our parsers from memory
    mpc_cleanup(
        10,
        Integer, Float, Number, String, Symbol, Atom, List,
        Element, Literal, Expr
    );
    return 0;
}
