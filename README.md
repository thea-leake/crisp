A Lisp interpreter written in C built around recursing - hence the name crisp.

It originally started around the [Build Your Own Lisp](http://www.buildyourownlisp.com/) book but has largely been personal exploration as I found I wanted the interpretor to behave internally more like Scheme.  This was created as an excersize to learn C, and to write an interpreter, as I've wanted to do both for a while.  Any recommendations and pointers are welcome, but keep in mind this wasn't written for use in the wild :).

# Notes:
 - It currently uses Clangs TCO build option, however many of the recursive functions are currently not TCO compatible.
 - It still has one loop - for the repl - the l part of 'repl'.
 - Numeric functions automatically downcast return to int when return val can be losslessly converted to int.

# Build:
``` make build```
Builds interpreter and places it in ```bin/crispy```

``` make debug```
Builds interpreter with debug symbols and places at ```bin/crispy-debug```

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
crisp $ ./bin/crispy
Crispy lisp interpreter.  Type Control-c to exit,
crispy> (define a +)
nil
crispy> (define a +)
already defined
crispy> (define b 3)
nil
crispy> (define c 5)
nil
crispy> (let '(c 2) a b c)
5
crispy> (define d (lambda '(a b) '(a b b)))
nil
crispy> (d a b)
6
crispy> (= "grr" "grr")
true
crispy> (define truthy (lambda '(a b) '(= a b)))
nil
crispy> (truthy "grr" 1)
false
crispy> (= "grr" "grr" "grr")
true'))')
crispy> (or false  b 1 true)
3
crispy> (or false nil false)
false
crispy> (and true a 3 false 3)
false
crispy> (if true (+ 3 4 ( * 3 4 a   )) ( - 12 (* 3 b)   ))
Invalid Type provided
crispy> (if true (+ 3 4 ( * 3 4 c   )) ( - 12 (* 3 b)   ))
67
crispy> (cons "grr" '( a b true    ))
("grr" + 3 true)
crispy> (list 2 b a (+ b 3))
(2 3 + 6)
crispy> (car (list 2 b (+ a 3)))
2
crispy> (cdr (list 2 b (+ b 3)))
(3 6)
crispy> (cons a (cdr (list 2 b (+ b 3))))
(+ 3 6)
crispy>')
```
