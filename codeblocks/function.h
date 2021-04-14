#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include"gap_buffer.h"

void save_file(buffer *b);
char last_character(buffer *b, int line_no);
int is_comment(buffer *b, int line_no);
void insert_string(buffer *b, int line_no, int position, char* data, int datalen);
void insert_character(buffer *b, int line_no, int position, char data);
char backspace(buffer *b, int line_no, int position);
int load_next_line(buffer *b);
int load_prev_line(buffer *b);
void read_file_firsttime(buffer* b);

#endif // FUNCTION_H_INCLUDED
