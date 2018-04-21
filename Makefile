DEP_DIRS = ./deps
C_SRC_FILES =parsing.c deps/mpc/mpc.c
LINK_LIB_FLAGS = -ledit -lm

deps:
	clib install orangeduck/mpc

cleandeps:
	rm -rf deps

all: clean  build

build: parsing.c
	mkdir -p bin
	gcc -I $(DEP_DIRS) --std=c99 -Wall $(C_SRC_FILES)  $(LINK_LIB_FLAGS) -o bin/lispy

clean:
	rm -rf bin/

.PHONY: clean deps all
