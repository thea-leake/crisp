#ifndef environment_h
#define environment_h

#include "common_types.h"


lval* get_val(env* e, char* key);
bool put_val(env* e , lval* l, char* key, int env_type);
env* new_val(env* p, lval* l, char* key);
env* init_env(env* e);
void del_env(env* e);
lval* get_builtin_lval(char* key);
bltn_ptr get_builtin(char* key);

#endif
