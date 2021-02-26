#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4
#define NODES_SIZE 50
#define DEFAULT_GAP_SIZE 10

typedef struct lines_node{
    char* arr;
    int gap_left,gap_right, gap_size;
    struct lines_node *next;
}lines_node;

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
            //return;
    }
    return;
}

typedef struct line{
    lines_node* head;
    //int line_size;
} line;

void init_line(line* l){
    l->head = NULL;
    //line_size = 0;
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
    while(len>0){
        lines_node* new = (lines_node*)malloc(sizeof(lines_node));
        init_lines_node(new);
        strncpy(new->arr, &arr[index], NODES_SIZE - DEFAULT_GAP_SIZE);
        if(len>NODES_SIZE)
            load_fullnode(new);
        else{
            new->gap_left = len;
            new->gap_right = NODES_SIZE - 1;
            new->gap_size = NODES_SIZE - len;
        }

        append_in_line(l, new);
        len -= NODES_SIZE;
        index += NODES_SIZE;
    }
    
    
}

void print_line(line l){

    lines_node *p = l.head;
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

// Function that is used to move the gap  
// left in the array  
void left(lines_node* node, int position)  
{  
    if(node->gap_size == 0){
		node->gap_left = position;
		node->gap_right = position - 1;
        return;
	}
        
    // Move the gap left character by character  
    // and the buffers  
    while (position < node->gap_left) {  
        node->gap_left--;  
        node->gap_right--;  
        node->arr[node->gap_right+1] = node->arr[node->gap_left];
    }  
}

void right(lines_node* node, int position)  
{
    if(node->gap_size == 0) {
        /*if this is last node of line and position is greater than size of node*/
		if(position >= NODES_SIZE) {            
			node->gap_left = NODES_SIZE;
			node->gap_right = NODES_SIZE - 1;
			return;
		}
		node->gap_left = position;
		node->gap_right = position - 1;
		return;
	}  
    // Move the gap right character by character  
    // and the buffers  
    while (position > node->gap_left) {  
        node->gap_left++;  
        node->gap_right++;  
        node->arr[node->gap_left-1] = node->arr[node->gap_right];  
    }  
}

lines_node* move_cursor(lines_node* node, int position){
    //find node to which position belongs or find last node
    while(position > (NODES_SIZE - node->gap_size)){       
        position -= NODES_SIZE - node->gap_size;
        if(node->next == NULL)
            break;
        node = node->next;
    }

    if(position < node->gap_left)
        left(node, position);
    if(position > node->gap_left)
        left(node, position);
    return node;
}

void insert_character(line l, int position, char data){

    lines_node* node = move_cursor(l.head, position);

    if(node->gap_size == 0){
        lines_node* newnode = (lines_node*)malloc(sizeof(lines_node*));
        if(newnode == NULL)
            return;
        init_lines_node(newnode);
        newnode->next = node->next;
        node->next = newnode;

        int copydata_len = NODES_SIZE - node->gap_left;

        strncpy(newnode->arr, &node->arr[node->gap_left], copydata_len);
        newnode->gap_left = copydata_len + 1;
        newnode->gap_size = node->gap_left - 1;
        node->gap_right = NODES_SIZE - 1;
        node->gap_size = copydata_len;

        //for last position
        if(node->gap_left >= NODES_SIZE)
            node = node->next;
    }
    node->arr[node->gap_left++] = data;
    node->gap_size--;
    return;
}

int main(){

    FILE* fptr = fopen("file.txt", "r");
    buffer b;
    init_buffer(b);
    read_file_firsttime(fptr, &b);
    print_buffer(b);
    printf("-----------------------------------------\n");
    int position = 7;
    move_cursor(b.head_array[0].head, position);
    insert_character(b.head_array[0], position, 'g');
    print_buffer(b);

    return 0;
}


