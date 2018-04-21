Thea's Lisp implementation loosely built around  the build your own lisp book http://www.buildyourownlisp.com/ by Daniel Holden.


#Build:
``` make build```
Places interpreter in ```bin/lispy```

#Clean:
``` make clean```


Example usage:
```
(default) Thea Addison Leake@Theas-MacBook:build_a_lisp $ ./bin/lispy
Lispy version 0.0.0.0.1, Starting args: 1 0x7ffee55affa8
To exit type ctrl-c
lispy> +
"+"
lispy> -
"-"
lispy> %
"%"
lispy> /
"/"
lispy> *
"*"
lispy> add
"add"
lispy> sub
"sub"
lispy> undefexpr
<stdin>:1:1: error: expected '+', '-', '*', '/', '%', 'a', 's', 'd', 'm', one or more of one of '0123456789', '"' or '(' at 'u'
lispy> "itsastr"
"\"itsastr\""
lispy> 1
1
lispy> + 1 2
3
lispy> + 1
1
lispy> (+ 1 ( + 1 3))
5
lispy> + 1 (+ 1 ( + 1 3.5 (+ 1 2 3)))
12.500000
lispy>
```
