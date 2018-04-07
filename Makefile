deps:
	clib install orangeduck/mpc

all: clean deps build

build: prompt.c
	mkdir -p bin
	gcc -I deps -std=c99 -Wall prompt.c -ledit -o bin/prompt

clean:
	rm -rf deps/
	rm -rf bin/

.PHONY: clean deps all
