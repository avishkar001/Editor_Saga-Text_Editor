#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define BUFFER_SIZE 25
#define NODES_SIZE 50
#define DEFAULT_GAP_SIZE 10
#define WINDOWS 0

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
    line *head_array;
    int head_index;
    int size;
    FILE *fptr, *fprev, *fnext;
}buffer;

void init_buffer(buffer *b, int size, char* filename){
    b->head_index = 0;
    b->head_array = (line*)malloc(sizeof(line)*size);
    b->size = size;
    b->fptr = fopen(filename, "r");
    b->fprev = fopen("fprev.txt", "w+");
    b->fnext = fopen("fprev.txt", "w+"); 
}

void read_file_firsttime(buffer* b){

    size_t len;

    for(int i=0; i<b->size; i++){
        len = 50;
        char* data = (char*)malloc(sizeof(char)*len);
        len = getline(&data, &len, b->fptr);
        line* newline = (line*)malloc(sizeof(line));
        init_line(newline);
        insert_in_line(newline, data, len);
        set_line_size(newline);
        b->head_array[(b->head_index + i) % b->size] = *newline;
    }
    return;
}

void print_buffer(buffer b){
    
    for(int i=0; i < b.size; i++){
        print_line(b.head_array[(b.head_index + i) % b.size]);
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
    
    for(int i=0; i < b.size; i++){
        write_line(f, b.head_array[(b.head_index + i) % b.size]);
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
   else
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


void load_next_line(buffer *b);

// TODO delete line if del position is 0
void backspace(buffer *b, int line_no, int position) {


	if(position == 0){
		if(line_no > 0){
			lines_node* p = b->head_array[((line_no - 1) + b->head_index) % b->size].head;
			
			while(p->next)
				p = p->next;
				
            
			p->next = b->head_array[(line_no + b->head_index) % b->size].head;
			for(int i = 0; i < (b->size - line_no); i++)
                b->head_array[(line_no + b->head_index + i) % b->size] = b->head_array[(line_no + i + 1 + b->head_index) % b->size];
            b->head_index = (b->head_index - 1 + b->size) % b->size;
            b->head_array[b->head_index].head = NULL; 
            b->head_array[b->head_index].line_size = 0;
            load_next_line(b);
		}
	}
	//numbering of line starts from 0
    //if head is at nonzero position add that offset 
    lines_node* node = b->head_array[(line_no + b->head_index) % b->size].head;
	
	lines_node* prev_node = NULL;
	
	//find node to whic position belongs or last node if position is too big
    while((NODES_SIZE - node->gap_size) < position) {
                
        if(node->next == NULL)       //stop at last node if position is too big
            break;
           prev_node = node;
        position -= (NODES_SIZE - node->gap_size);
        node = node->next;
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
    b->head_array[(line_no + b->head_index) % b->size].line_size--;
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



void load_next_line(buffer *b) {	
	char ch;
	int fnext_flag = 0;     //flag showing whether line is to be loaded from fnext file
	if(fseek(b->fnext, -1, SEEK_CUR) != -1) {          //check if file has line
		fseek(b->fnext, 1, SEEK_CUR);                  //restore file pointer
		fnext_flag = 1;
        //printf("debug\n");
	}
	else if((ch = fgetc(b->fptr)) == -1){
		//return if file is empty
		return;
	}
    ungetc(ch, b->fptr);

    /*write first line to tmp file*/
    if (b->head_array[b->head_index].head != NULL){ 
        write_line(b->fprev, b->head_array[b->head_index]);
        destroy_line(&b->head_array[b->head_index]);
    }
    //else
        //return;           TODO:check this case
	//TODO: try to do without destroying

	char c = ' ';
    unsigned long position;
    fseek(b->fnext, -1, SEEK_CUR);
    if (fnext_flag){

        if (WINDOWS)
            fseek(b->fnext, -1, SEEK_CUR);
        //find start of line
        while(c != '\n'){
            if (fseek(b->fnext, -2, SEEK_CUR) == -1){
                fseek(b->fnext, 0, SEEK_SET);             //if file has only 1 line
                break;
            }
            c = fgetc(b->fnext);
        }
        //fflush(fp);
        position = ftell(b->fnext);
    }

    /*TODO:try to write a function to read line from file, make code more modular */
    size_t len = 50;
    char *data = (char *)malloc(sizeof(char) * len);
    if(fnext_flag)
        len = getline(&data, &len, b->fnext);
    else
        len = getline(&data, &len, b->fptr);
    
    line *newline = (line *)malloc(sizeof(line));
    init_line(newline);
    insert_in_line(newline, data, len);
    
   
    set_line_size(newline);
    b->head_array[b->head_index] = *newline;

    /*after reading a line, set file pointer to beginning indicating line is deleted */
    if(fnext_flag)
        fseek(b->fnext, position, SEEK_SET);          
        
    b->head_index = (b->head_index + 1) % b->size;        //change start position of cicular array

	return;
}


void load_prev_line(buffer *b) {
	// check if line is present in fprev file
	if(fseek(b->fprev, -1, SEEK_CUR) != -1)	
		fseek(b->fprev, 1, SEEK_CUR);
	else
        return;            //line not present to load previous line

	b->head_index = (b->head_index - 1 + b->size) % b->size;

    if (b->head_array[b->head_index].head != NULL){         //if last line is present store it
        write_line(b->fnext, b->head_array[b->head_index]);
        destroy_line(&b->head_array[b->head_index]);
    }
     // TODO: try to do without free

    char c = ' ';
    unsigned long position;
    if(WINDOWS)              //because newline character is diff in linux and windows
        fseek(b->fprev, -1, SEEK_CUR);
    
    while (c != '\n'){
        //printf("debugging\n");
        if (fseek(b->fprev, -2, SEEK_CUR) == -1){
            fseek(b->fprev, 0, SEEK_SET);  //if file has only 1 line, go to starting position
            break;
        }
        c = fgetc(b->fprev);
    }
    //fflush(fprev);
    position = ftell(b->fprev);

    /*TODO:try to write a function to read line from file, make code more modular */
    size_t len = 50;
    char *data = (char *)malloc(sizeof(char) * len);
    len = getline(&data, &len, b->fprev);
    line *newline = (line *)malloc(sizeof(line));
    init_line(newline);
    insert_in_line(newline, data, len);
    set_line_size(newline);
    b->head_array[b->head_index] = *newline;

    /*after reading a line, set file pointer to beginning indicating line is deleted */
    fseek(b->fprev, position, SEEK_SET);
	return;
}


void print_loc(int x, int y) {
        move(27, 20);
        mvprintw(27, 30, "x: %d y: %d", x, y);
}

void print_line_ncurses(line l, int line_num){

    lines_node *p = l.head;
    int col_num = 0;

    move(line_num, 0);
    clrtoeol();
    while(p){
        for(int i=0; i<NODES_SIZE; i++){
            if(i==p->gap_left && p->gap_size != 0){
                i=p->gap_right;
                continue;
            }
            mvaddch(line_num, col_num++, p->arr[i]);
        }
        p = p->next;
    }
    return;
}



void print_page_ncurses(buffer b){
    for(int i=0; i < b.size; i++){
        print_line_ncurses(b.head_array[(b.head_index + i) % b.size], i);
    }
}


//TODO:handle EOF character
int main(int argc, char **argv){
   	
    buffer b;
    init_buffer(&b, 24, "file.txt");
   
    read_file_firsttime(&b);
/*    print_buffer(b);
    printf("-----------------------------------------\n");

    for(int i = b.head_index; i< b.size; i++){
        printf("%d\n", b.head_array[i].line_size);
    }

    int position = 7;
    move_cursor(b.head_array[0].head, position);
    insert_character(&b.head_array[0], position - 1, 'g');
    print_buffer(b);
    for(int i = 0; i<b.size; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % b.size].line_size);
    }
    printf("-----------------------------------------\n");
    backspace(&b,0 ,position);
    print_buffer(b);
    for(int i = 0; i<b.size; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % b.size].line_size);
    }
    
    //write_buffer(fprev, b);
 
    int len = 1;
    char *data = (char *)malloc(sizeof(char) * len);
    len = getline(&data, &len, fprev);
    printf("88\n%s", data);

    for(int i = 0; i<b.size; i++){
        printf("%d\n", b.head_array[(i + b.head_index) % b.size].line_size);
    }
*/  
/*    
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
*/
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
    

    print_page_ncurses(b);
    
    print_loc(line_no, col_no);
    move(line_no, col_no);
    while (1){
        ch = getch();
        switch (ch){
		case KEY_F(1):
                endwin();
                return 0;

            case KEY_LEFT:
                if (col_no)
                    col_no--;
                break;

            case KEY_RIGHT:
                if(col_no < b.head_array[(line_no + b.head_index) % b.size].line_size - 1)
                    col_no++;
                break;

            case KEY_DOWN:
                if(line_no < b.size - 1)
                    line_no++;
                else
                    load_next_line(&b);

                if(col_no > b.head_array[(line_no + b.head_index) % b.size].line_size - 1)
                    col_no = b.head_array[(line_no + b.head_index) % b.size].line_size - 1;
                break;

            case KEY_UP:
                if (line_no > 0)
                    line_no--;
                else
                    load_prev_line(&b);

                if (col_no > b.head_array[(line_no + b.head_index) % b.size].line_size)
                    col_no = b.head_array[(line_no + b.head_index) % b.size].line_size;
		break;
	    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y':case 'z':case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y':case 'Z':case ' ':
		insert_character(&b.head_array[(line_no + b.head_index) % b.size], col_no, ch);
		col_no++;
		break;
	    case KEY_BACKSPACE:
	    	if(col_no){
                backspace(&b, line_no, col_no);
                col_no--;
		    }
            else{
                backspace(&b, line_no, col_no);
                col_no = b.head_array[(b.head_index + line_no - 1)%b.size].line_size - 1;
                line_no--;
            }
		break;
	    default:
                break;
            }
            print_page_ncurses(b);
            print_loc(line_no, col_no);
            move(line_no, col_no);
    }
    return 0;
}


