#include "data_structure.h"
#include<stdlib.h>
#include<stdio.h>
// Function that is used to move the gap
// left in the array
void left(lines_node* node, int position){
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
        node->arr[node->gap_right+1] = node->arr[node->gap_left];
    }
}

void right(lines_node* node, int position){
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
        node->arr[node->gap_left-1] = node->arr[node->gap_right];
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
