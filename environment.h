typedef struct env env;

struct env {
    char* key;
    lval* val;
    env* next;
};

lval* get_val(env* e, char* key);
bool put_val(env* e , lval* l, char* key);
env* new_val(lval* l, char* key);
void del_env(env* e);
