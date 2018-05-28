A Lisp interpreter written in C built around recursing - hence crisp.

It originally started around the [Build Your Own Lisp](http://www.buildyourownlisp.com/) and the [Make-A-Lisp](https://github.com/kanaka/mal/blob/master/process/guide.md) project, but has largely been personal exploration.  Most of the decisions around what have been done have been centered around exploration/expiramentation as a fun way of learning C.

# Notes on usage:
 - It currently uses GCCs TCO build option, but is many of the recursive functions are currently not TCO compatible.
 - Numeric functions automatically downcast return to int when return val is int.

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
