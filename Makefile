DEP_DIRS = ./deps
C_SRC_FILES = parsing.c operators.c common_types.c environment.c eval.c prompt.c  deps/mpc/mpc.c
LINK_LIB_FLAGS = -ledit -lm
PRE_BUILD_CMDS = mkdir -p bin
COMMON_BUILD_OPTS =  clang -I $(DEP_DIRS) --std=c99 -Wall $(C_SRC_FILES)  $(LINK_LIB_FLAGS) -O3


deps:
	clib install orangeduck/mpc

cleandeps:
	rm -rf deps

all: clean  build

build: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -o bin/crispy

debug: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -o bin/crispy-debug -g

clean:
	rm -rf bin/

.PHONY: clean deps all
