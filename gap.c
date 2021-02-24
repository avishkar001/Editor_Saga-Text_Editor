#define NODES_SIZE 50

typedef struct lines_node{
    char* arr;
    int gap_left,gap_right, gap_size;
    struct lines_node *next;
}lines_node;

// Function that is used to move the gap  
// left in the array  
void left(lines_node* node, int position)  
{  
    if(node->gap_size == 0)
        return;
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
    if(node->gap_size == 0)
        return;  
    // Move the gap right character by character  
    // and the buffers  
    while (position > node->gap_left) {  
        node->gap_left++;  
        node->gap_right++;  
        buffer[node->gap_left-1] = buffer[node->gap_right];  
    }  
}

void move_cursor(lines_node* node, int position){

    if(position > (NODES_SIZE - node->gap_size)){
        position -= NODES_SIZE - node->gap_size;
        node = node->next;
    }
}