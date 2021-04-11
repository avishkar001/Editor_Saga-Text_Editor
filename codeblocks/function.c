#include<stdlib.h>
#include<stdio.h>
#include"function.h"
#include"data_structure.h"
#include<string.h>
#include<limits.h>
#include <sys/stat.h>

void save_file(buffer *b){
    
	FILE* fsave = fopen("save.txt", "w");
	
    struct stat st;
    stat(b->filename, &st);
    chmod("save.txt", st.st_mode);
	
    long int no_of_bytes;
	char c  = ' ';

	no_of_bytes = ftell(b->fprev);
    fseek(b->fprev, 0, SEEK_SET);

    for(int i=0; i<no_of_bytes; i++){
        
        c = fgetc(b->fprev);
        fputc(c, fsave);
    }

    
    write_buffer(fsave, *b);
    
    while(ftell(b->fnext) != 0){    
        c = ' ';
        while(c != '\n'){
            if (fseek(b->fnext, -2, SEEK_CUR) == -1){
                fseek(b->fnext, 0, SEEK_SET);             //if file has only 1 line
                break;
            }
            c = fgetc(b->fnext);
        }
            //fflush(fp);
        no_of_bytes = ftell(b->fnext);

        c = fgetc(b->fnext);
        
        while (c != '\n'){
        
            fputc(c, fsave);
            c = fgetc(b->fnext);
        }

        fputc(c, fsave);
        fseek(b->fnext, no_of_bytes, SEEK_SET);
        
    }
    
    c = fgetc(b->fptr);
	
    while (c != EOF){
    
        fputc(c, fsave);
        c = fgetc(b->fptr);
    }
    /*
    fcloseall();
    remove(b->filename);
    rename("save.txt", b->filename);
    
	
	int flag = 0;
	while(ftell(b->fnext) != 0){
		load_next_line(b);
		flag = 1;
	}
	write_buffer(b->fprev, *b);
	
	char c;
	//fflush(b->fptr);
	fflush(b->fprev);
	//char arr[100];
	//fscanf(b->fptr, "[^\n]%s", arr);
	
	//while ((c = fgetc(b->fptr)) != '\n');
	if(flag){
		for(int i=0; i<NODES_SIZE - DEFAULT_GAP_SIZE - 1; i++)
			c = fgetc(b->fptr);
	}
	c = fgetc(b->fptr);
	
    while (c != EOF){
    
        fputc(c, b->fprev);
        c = fgetc(b->fptr);
    }
    */
}


void insert_string(buffer *b, int line_no, int position, char* data, int datalen){

	for(int i=0; i<datalen; i++)
		insert_character(b, line_no, position + i, data[i]);

}

int is_comment(buffer *b, int line_no){
	
	lines_node* p = b->head_array[(line_no + b->head_index) % b->size].head;
	
	char ch_prev = ' ';
	while(p){
		for(int i=0; i<NODES_SIZE; i++){
			if(i==p->gap_left && p->gap_size != 0){
				i=p->gap_right;
				continue;
			}
			if(p->arr[i] != ' ' && p->arr[i] != '/')
				return 0;
			if(ch_prev == '/' && p->arr[i] == '/')
				return 1;
			ch_prev = p->arr[i];
		}
		p = p->next;
	}
	
}

char last_character(buffer *b, int line_no){
	
	lines_node* p = b->head_array[(line_no + b->head_index) % b->size].head;
	
	while(p->next)
		p = p->next;
		
	if(p->gap_size == NODES_SIZE)
		return CHAR_MIN;
		
	else return p->arr[p->gap_left - 1];
}


void insert_character(buffer *b, int line_no, int position, char data){
	
	line* l = &b->head_array[(line_no + b->head_index) % b->size];
    lines_node* node = move_cursor(l->head, position);

	if(data == '\n'){
	
        lines_node* newnode = (lines_node*)malloc(sizeof(lines_node));
        if(newnode == NULL)
            return;
        init_lines_node(newnode);
        newnode->next = node->next;
        node->next = NULL;
        int copydata_len = NODES_SIZE - node->gap_right - 1;

        strncpy(newnode->arr, &node->arr[node->gap_right + 1], copydata_len);
        newnode->gap_left = copydata_len;
        newnode->gap_right = NODES_SIZE - 1;
        node->gap_right = NODES_SIZE - 1;
        node->gap_size = node->gap_right - node->gap_left + 1;
        newnode->gap_size = newnode->gap_right - newnode->gap_left + 1;

        /*for last position
        if(node->gap_left >= NODES_SIZE)
            node = node->next;
        */
        
		b->head_array[(line_no + b->head_index) % b->size].line_size = get_line_size(b->head_array[(line_no + b->head_index) % b->size]);
		//printf("%d", get_line_size(b->head_array[(line_no + b->head_index) % b->size]));
		
        int i = b->head_index - 1;
		while(i != line_no + b->head_index + 1){
			if(i == -1){
				i = b->size - 1;
				continue;
			}
			//b->head_array[(line_no + b->head_index + i) % b->size] = b->head_array[(line_no + i - 1 + b->head_index) % b->size];
			b->head_array[i] = b->head_array[(i - 1 + b->size) % b->size];
			i--;
		}
		
        b->head_array[(line_no + b->head_index + 1) % b->size].head = newnode;
        b->head_array[(line_no + b->head_index + 1) % b->size].line_size = get_line_size(b->head_array[(line_no + b->head_index + 1) % b->size]);
        //set_line_size(&b->head_array[(line_no + b->head_index + 1) % b->size]);
        
		
        
        
        //for(int i = (b->head_index + b->size - 1) % ; i >= (line_no + b->head_index + i) % b->size ; i--)
                //b->head_array[(line_no + b->head_index + i) % b->size] = b->head_array[(line_no + i - 1 + b->head_index) % b->size];
                return;
    }

    if(node->gap_size == 0){
        lines_node* newnode = (lines_node*)malloc(sizeof(lines_node));
        if(newnode == NULL)
            return;
        init_lines_node(newnode);
        newnode->next = node->next;
        node->next = newnode;

        int copydata_len = NODES_SIZE - node->gap_left;

        strncpy(newnode->arr, &node->arr[node->gap_left], copydata_len);
        newnode->gap_left = copydata_len;
        newnode->gap_size = node->gap_left;

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
void backspace(buffer *b, int line_no, int position){


	if(position == 0){
		if(line_no > 0){
			lines_node* p = b->head_array[((line_no - 1) + b->head_index) % b->size].head;
			
			while(p->next)
				p = p->next;
			
			p->next = b->head_array[(line_no + b->head_index) % b->size].head;
			b->head_array[((line_no - 1) + b->head_index) % b->size].line_size += b->head_array[(line_no + b->head_index) % b->size].line_size;
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

	//find node to which position belongs or last node if position is too big
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
	//delete character by growing gap
	node->gap_left--;
	node->gap_size++;
    b->head_array[(line_no + b->head_index) % b->size].line_size--;
	return;
}

void load_next_line(buffer *b){
	char ch;
	int fnext_flag = 0;     //flag showing whether line is to be loaded from fnext file
	if(fseek(b->fnext, -1, SEEK_CUR) != -1) {          //check if file has line
		fseek(b->fnext, 1, SEEK_CUR);                  //restore file pointer
		fnext_flag = 1;
        //printf("debug\n");
	}
    else{
        if((ch = fgetc(b->fptr)) == -1)
		    return;                //return if file is empty
		else
            ungetc(ch, b->fptr);
	}
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
    //fseek(b->fnext, -1, SEEK_CUR);
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

    if(len>0 && data[len-1] == '\n')
		len--;
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

void load_prev_line(buffer *b){
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
    size_t len = NODES_SIZE;
    char *data = (char *)malloc(sizeof(char) * len);
    len = getline(&data, &len, b->fprev);
    if(len>0 && data[len-1] == '\n')
		len--;
    line *newline = (line*)malloc(sizeof(line));
    init_line(newline);
    insert_in_line(newline, data, len);
    set_line_size(newline);
    b->head_array[b->head_index] = *newline;

    /*after reading a line, set file pointer to beginning indicating line is deleted */
    fseek(b->fprev, position, SEEK_SET);
	return;
}

void read_file_firsttime(buffer* b){

    size_t len;

    for(int i=0; i<b->size; i++){
        len = NODES_SIZE;
        char* data = (char*)malloc(sizeof(char)*len);
        len = getline(&data, &len, b->fptr);
        if(data[len -1] == '\n')
        	len--;
        line* newline = (line*)malloc(sizeof(line));
        init_line(newline);
        insert_in_line(newline, data, len);
        set_line_size(newline);
        b->head_array[(b->head_index + i) % b->size] = *newline;
    }
    return;
}
