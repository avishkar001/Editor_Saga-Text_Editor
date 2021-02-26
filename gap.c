#include<string.h>
#define NODES_SIZE 50

typedef struct lines_node{
    char* arr;
    int gap_left,gap_right, gap_size;
    struct lines_node *next;
}lines_node;

typedef lines_node* line;

void init_lines_node(lines_node* l){
    l->arr = (char*)malloc(sizeof(char)*NODES_SIZE);
    l->next = NULL;
    l->gap_size = NODES_SIZE;
    l->gap_left = 0;
    l->gap_right = NODES_SIZE - 1;
    return;
}

// Function that is used to move the gap  
// left in the array  
void left(lines_node* node, int position)  
{  
    if(node->gap_size == 0){
		node->gap_left = position;
		node->gap_right = position - 1;
        return;
	}
        
    // Move the gap left character by character  
    // and the buffers  
    while (position < node->gap_left) {  
        node->gap_left--;  
        node->gap_right--;  
        buffer[node->gap_right+1] = buffer[node->gap_left];
    }  
}

void right(lines_node* node, int position)  
{
    if(node->gap_size == 0) {
        /*if this is last node of line and position is greater than size of node*/
		if(position >= NODES_SIZE) {            
			node->gap_left = NODES_SIZE;
			node->gap_right = NODES_SIZE - 1;
			return;
		}
		node->gap_left = position;
		node->gap_right = position - 1;
		return;
	}  
    // Move the gap right character by character  
    // and the buffers  
    while (position > node->gap_left) {  
        node->gap_left++;  
        node->gap_right++;  
        buffer[node->gap_left-1] = buffer[node->gap_right];  
    }  
}

lines_node* move_cursor(lines_node* node, int position){
    //find node to which position belongs or find last node
    while(position > (NODES_SIZE - node->gap_size)){       
        position -= NODES_SIZE - node->gap_size;
        if(node->next == NULL)
            break;
        node = node->next;
    }

    if(position < node->gap_left)
        left(node, position);
    if(position > node->gap_left)
        left(node, position);
    return node;
}

void insert_character(line l, int position, char data){

    lines_node* node = move_cursor(l, position);

    if(node->gap_size == 0){
        lines_node* newnode = (lines_node*)malloc(sizeof(lines_node*));
        if(newnode == NULL)
            return;
        init_lines_node(newnode);
        newnode->next = node->next;
        node->next = newnode;

        int copydata_len = NODES_SIZE - node->gap_left;

        strncpy(newnode->arr, node->arr[node->gap_left], copydata_len);
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
    return;
}

void delete_character(line l, int position, char data){

    lines_node* node = move_cursor(l, position);

    node->gap_left--;
    node->gap_size++;
    return;
}