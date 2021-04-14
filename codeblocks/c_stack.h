#ifndef stack_H_INCLUDED
#define stack_H_INCLUDED

#define OPERATION_NAVIGATE 1
#define OPERATION_INSERT 2
#define OPERATION_DELETE 3
#define OPERATION_NEXT_LINE 4
#define OPERATION_PREV_LINE 5
#define OPERATION_NEXT_PAGE 6
#define OPERATION_PREV_PAGE 7
#define OPERATION_NEWLINE 8
#define OPERATION_BACKSPACE_OLDLINE 9

typedef struct position{
	int line_no;
	int col_no;
}position;

typedef struct stack_node{
    int operation_id;
    char array[100];
 	int len_of_arr;
 	position initial_position, final_position;
}stack_node;

typedef struct c_stack{
    int rear;
    int front;
    int size;
    stack_node** q;

}c_stack;

void init(c_stack *c, int size);
int isEmpty(c_stack c);
void push(c_stack *c,int operation_id,position final_position, char data);
stack_node* pop(c_stack *c);
void push_string(c_stack *c,int operation_id ,position final_position, char* string, int string_len);

#endif // stack_H_INCLUDED
