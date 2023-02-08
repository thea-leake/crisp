A Lisp interpreter written in C built around recursing - hence the name crisp.

It originally started using the [Build Your Own Lisp](http://www.buildyourownlisp.com/) book as a reference but switched early on to personal exploration as I found I wanted the interpretor to behave internally more like Scheme.  This was created as an exercise to learn C, and to write an interpreter, as I've wanted to do both for a while.  Any recommendations and pointers are welcome, but keep in mind this wasn't written for use in the wild :).

# Notes:
 - It currently uses Clangs TCO build option, however many of the recursive functions are currently not TCO compatible.
 - It still has one loop - for the repl - the l part of 'repl'.
 - Numeric functions automatically downcast return to int when return val can be losslessly converted to int.
 - Repo for documentation is [here](https://github.com/thea-leake/crispy-docs).
 - Documentation is hosted [here](https://thea-leake.github.io/crisp/).

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
./bin/crispy
Crispy lisp interpreter.  Type (quit) to exit.
crispy> (define nil? (lambda '(x) '(if (= nil x) true false)))
nil
crispy> (nil? 5)
false
crispy> (nil? nil)
true
crispy> (define true? (lambda '(x) '(if x true false)))
nil
crispy> (true? 1)
true
crispy> (true? nil)
false
crispy> (true? true)
true
crispy> (define false? (lambda '(x) '(if x false true)))
nil
crispy> (false? 1)
false
crispy> (false? nil)
true
crispy> (false? false)
true
crispy> (define range (lambda '(r) '(let '(b (lambda '(c d) '(if (= c 0) d (b (- c 1 ) (cons c d )) )))  b r '()  )))
nil
crispy> (range 25)
(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24)
crispy> ((lambda '() '( + 1 4 )))
5
crispy> (or false false nil )
nil
crispy> (or false  "grr" 1 true)
"grr"
crispy> (define b "rarr")
nil
crispy> (= "grr" b)
false
crispy> (define c 1)
nil
crispy> (and true c 3 false 4)
false
crispy> (and true c 3 )
3
crispy> ^D
Ending session.
Goodbye.
```
