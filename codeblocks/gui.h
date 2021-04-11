#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED
#include"function.h"

void print_loc(int x, int y);
void print_line_ncurses(line l, int line_num, int type, int line_offset);
void print_page_ncurses(buffer b);

#endif // GUI_H_INCLUDED
