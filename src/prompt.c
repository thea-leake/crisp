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
};

struct iosrc {
    FILE* input_src;
    FILE* output_src;
};


expr* init_partial_expr(char c){
    expr* val_container = malloc(sizeof(expr));
    partial* val_partial = malloc(sizeof(partial));
    val_container->type = PARTIAL;
    val_container->list = NULL;
    val_container->symbol = NULL;
    val_container->partial = val_partial;
    val_partial->count = 1;
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

cell* add_char(cell* prev_cell, char c,type type){
    if (prev_cell == NULL){
        return init_atom(NULL, c);
    }
    if (prev_cell->val->type != PARTIAL){
        return init_atom(prev_cell, c);

    }
    partial* prev = prev_cell->val->partial;
    partial* nw_chr = malloc(sizeof(partial));
    nw_chr->c = c;
    nw_chr->prev = prev;
    if (prev != NULL){
        nw_chr->count = prev->count + 1;
    } else {
        nw_chr->count = 0;
    }
    nw_chr->prev = prev;
    // Updating head as we've prepended an item to the list
    prev_cell->val->partial = nw_chr;
    return prev_cell;
}

void prev_cell_to_char(char* c, partial* curr){
    if (curr ==  NULL ){
        return;
    }
    c[curr->count] = curr->c;
    partial* prev = curr->prev;
    free(curr);
    prev_cell_to_char(c, prev);
}



char* cell_to_char(partial* string){
    if (string == NULL){
        return NULL;
    }
    char* char_array = malloc((sizeof(char) * string->count + 1 ));
    char_array[string->count] = '\0';
    prev_cell_to_char(char_array, string);
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

cell* handle_whitespace(cell* prev_cell){
    if (prev_cell != NULL && prev_cell->val->type != PARTIAL){
        return prev_cell;
    }
    return roll_up_atom(prev_cell);

}


cell* read_line(iosrc* io, cell* prev_cell, int char_count){
    char input = getc(io->input_src);
    if (input == EOF){
        printf("Goodbye!\n");
        exit(0);
    }
    if (input == '\n'){
        return prev_cell;
    }
    if (input == ' ' || input == '\t' ){
        return read_line(io, handle_whitespace(prev_cell), char_count );
    }
    return read_line(io, add_char(prev_cell, input, SYMBOL), char_count + 1);
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

int main(){
    iosrc* repl = malloc(sizeof(iosrc));
    repl->input_src = stdin;
    repl->output_src = stdout;
    while (true) {
        printf("Getting line:\n");
        cell* input_cell = read_line(repl, NULL, 0);
        print_list(input_cell);
        free(input_cell);
        // now need to add cascading free
    }
    free(repl);
    return 0;
}
