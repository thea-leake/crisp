My Lisp implementation loosely built around the build your own lisp book http://www.buildyourownlisp.com/ by Daniel Holden.


As of right now it is pretty much just an infix notation calculator using cons cells..


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
build_a_lisp $ ./bin/lispy
Lispy version 0.0.0.0.1, Starting args: 1 0x7ffeec001fa8
To exit type ctrl-c
lispy> 1
1
lispy> "rar"
"\"rar\""
lispy> *
"*"
lispy> / 5 (- 4 2) 1 ( * (* 2 2) 2)
0.312500
lispy> (% 5 2)
1
lispy> / 10 2
5
```
