A Lisp interpreter written in C built around recursing - hence crisp.

It originally started around the [Build Your Own Lisp](http://www.buildyourownlisp.com/) and the [Make-A-Lisp](https://github.com/kanaka/mal/blob/master/process/guide.md) project, but has largely been personal exploration.  Most of the decisions around what have been done have been centered around exploration/expiramentation as a fun way of learning C.

# Notes on usage:
 - It currently uses GCCs TCO build option, but is many of the recursive functions are currently not TCO compatible.
 - Numeric functions automatically downcast return to int when return val is int.

# Build:
``` make build```
Builds interpreter and places it in ```bin/crispy```

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
(default) Thea Addison Leake@Theas-MacBook:crisp $ ./bin/crispy
Crisp version 0.0.0.0.1, Starting args: 1 0x7ffee61b5128
To exit type ctrl-c
crispy> (define a 1)
nil
crispy> (define b 4)
nil
crispy> (+ a b)
5
crispy> (or false  b 1 true)
4
crispy> (or false nil false)
false
crispy> (and true a 3 false 3)
false
crispy> (if true (+ 3 4 ( * 3 4 a  )) ( - 12 (* 3 b)  ))
19
crispy> (cons "grr" '( a b true   ))
("grr" 1 4 true)
crispy> (list 2 b a (+ b 3))
(2 4 1 7)
crispy> (car (list 2 b (+ a 3)))
2
crispy> (cdr (list 2 b (+ b 3)))
(4 7)
crispy> (cons a (cdr (list 2 b (+ b 3))))
(1 4 7)
crispy>')
```
