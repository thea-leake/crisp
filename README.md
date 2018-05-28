My Lisp implementation that uses recursion for all flow control aside from the repl prompt (the loop part of repl :) ).

It originally started around the Build Your Own Lisp book http://www.buildyourownlisp.com/ by Daniel Holden and the Make-A-Lisp project at https://github.com/kanaka/mal/blob/master/process/guide.md. But has since diverged into it's own thing.


As of right now it is pretty much just an infix notation calculator using cons cells..
It also uses recursion for all iteration except for the repl loop (it's a toy, and a Lisp!), and not all are TCO compatible yet so large datasets run the risk of stack call overflows.


# Build:
``` make build```
Builds interpreter and places it in ```bin/lispy```

# Clean:
``` make clean```

# Dependencies:
Dependencies are stored in `deps/` and should be included in the repo by default.
Dependencies are pulled in by [clib](https://github.com/clibs/clib)
Dependencies can be pulled in again via `make deps`, or removed w/ `make cleandeps`.

Current external dependencies are:
 - [mpc](https://github.com/orangeduck/mpc): for parsing code into AST.

# Example usage:
```
$ ./bin/crispy
Lispy version 0.0.0.0.1, Starting args: 1 0x7ffee766a130
To exit type ctrl-c
crispy> (or false 1 true)
1
crispy> (or false nil false)
false
crispy> (and true 1 3 false 3)
false
crispy> (and true 1 3 3)
3
crispy> (if (true) (+ 3 4 ( * 3 4 )) ( - 12 (* 3 2) ))
19
crispy> (cons "grr" '( 3 4 true  ))
("grr" 3 4 true)
crispy> (list 2 4 (+ 4 3))
(2 4 7)
crispy> (car (list 2 4 (+ 4 3)))
2
crispy> (cdr (list 2 4 (+ 4 3)))
(4 7)
crispy> (cons 4 (cdr (list 2 4 (+ 4 3))))
(4 4 7)
crispy>
```
