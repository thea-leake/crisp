C_SRC_FILES = src/prompt.c
LINK_LIB_FLAGS = -ledit -lm
PRE_BUILD_CMDS = mkdir -p bin
COMMON_BUILD_OPTS =  clang --std=c99 -Wall $(C_SRC_FILES)  $(LINK_LIB_FLAGS)


all: clean  build debug debug_tco

build: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -O3 -o bin/crispy -g

debug: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -o bin/crispy-debug -g


debug_tco: $(C_SRC_FILES)
	$(PRE_BUILD_CMDS)
	$(COMMON_BUILD_OPTS) -O3 -o bin/crispy-debug-tco -g

clean:
	rm -rf bin/

.PHONY: clean deps all
