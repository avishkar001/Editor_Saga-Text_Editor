#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define BUFFER_SIZE 4
#define NODES_SIZE 50
#define DEFAULT_GAP_SIZE 10
#define WINDOWS 1

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
    int line_size;
} line;

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
    while(len>0){
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
    return;
}

typedef struct buffer{
    line head_array[BUFFER_SIZE];
    int head_index;
}buffer;

void init_buffer(buffer *b){
    b->head_index = 0;
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
        set_line_size(newline);
        b->head_array[(b->head_index + i) % BUFFER_SIZE] = *newline;
    }
    return;
}

void print_buffer(buffer b){
    
    for(int i=0; i < BUFFER_SIZE; i++){
        print_line(b.head_array[(b.head_index + i) % BUFFER_SIZE]);
    }

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
            //return;
    }
    return;
}

void write_line(FILE* f, line l){

    lines_node *p = l.head;
    while(p){
        write_node(f, *p);
        p = p->next;
    }
    return;
}

void write_buffer(FILE* f, buffer b){
    
    for(int i=0; i < BUFFER_SIZE; i++){
        write_line(f, b.head_array[(b.head_index + i) % BUFFER_SIZE]);
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

void insert_character(line *l, int position, char data){

    lines_node* node = move_cursor(l->head, position);

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
    l->line_size++;

    return;
}

// TODO delete line if del position is 0
void backspace(buffer *b, int line_no, int position) {
	//numbering of line starts from 0
    //if head is at nonzero position add that offset 
    lines_node* node = b->head_array[(line_no + b->head_index) % BUFFER_SIZE].head;
    
    lines_node *prev_node = NULL;
	
	
	//find node to whic position belongs or last node if position is too big
    while((NODES_SIZE - node->gap_size) < position) {
                
        if(node->next == NULL)       //stop at last node if position is too big
            break;
        prev_node = node;
        node = node->next;

        position -= (NODES_SIZE - node->gap_size);
    }

	//bring gap buffer at required position
    if (position < node->gap_left)
        left(node, position);
    else if(position > node->gap_right)
        right(node, position);

	// if left gap boundary is at 0, take previous node
	if(node->gap_left == 0) {
		if(prev_node)
			node = prev_node;
		else
			return;
	}
	//delete chaaracter by growing gap
	node->gap_left--;
	node->gap_size++;
    b->head_array[(line_no + b->head_index) % BUFFER_SIZE].line_size--;
	return;
}

void destroy_line(line* l){

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



void load_next_line(buffer *b, FILE *fprev, FILE *fnext, FILE *fmain) {	
	char ch;
	int fnext_flag = 0;     //flag showing whether line is to be loaded from fnext file
	if(fseek(fnext, -1, SEEK_CUR) != -1) {          //check if file has line
		fseek(fnext, 1, SEEK_CUR);                  //restore file pointer
		fnext_flag = 1;
        //printf("debug\n");
	}
	else if((ch = fgetc(fmain)) == -1){
		//return if file is empty
		return;
	}
    ungetc(ch, fmain);

    /*write first line to tmp file*/
    if (b->head_array[b->head_index].head != NULL) 
        write_line(fprev, b->head_array[b->head_index]);
    else
        return;

	destroy_line(&b->head_array[b->head_index]);	//TODO: try to do without destroying

	char c = ' ';
    unsigned long position;
    fseek(fnext, -1, SEEK_CUR);
    if (fnext_flag){

        if (WINDOWS)
            fseek(fnext, -1, SEEK_CUR);
        //find start of line
        while(c != '\n'){
            if (fseek(fnext, -2, SEEK_CUR) == -1){
                fseek(fnext, 0, SEEK_SET);             //if file has only 1 line
                break;
            }
            c = fgetc(fnext);
        }
        //fflush(fp);
        position = ftell(fnext);
    }

    /*TODO:try to write a function to read line from file, make code more modular */
    int len = 50;
    char *data = (char *)malloc(sizeof(char) * len);
    if(fnext_flag)
        len = getline(&data, &len, fnext);
    else
        len = getline(&data, &len, fmain);
    
    line *newline = (line *)malloc(sizeof(line));
    init_line(newline);
    insert_in_line(newline, data, len);
    
    printf("hi%d", newline->line_size);
    set_line_size(newline);
    printf("hi%d", newline->line_size);
    b->head_array[b->head_index] = *newline;

    /*after reading a line, set file pointer to beginning indicating line is deleted */
    if(fnext_flag)
        fseek(fnext, position, SEEK_SET);          
        
    b->head_index = (b->head_index + 1) % BUFFER_SIZE;        //change start position of cicular array

	return;
}


void load_prev_line(buffer *b, FILE *fprev, FILE *fnext) {
	// check if line is present in fprev file
	if(fseek(fprev, -1, SEEK_CUR) != -1)	
		fseek(fprev, 1, SEEK_CUR);
	else
        return;            //line not present to load previous line

	b->head_index = (b->head_index - 1 + BUFFER_SIZE) % BUFFER_SIZE;

    if (b->head_array[b->head_index].head != NULL)          //if last line is present store it
        write_line(fnext, b->head_array[b->head_index]);

    destroy_line(&b->head_array[b->head_index]); // TODO: try to do without free

    char c = ' ';
    unsigned long position;
    if(WINDOWS)              //because newline character is diff in linux and windows
        fseek(fprev, -1, SEEK_CUR);
    
    while (c != '\n'){
        //printf("debugging\n");
        if (fseek(fprev, -2, SEEK_CUR) == -1){
            fseek(fprev, 0, SEEK_SET);  //if file has only 1 line, go to starting position
            break;
        }
        c = fgetc(fprev);
    }
    //fflush(fprev);
    position = ftell(fprev);

    /*TODO:try to write a function to read line from file, make code more modular */
    int len = 50;
    char *data = (char *)malloc(sizeof(char) * len);
    len = getline(&data, &len, fprev);
    line *newline = (line *)malloc(sizeof(line));
    init_line(newline);
    insert_in_line(newline, data, len);
    set_line_size(newline);
    b->head_array[b->head_index] = *newline;

    /*after reading a line, set file pointer to beginning indicating line is deleted */
    fseek(fprev, position, SEEK_SET);
	return;
}


void print_loc(int x, int y) {
        move(20, 20);
        mvprintw(10, 30, "x: %d y: %d", x, y);
}


void print_page(buffer b){
    for(int i=0; i < BUFFER_SIZE; i++){
        print_line(b.head_array[(b.head_index + i) % BUFFER_SIZE], i);
    }
}

void print_line(line l, int line_num){

    lines_node *p = l.head;
    int col_num = 0;

    move(i, 0);
    clrtoeol();
    while(p){
        for(int i=0; i<NODES_SIZE; i++){
            if(i==node.gap_left && node.gap_size != 0){
                i=node.gap_right;
                continue;
            }
            mvaddch(line_num, col_num,node.arr[i]);
        }
        p = p->next;
    }
    return;
}

//TODO:handle EOF character
int main(){

    FILE* fptr = fopen("file.txt", "r");
    FILE* fprev = fopen("fprev.txt", "w+");
    FILE* fnext = fopen("fnext.txt", "w+");
    
    buffer b;
    init_buffer(&b);
    
    read_file_firsttime(fptr, &b);
    print_buffer(b);
    printf("-----------------------------------------\n");

    for(int i = b.head_index; i<BUFFER_SIZE; i++){
        printf("%d\n", b.head_array[i].line_size);
    }
/*
    int position = 7;
    move_cursor(b.head_array[0].head, position);
    insert_character(&b.head_array[0], position - 1, 'g');
    print_buffer(b);
    for(int i = 0; i<BUFFER_SIZE; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % BUFFER_SIZE].line_size);
    }
    printf("-----------------------------------------\n");
    backspace(&b,0 ,position);
    print_buffer(b);
    for(int i = 0; i<BUFFER_SIZE; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % BUFFER_SIZE].line_size);
    }
    
    //write_buffer(fprev, b);
 
    int len = 1;
    char *data = (char *)malloc(sizeof(char) * len);
    len = getline(&data, &len, fprev);
    printf("88\n%s", data);

    for(int i = 0; i<BUFFER_SIZE; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % BUFFER_SIZE].line_size);
    }
*/  
    
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    
    
    load_prev_line(&b, fprev, fnext);
    print_buffer(b);
    printf("-----------------------------------------\n");
    load_prev_line(&b, fprev, fnext);
    print_buffer(b); 
    printf("-----------------------------------------\n");

    
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
/*
    printf("%ld", ftell(fprev));
    fseek(fprev, -2, SEEK_CUR);
    printf("%ld", ftell(fprev));
    printf("a%da", fgetc(fprev));
    printf("%ld", ftell(fprev));

    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    load_prev_line(&b, fprev, fnext);
    print_buffer(b);
    printf("-----------------------------------------\n");

    


    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
 

    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
  printf("-----------------------------------------\n");
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");
    load_next_line(&b, fprev, fnext, fptr);
    print_buffer(b);
    printf("-----------------------------------------\n");

*/

    // curses interface
    initscr();
    noecho();
    keypad(stdscr, true);

    int ch;
    int line_no = 0, col_no = 0;

    print_page(b);
    print_loc(line_no, col_no);
    move(line_no, col_no);
    while (1){
        ch = getch();
        switch (ch){
            case 'q':
                endwin();
                return 0;

            case KEY_LEFT:
                if (col_no)
                    col_no--;
                break;

            case KEY_RIGHT:
                //if (col_no < (window_1.head)[head_index(window_1, line_no)].line_size)
                if(col_no < b.head_array[(line_no + b.head_index) % BUFFER_SIZE].line_size)
                    col_no++;
                break;

            case KEY_DOWN:
                //if (line_no < window_1.tot_lines - 1)
                if(line_no < BUFFER_SIZE - 1){
                    line_no++;
                else
                    load_next_line(&b, fprev, fnext, fptr);

                //if (col_no > (window_1.head)[h_indx].line_size)
                    //col_no = (window_1.head)[h_indx].line_size;
                if(col_no > b.head_array[(line_no + b.head_index) % BUFFER_SIZE].line_size)
                    col_no = b.head_array[(line_no + b.head_index) % BUFFER_SIZE].line_size;
                break;

            case KEY_UP:
                if (line_no > 0)
                    line_no--;
                else
                    load_prev_line(&b, fprev, fnext);

                if (col_no > (window_1.head)[h_indx].line_size)
                    col_no = (window_1.head)[h_indx].line_size;
            default:
                break;
            }
            print_page(b);
            print_loc(line_no, col_no);
            move(line_no, col_no);
    }
    return 0;
}


