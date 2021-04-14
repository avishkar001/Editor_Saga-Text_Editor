#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "c_stack.h"

void init(c_stack *c, int size) {
    c->size = size;
    c->front = c->rear = -1;
    c->q = (stack_node**)malloc(sizeof(stack_node*)*size);
}

int isEmpty(c_stack c) {
    if(c.front == -1)
        return 1;
    else
        return 0;
}
void push_string(c_stack *c,int operation_id ,position final_position, char* string, int string_len){
    for (int i = 0; i < string_len; i++){

        push(c,operation_id ,final_position, string[i]);
    }
    
}


void push(c_stack *c,int operation_id ,position final_position, char data){

	if(c->rear != -1 && operation_id == c->q[c->rear]->operation_id){
		
		if(operation_id == OPERATION_NAVIGATE)
			return;
			
		else{
            //if(operation_id != OPERATION_BACKSPACE_OLDLINE)
			c->q[c->rear]->final_position = final_position;
			c->q[c->rear]->len_of_arr += 1;
			c->q[c->rear]->array[c->q[c->rear]->len_of_arr] = data;
		}
		return;
	}
	else{
		stack_node* newnode = (stack_node*)malloc(sizeof(stack_node));
		newnode->operation_id = operation_id;
		newnode->len_of_arr = 0;
		newnode->array[newnode->len_of_arr] = data;
        newnode->initial_position = final_position;
		newnode->final_position = final_position;
	
    c->rear = ((c->rear) + 1) % c->size;
    c->q[c->rear] = newnode;
    if(c->front == c->rear)
        c->front = (c->front + 1) % c->size;
	}
	
	if(c->front == -1)
        (c->front)++;               //for 1st element change front from -1 0
    return;

}

stack_node* pop(c_stack *c){
        stack_node* item;
        if(isEmpty(*c)) {
            return NULL;
        }
        item = c->q[c->rear];
        if(c->front == c->rear) {            //if you remove the only element present, set front and rear to -1 to show empty list
                c->front=-1;
                c->rear=-1;
        }
        else
            c->rear = (c->rear - 1  + c->size) % c->size;

        return item;
}
