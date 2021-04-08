#ifndef DATA_STRUCTURE_H_INCLUDED
#define DATA_STRUCTURE_H_INCLUDED


#define NODES_SIZE 10
#define DEFAULT_GAP_SIZE 1
#define WINDOWS 0

#define C_FILE 0
#define TEXT_FILE 1

#include<stdio.h>

typedef struct lines_node{
    char *arr;
    int gap_left,gap_right, gap_size;
    struct lines_node *next;
}lines_node;

typedef struct line{
    lines_node* head;
    int line_size;
} line;

typedef struct buffer{
    line *head_array;
    int head_index;
    int size;
    FILE *fptr, *fprev, *fnext;
    int filetype;
}buffer;


void init_lines_node(lines_node* l);
void load_fullnode(lines_node* l);
void print_node(lines_node node);
void init_line(line* l);
void append_in_line(line *l, lines_node *new);
void insert_in_line(line *l, char* arr, int len);
void set_line_size(line* l);
void print_line(line l);
void init_buffer(buffer *b, int size, char* filename);
void destroy_line(line* l);
void write_node(FILE *f, lines_node node);
void write_line(FILE* f, line l);
void write_buffer(FILE* f, buffer b);
void print_buffer(buffer b);
int get_line_size(line l);
#endif // DATA_STRUCTURE_H_INCLUDED
