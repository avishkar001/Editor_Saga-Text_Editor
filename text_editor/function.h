#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include"gap_buffer.h"

void insert_character(line *l, int position, char data);
void backspace(buffer *b, int line_no, int position);
void load_next_line(buffer *b);
void load_prev_line(buffer *b);

#endif // FUNCTION_H_INCLUDED
