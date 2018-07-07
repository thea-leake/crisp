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
```lisp
(default) Thea Addison Leake@Theas-MacBook:crisp $ ./bin/crispy
Crispy lisp interpreter.  Type (quit) to exit.
crispy> (define b 3)
nil
crispy> (define c 5)
nil
crispy> (define c 4)
ERROR:#builtin:define: value already defined
crispy> (if true (+ 3 4 ( * 3 4 c)) (- 12 (* 3 b)))
67
crispy> (and true c 3 false 4)
false
crispy> (or false nil false)
false
crispy> (or false  b 1 true)
3
crispy> (= "grr" "grr")
true
crispy> (= "grr" b)
false
crispy> (define eq (lambda '(a b) '(= a b)))
nil
crispy> (eq "grr" 1)
false
crispy> (define a (lambda '(b c d e) '(if (= c d) ( * d e ) ( a b (* c 2 ) d e))))
nil
crispy> (a + 2 8 4)
32
crispy> (let '(a (lambda '(a b) '(+ a b))) a 1 2 )
3
crispy>  (cons "grr" '( a b true ))
("grr" SYMBOL:a SYMBOL:b true)
crispy> (list 2 b a (+ b 3))
(2 SYMBOL:b SYMBOL:a 6)
crispy> (car '( 2 b (+ c 3))
<stdin>:1:21: error: expected '"', '-', one or more of one of '0123456789', "true", "false", "nil", one or more of one of 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:!_%=?-+*/', '(', ''' or ')' at end of input
crispy> (car '( 2 b (+ c 3)))
2
crispy> (cdr '( 2 b (+ b 3)))
(SYMBOL:b (SYMBOL:+ SYMBOL:b 3))
crispy> (cons c (cdr (list 2 b (+ b 3))))
(5 SYMBOL:b 6)
crispy> ((lambda '(b a) '(a b b)) 2 + )
4
crispy> ((lambda '() '( + 1 4 )))
5
crispy> (quit)
Ending session.
Goodbye.

 ```
