deps:
	clib install orangeduck/mpc

cleandeps:
	rm -rf deps

all: clean  build

build: prompt.c
	mkdir -p bin
	gcc -I ./deps --std=c99 -Wall prompt.c -ledit -o bin/prompt

clean:
	rm -rf bin/

.PHONY: clean deps all
