#ifndef environment_h
#define environment_h

#include "common_types.h"

typedef struct env env;

struct env {
    char* key;
    lval* val;
    env* next;
};

lval* get_val(env* e, char* key);
bool put_val(env* e , lval* l, char* key);
env* new_val(lval* l, char* key);
env* init_env();
void del_env(env* e);

#endif
