My Lisp implementation that started around the Build Your Own Lisp book http://www.buildyourownlisp.com/ by Daniel Holden and the Make-A-Lisp project at https://github.com/kanaka/mal/blob/master/process/guide.md.


As of right now it is pretty much just an infix notation calculator using cons cells..
It also uses recursion for all iteration except for the repl loop (it's a toy, and a Lisp!), and not all are TCO compatible yet (nor is the gcc flag to enable it set in the build task), so large datasets run the risk of stack call overflows.


#Build:
``` make build```
Builds interpreter and places it in ```bin/lispy```

#Clean:
``` make clean```

#Dependencies:
Dependencies are stored in `deps/` and are pulled in by [clib](https://github.com/clibs/clib)
Dependencies can be pulled in again via `make deps`, or removed w/ `make cleandeps`, though they should already be available.

Example usage:
```
(default) Thea Addison Leake@Theas-MacBook:build_a_lisp $ ./bin/lispy
Lispy version 0.0.0.0.1, Starting args: 1 0x7ffeea717148
To exit type ctrl-c
lispy> +
+
lispy> "rarrr"
"rarrr"
lispy> + 3 5
8
lispy> (+ 1 23 4 ( - 5 2 ) ( * 4 2))
43
lispy> + 1 23 4 ( / 5 2 ) ( % 5 4)
28
lispy>
```
