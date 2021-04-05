#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "gui.h"

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
                if(col_no < b.head_array[(line_no + b.head_index) % b.size].line_size)
                    col_no++;
                break;

            case KEY_DOWN:
                if(line_no < b.size - 1)
                    line_no++;
                else
                    load_next_line(&b);

                if(col_no > b.head_array[(line_no + b.head_index) % b.size].line_size)
                    col_no = b.head_array[(line_no + b.head_index) % b.size].line_size;
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
		insert_character(&b, line_no, col_no, ch);
		col_no++;
		break;
		case '\n':
			write_line(b.fnext, b.head_array[(b.head_index - 1 + b.size)%b.size]);
			insert_character(&b, line_no, col_no, ch);
			line_no++;
			col_no = 0;
			break;
	    case KEY_BACKSPACE:
	    	if(col_no){
                backspace(&b, line_no, col_no);
                col_no--;
		    }
            else{
            	int temp = b.head_array[(b.head_index + line_no - 1)%b.size].line_size;
                backspace(&b, line_no, col_no);
                col_no = temp;
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
