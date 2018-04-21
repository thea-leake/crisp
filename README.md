Thea's Lisp implementation loosely built around  the build your own lisp book http://www.buildyourownlisp.com/ by Daniel Holden.


#Build:
``` make build```
Places interpreter in ```bin/lispy```

#Clean:
``` make clean```


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
