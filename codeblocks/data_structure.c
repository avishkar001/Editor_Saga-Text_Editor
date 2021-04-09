#include "data_structure.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


void init_lines_node(lines_node* l){
	l->arr = (char*)malloc(sizeof(char)*NODES_SIZE);
    l->next = NULL;
    l->gap_size = NODES_SIZE;
    l->gap_left = 0;
    l->gap_right = NODES_SIZE - 1;
    return;
}

void load_fullnode(lines_node* l){
    l->gap_size = DEFAULT_GAP_SIZE;
    l->gap_left = NODES_SIZE - DEFAULT_GAP_SIZE;
    l->gap_right = NODES_SIZE - 1;
    return;
}

void print_node(lines_node node){
    //printf("inside print node");
    for(int i=0; i<NODES_SIZE; i++){
        if(i==node.gap_left && node.gap_size != 0){
            i=node.gap_right;
            continue;
        }
        printf("%c", node.arr[i]);
        //if(node.arr[i] == '\n')
            //printf("%c", '\n');
    }
    return;
}


void init_line(line* l){
    l->head = NULL;
    l->line_size = 0;
}

void append_in_line(line *l, lines_node *new){
    if(l->head == NULL){
        l->head = new;
        return;
    }
    lines_node* p = l->head;
    while(p->next != NULL){
        p = p->next;
    }

    p->next = new;
    return;
}

void insert_in_line(line *l, char* arr, int len){
    //printf("inside insert_in_line");
    int index = 0;
    while(len>=0){
        lines_node* new = (lines_node*)malloc(sizeof(lines_node));
        init_lines_node(new);
        strncpy(new->arr, &arr[index], NODES_SIZE - DEFAULT_GAP_SIZE);
        if(len>NODES_SIZE - DEFAULT_GAP_SIZE)
            load_fullnode(new);
        else{
            new->gap_left = len;
            new->gap_right = NODES_SIZE - 1;
            new->gap_size = NODES_SIZE - len;
        }

        append_in_line(l, new);
        len -= NODES_SIZE - DEFAULT_GAP_SIZE;
        index += NODES_SIZE - DEFAULT_GAP_SIZE;
    }

}

void set_line_size(line* l){
    lines_node* p = l->head;

    while (p){
        l->line_size += NODES_SIZE - p->gap_size;
        p = p->next;
    }
    return;
}

void print_line(line l){

    lines_node *p = l.head;
    while(p){
        print_node(*p);
        p = p->next;
    }
    printf("%c", '\n');
    return;
}

void init_buffer(buffer *b, int size, char* filename){
    b->head_index = 0;
    b->head_array = (line*)malloc(sizeof(line)*size);
    b->size = size;
    b->fptr = fopen(filename, "r");
    b->fprev = fopen("fprev.txt", "w+");
    b->fnext = fopen("fnext.txt", "w+");
    b->filename = (char*)malloc(sizeof(char)*(strlen(filename)+1));
    strcpy(b->filename, filename);
    
    if(filename[strlen(filename) - 1] == 'c' && filename[strlen(filename) - 2] == '.')
    	b->filetype = C_FILE;
    else
    	b->filetype = TEXT_FILE;
}


void destroy_line(line* l){
	if(l == NULL)
		return;
    lines_node *p = l->head, *q = NULL;

    while(p){
        q = p;
        p = p->next;
        free(q->arr);
        free(q);
    }
    l->head = NULL;
    l->line_size = 0;
    return;
}


void write_node(FILE *f, lines_node node){
    //printf("inside print node");
    for(int i=0; i<NODES_SIZE; i++){
        if(i==node.gap_left && node.gap_size != 0){
            i=node.gap_right;
            continue;
        }
        fprintf(f, "%c", node.arr[i]);
        //if(node.arr[i] == '\n')
            //fprintf(f, "%c", '\n');
    }
    return;
}

void write_line(FILE* f, line l){

    lines_node *p = l.head;
    while(p){
        write_node(f, *p);
        p = p->next;
    }
    fprintf(f, "%c", '\n');
    return;
}

void write_buffer(FILE* f, buffer b){
    
    for(int i=0; i < b.size; i++){
        write_line(f, b.head_array[(b.head_index + i) % b.size]);
    }
}


void print_buffer(buffer b){
    
    for(int i=0; i < b.size; i++){
        print_line(b.head_array[(b.head_index + i) % b.size]);
    }

}

int get_line_size(line l){

	lines_node* p = l.head;
	
	int count = 0;
	while(p){
		count += NODES_SIZE - p->gap_size;
		p = p->next;
	}
	return count;

}
