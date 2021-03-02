#include<ncurses.h>
#include"gui.h"
void print_loc(int x, int y){
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
