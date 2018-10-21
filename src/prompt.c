#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <readline/readline.h>


typedef struct cell cell;
typedef struct iosrc iosrc;
typedef struct expr expr;
typedef struct partial partial;

typedef int bool;
typedef int type;

enum {false, true};
enum {LIST, PARTIAL, SYMBOL};



struct cell {
    cell* next;
    // prev and parent allow for continuations on partial inputs from the repl
    // while still parsing into data structure on single pass.
    cell* prev;
    cell* parent;
    expr* val;
};

struct expr {
    type type;
    char* symbol;
    cell* list;
    partial* partial;
};

struct partial {
    int count;
    partial* prev;
    char c;
    int escaped;
};

struct iosrc {
    FILE* input_src;
    FILE* output_src;
};


void delete_partial(partial* p);
void delete_expr(expr* e);


expr* init_partial_expr(char c){
    expr* val_container = malloc(sizeof(expr));
    partial* val_partial = malloc(sizeof(partial));
    val_container->type = PARTIAL;
    val_container->list = NULL;
    val_container->symbol = NULL;
    val_container->partial = val_partial;
    val_partial->count = 0;
    val_partial->escaped = false;
    val_partial->prev = NULL;
    val_partial->c = c;
    return val_container;
}

cell* init_atom(cell* prev, char c){
    cell* new_cell = malloc(sizeof(cell));
    new_cell->prev = prev;
    if (prev != NULL){
        prev->next = new_cell;
        new_cell->parent = prev->parent;
    } else{
        new_cell->parent = NULL;
    }
    new_cell->next = NULL;
    new_cell->val = init_partial_expr(c);
    return new_cell;
}

partial* append_partial(partial* prev, char c){
    partial* nw_chr = malloc(sizeof(partial));
    if (c == '\\'){
        nw_chr->c = '!';
        nw_chr->escaped = true;
    } else {
        nw_chr->c = c;
        nw_chr->escaped = false;
    }
    nw_chr->prev = prev;
    if (prev != NULL){
        nw_chr->count = prev->count + 1;
    } else {
        nw_chr->count = 0;
    }
    nw_chr->prev = prev;
    return nw_chr;
}

cell* add_char(cell* prev_cell, char c){
    if (prev_cell == NULL){
        return init_atom(NULL, c);
    }
    if (prev_cell->val->type != PARTIAL){
        return init_atom(prev_cell, c);

    }
    partial* prev = prev_cell->val->partial;
    if (prev->escaped == true){
        prev->c = c;
        prev->escaped = false;
        return prev_cell;
    }
    prev_cell->val->partial = append_partial(prev, c);
    return prev_cell;
}

void char_lst_to_char(char* c, partial* curr){
    if (curr ==  NULL ){
        return;
    }
    c[curr->count] = curr->c;
    partial* prev = curr->prev;
    free(curr);
    char_lst_to_char(c, prev);
}



char* cell_to_char(partial* string){
    if (string == NULL){
        return NULL;
    }
    char* char_array = malloc((sizeof(char) * string->count + 1 ));
    char_array[string->count + 1] = '\0';
    char_lst_to_char(char_array, string);
    return char_array;
}



cell* roll_up_atom(cell* item){
    partial* string = item->val->partial;
    char* symbol = cell_to_char(string);
    expr* atom = item->val;
    atom->type = SYMBOL;
    atom->list = NULL;
    atom->partial = NULL;
    atom->symbol = symbol;
    return item;
}

cell* handle_whitespace(cell* prev_cell, char c){
    if (prev_cell != NULL && prev_cell->val->type != PARTIAL){
        return prev_cell;
    }
    if (prev_cell->val->partial->escaped == true){
        return add_char(prev_cell, c);
    }
    return roll_up_atom(prev_cell);
}


cell* read_line(iosrc* io, cell* prev_cell){
    char input = getc(io->input_src);
    if (input == EOF){
        free(io);
        exit(0);
    }
    if (input == '\n'){
        return handle_whitespace(prev_cell, input);
    }
    if (input == ' ' || input == '\t' ){
        return read_line(io, handle_whitespace(prev_cell, input));
    }
    return read_line(io, add_char(prev_cell, input));
}


void print_list(cell* list){
    if (list == NULL){
        printf("End of list\n");
        return;
    }
    if (list->val == NULL){
        printf("List val null\n");
        // placing in return for TCO, not returning as void
        return print_list(list->prev);
    }
    if (list->val->type == SYMBOL){
        printf("List symbol val: %s\n", list->val->symbol);
        return print_list(list->prev);
    }
        printf("Not symbol\n");
        return print_list(list->prev);
}

void delete_partial(partial* p){
    if (p == NULL){
        return;
    }
    partial* prev = p->prev;
    free(p);
    delete_partial(prev);
}

void delete_expr(expr* e){
    if (e->type == PARTIAL){
        delete_partial(e->partial);
    } else if (e->type == SYMBOL){
        free(e->symbol);
    } // add fw delete list
    free(e);
}

void delete_cell_bw(cell* c){
    if (c == NULL){
        return;
    }
    cell* prev = c->prev;
    cell* parent = c->parent;
    delete_expr(c->val);
    free(c);
    if (prev == NULL){
        if (parent == NULL){
            return;
        }
        return delete_cell_bw(parent);
    }
    delete_cell_bw(prev);
}

int main(){
    iosrc* repl = malloc(sizeof(iosrc));
    repl->input_src = stdin;
    repl->output_src = stdout;
    while (true) {
        printf("Getting line:\n");
        cell* input_cell = read_line(repl, NULL);
        print_list(input_cell);
        delete_cell_bw(input_cell);
        // now need to add cascading free
    }
    free(repl);
    return 0;
}
