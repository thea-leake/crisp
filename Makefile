DEP_DIRS = ./deps
C_SRC_FILES = parsing.c operators.c common_types.c environment.c eval.c prompt.c  deps/mpc/mpc.c
LINK_LIB_FLAGS = -ledit -lm
PRE_BUILD_CMDS = mkdir -p bin
COMMON_BUILD_OPTS =  clang -I $(DEP_DIRS) --std=c99 -Wall $(C_SRC_FILES)  $(LINK_LIB_FLAGS)


deps:
	clib install orangeduck/mpc

cleandeps:
	rm -rf deps

all: clean  build debug debug_tco

build: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -O3 -o bin/crispy

debug: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -o bin/crispy-debug -g


debug_tco: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -O3 -o bin/crispy-debug-tco -g

clean:
	rm -rf bin/

.PHONY: clean deps all
