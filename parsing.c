/*
 * Thea's Lisp
 * BSD 3 License
 * Implemented foloowing Build Your Own Lisp - http://www.buildyourownlisp.com/
 * Thea Leake
 * https://github.com/thea-leake/build_a_lisp
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpc/mpc.h>
#include <editline/readline.h>


int main(int argc, char** argv) {
    // Version and exit information
    printf("Lispy version 0.0.0.0.1, Starting args: %d %p\n", argc, argv);
    puts("To exit type ctrl-c");

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                     \
            number:   /-?[0-9]+/ ;                            \
            operator: '+' | '-' | '*' | '\' ;                 \
            expr:     <number> | '(' <operator> <expr>+ ')' ; \
            lispy:    /^/ <operator> <expr>+ /$/ ;            \
        ",
        Number, Operator, Expr, Lispy
    );

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        printf("No you're a %s\n", input);

        free(input);

    }

    // clean up our parsers from memory
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
    return 0;
}
