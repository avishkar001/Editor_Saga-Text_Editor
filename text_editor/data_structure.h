#ifndef DATA_STRUCTURE_H_INCLUDED
#define DATA_STRUCTURE_H_INCLUDED

typedef struct lines_node{
    char* arr;
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

#endif // DATA_STRUCTURE_H_INCLUDED
