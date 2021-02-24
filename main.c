#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 40
#define NODES_SIZE 50

typedef struct lines_node{
    char* arr;
    struct lines_node *next;
}lines_node;

void init_lines_node(lines_node* l){
    l->arr = (char*)malloc(sizeof(char)*NODES_SIZE);
    l->next = NULL;
}

void print_node(lines_node node){

    for(int i=0; i<NODES_SIZE; i++){
        printf("%c", node.arr[i]);
        if(node.arr[i] == '\n')
            return;
    }
    return;
}

typedef lines_node* line;

void init_line(line* l){
    *l = NULL;
}

void append_in_line(line *l, lines_node *new){
    if(*l == NULL){
        *l = new;
        return;
    }
    lines_node* p = *l;
    while(p->next != NULL){
        p = p->next;
    }
    
    p->next = new;
    return;
}

void insert_in_line(line *l, char* arr, int len){
    //printf("inside insert_in_line");
    int index = 0;
    while(len>0){
        lines_node* new = (lines_node*)malloc(sizeof(lines_node));
        init_lines_node(new);
        strncpy(new->arr, &arr[index], NODES_SIZE);
        append_in_line(l, new);
        len -= NODES_SIZE;
        index += NODES_SIZE;
    }
    
    
}

void print_line(line l){

    lines_node *p = l;
    while(p){
        print_node(*p);
        p = p->next;
    }
    return;
}

typedef struct buffer{
    line head_array[BUFFER_SIZE];
    int head_index;
}buffer;

void init_buffer(buffer b){
    b.head_index = 0;
}

void read_file_firsttime(FILE* fptr, buffer* b){

    int len;

    for(int i=0; i<BUFFER_SIZE; i++){
        len = 50;
        char* data = (char*)malloc(sizeof(char)*len);
        len = getline(&data, &len, fptr);
        line* newline = (line*)malloc(sizeof(line));
        init_line(newline);
        insert_in_line(newline, data, len);
        b->head_array[(b->head_index + i) % BUFFER_SIZE] = *newline;
    }
}

void print_buffer(buffer b){
    
    for(int i=0; i < BUFFER_SIZE; i++){
        print_line(b.head_array[(b.head_index + i) % BUFFER_SIZE]);
    }

}

int main(){

    FILE* fptr = fopen("file.txt", "r");
    buffer b;
    init_buffer(b);
    read_file_firsttime(fptr, &b);
    print_buffer(b);

    return 0;
}


