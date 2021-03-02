#include<stdlib.h>
#include<stdio.h>
#include"function.h"


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
void backspace(buffer *b, int line_no, int position){
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

void load_next_line(buffer *b){
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
    if (b->head_array[b->head_index].head != NULL)
        write_line(b->fprev, b->head_array[b->head_index]);
    else
        return;

	destroy_line(&b->head_array[b->head_index]);	//TODO: try to do without destroying

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

void load_prev_line(buffer *b){
	// check if line is present in fprev file
	if(fseek(b->fprev, -1, SEEK_CUR) != -1)
		fseek(b->fprev, 1, SEEK_CUR);
	else
        return;            //line not present to load previous line

	b->head_index = (b->head_index - 1 + b->size) % b->size;

    if (b->head_array[b->head_index].head != NULL)          //if last line is present store it
        write_line(b->fnext, b->head_array[b->head_index]);

    destroy_line(&b->head_array[b->head_index]); // TODO: try to do without free

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
