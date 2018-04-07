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
    puts("Lispy version 0.0.0.0.1");
    puts("To exit type ctrl-c");

    // infinite read eval print loop
    while(1) {
        char* input = readline("lispy> ");

        add_history(input);

        printf("No you're a %s\n", input);

        free(input);

    }
    return 0;
}
