#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include "gui.h"
#include "shortcuts.h"
#include "c_stack.h"
#ifndef CTRL
#define	CTRL(c)	(c & 037)
#endif

#define CURSOR_CHANGE 'c'
#define LINE_CHANGE 'l'
#define PAGE_CHANGE 'p'

int main(int argc, char **argv){

    buffer b;
    init_buffer(&b, 35, "file2.txt");

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

	c_stack undo;
	stack_node* snode;
    init(&undo,20);
	position cur_position = {0 , 0};
	char deleted_char;

    // curses interface
    initscr();
    noecho();
    keypad(stdscr, true);

    int ch;
    int line_no = 0, col_no = 0;

	char str[100];

    print_page_ncurses(b);

    print_loc(line_no, col_no);
    move(line_no, col_no);
    while (1){
    	char change;
        ch = getch();
        switch (ch){
			case KEY_F(1):
                endwin();
                return 0;

			
			case KEY_F(2):
				if(b.mode == READ_ONLY)
					break;
				insert_string(&b, line_no, col_no, F2_SHORTCUT, strlen(F2_SHORTCUT));
				col_no += 7;
				change = LINE_CHANGE;
				break;

			case KEY_F(3):
				if(b.mode == READ_ONLY)
					break;
				insert_string(&b, line_no, col_no, F3_SHORTCUT, strlen(F3_SHORTCUT));
				col_no += strlen(F3_SHORTCUT);
				change = LINE_CHANGE;
				break;
				
			case KEY_F(4):
				if(b.mode == READ_ONLY)
					break;
				insert_string(&b, line_no, col_no, F4_SHORTCUT, strlen(F4_SHORTCUT));
				col_no += strlen(F4_SHORTCUT);
				change = LINE_CHANGE;
				break;
				
			case KEY_F(5):
				if(b.mode == READ_ONLY)
					break;
				save_file(&b);
				endwin();
                return 0;
				break;
				
			case KEY_F(6):
				if(b.mode == READ_ONLY)
					break;
				if(last_character(&b, line_no) == ';')
					backspace(&b, line_no, b.head_array[(line_no + b.head_index) % b.size].line_size);
				else
					insert_character(&b, line_no, b.head_array[(line_no + b.head_index) % b.size].line_size, ';');
				
				change = LINE_CHANGE;
				break;
			
			case KEY_F(7):
				if(b.mode == READ_ONLY)
					break;
				if(!is_comment(&b, line_no)){
					insert_character(&b, line_no, 0, '/');
					insert_character(&b, line_no, 0, '/');
				}
				change = LINE_CHANGE;
				break;

			case KEY_F(8):
				snode = pop(&undo);
    			if(snode == NULL){
					break;
				}
				if(snode->operation_id == OPERATION_NAVIGATE){
					line_no = snode->final_position.line_no;
					col_no = snode->final_position.col_no;
					change = CURSOR_CHANGE;
				}
				else if(snode->operation_id == OPERATION_INSERT){
					int i = 0;
					while(i <= snode->len_of_arr){
						backspace(&b, snode->final_position.line_no, snode->final_position.col_no - i);
						i++;
					}
					line_no = snode->final_position.line_no;
					col_no = snode->final_position.col_no - snode->len_of_arr;
					change = LINE_CHANGE;
				}else if(snode->operation_id == OPERATION_DELETE){
					int i = 0;
					while(i <= snode->len_of_arr){
						insert_character(&b, snode->final_position.line_no, snode->final_position.col_no + i, snode->array[snode->len_of_arr - i]);
						i++;
					}
					line_no = snode->final_position.line_no;
					col_no = snode->final_position.col_no + snode->len_of_arr;
					change = LINE_CHANGE;
				}	
				
				break;
				
		    case KEY_LEFT:
				//for undo
				cur_position.line_no = line_no;
				cur_position.col_no = col_no;
				push(&undo, 1, cur_position, 'h');

		        if (col_no)
		            col_no--;
				change = CURSOR_CHANGE;
		        break;

		    case KEY_RIGHT:
				//for undo
				cur_position.line_no = line_no;
				cur_position.col_no = col_no;
				push(&undo, 1, cur_position, 'h');

		        if(col_no < b.head_array[(line_no + b.head_index) % b.size].line_size)
		            col_no++;
		        change = CURSOR_CHANGE;
		        break;

		    case KEY_DOWN:
		    	if(b.head_array[(line_no + 1 + b.head_index) % b.size].head == NULL)
		    		break;
		    
		        if(line_no < b.size - 1){

					//for undo
					cur_position.line_no = line_no;
					cur_position.col_no = col_no;
					push(&undo, 1, cur_position, 'h');

		            line_no++;
		        	change = CURSOR_CHANGE;
		        }
		        else{
		        
		            if(load_next_line(&b))
			        	change = PAGE_CHANGE;
			        else{
			        	change = CURSOR_CHANGE;
			        	break;
			        }
		        }
		        if(col_no > b.head_array[(line_no + b.head_index) % b.size].line_size)
		            col_no = b.head_array[(line_no + b.head_index) % b.size].line_size;
		        break;

		    case KEY_UP:
		        if (line_no > 0){

					//for undo
					cur_position.line_no = line_no;
					cur_position.col_no = col_no;
					push(&undo, 1, cur_position, 'h');

		            line_no--;
		        	change = CURSOR_CHANGE;

		        }
		        else{
		            if(load_prev_line(&b))
			        	change = PAGE_CHANGE;
			        else{
			        	change = CURSOR_CHANGE;
			        	break;
			        }
		        }
		        if (col_no > b.head_array[(line_no + b.head_index) % b.size].line_size)
		            col_no = b.head_array[(line_no + b.head_index) % b.size].line_size;
				break;
				
			case KEY_END:
				col_no = b.head_array[(line_no + b.head_index) % b.size].line_size;
				break;
			
			case KEY_HOME:
				col_no = 0;
				break;
				
			case KEY_NPAGE:
				for(int i=0; i<b.size; i++)
					load_next_line(&b);
				break;
					
			case KEY_PPAGE:
				for(int i=0; i<b.size; i++)
					load_prev_line(&b);
				break;
				
			case '\t':
				if(b.mode == READ_ONLY)
					break;
				insert_string(&b, line_no, col_no, "    ", 4);
				col_no += 4;
				change = LINE_CHANGE;
				break;
				
			case '\n':
				if(b.mode == READ_ONLY)
					break;
				if(line_no == b.size -1)
					write_line(b.fprev, b.head_array[b.head_index]);
				else if(b.head_array[(b.head_index - 1 + b.size)%b.size].head != NULL)	
					write_line(b.fnext, b.head_array[(b.head_index - 1 + b.size)%b.size]);
				insert_character(&b, line_no, col_no, ch);
				if(line_no != b.size -1)
					line_no++;
				col_no = 0;
				change = PAGE_CHANGE;
				break;
			case KEY_BACKSPACE:
				if(b.mode == READ_ONLY)
					break;
				if(col_no){
		            deleted_char =  backspace(&b, line_no, col_no);
		            col_no--;
		            change = LINE_CHANGE;

					//for undo
					cur_position.line_no = line_no;
					cur_position.col_no = col_no;
					push(&undo, OPERATION_DELETE, cur_position, deleted_char);
				}
		        else{
		        	if(line_no){
		        	int temp = b.head_array[(b.head_index + line_no - 1)%b.size].line_size;
		            backspace(&b, line_no, col_no);
		            col_no = temp;
		            line_no--;
		            }
		            else{
						load_prev_line(&b);
		            	int temp = b.head_array[b.head_index].line_size;
				        backspace(&b, line_no, col_no);
				        col_no = temp;
		            }
		            change = PAGE_CHANGE;
		        }
			break;
			default:
				if(b.mode == READ_ONLY)
					break;
				if(ch <= 256){
					insert_character(&b, line_no, col_no, ch);
					col_no++;
					change = LINE_CHANGE;

					//for undo
					cur_position.line_no = line_no;
					cur_position.col_no = col_no;
					push(&undo, OPERATION_INSERT, cur_position, 'h');
				}
		        break;
		}
		if(col_no > 141)
			b.line_offset = col_no - 141;
		else{
			b.line_offset = 0;
			change = PAGE_CHANGE;
		}
		if(b.line_offset)
			change = PAGE_CHANGE;
			
        if(change == PAGE_CHANGE)
        	print_page_ncurses(b);
        else if(change == LINE_CHANGE)
        	print_line_ncurses(b.head_array[(line_no + b.head_index) % b.size], line_no, b.filetype, b.line_offset);	
        print_loc(line_no, col_no);
        
        if(b.line_offset)
        	move(line_no, 141);
        else
        	move(line_no, col_no);
    }
    return 0;
}
