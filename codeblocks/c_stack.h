#ifndef stack_H_INCLUDED
#define stack_H_INCLUDED

#define OPERATION_NAVIGATE 1
#define OPERATION_INSERT 2
#define OPERATION_DELETE 3
#define OPERATION_NEXT_LINE 4
#define OPERATION_PREV_LINE 5

typedef struct position{
	int line_no;
	int col_no;
}position;

typedef struct stack_node{
    int operation_id;
    char array[100];
 	int len_of_arr;
 	position final_position;
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

#endif // stack_H_INCLUDED
