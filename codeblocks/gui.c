#include<ncurses.h>
#include<stdlib.h>
#include<string.h>
#include"gui.h"
#include"trie.h"
#include"data_structure.h"
void print_loc(int x, int y){
        move(37, 0);
		clrtoeol();
        mvprintw(37, 0, "x: %d y: %d", x, y);
}

void print_line_ncurses(line l, int line_num, int type){

	if(l.head == NULL){
		start_color();
		init_color(COLOR_BLACK, 0, 0, 0);
		init_pair(3, COLOR_CYAN, COLOR_BLACK);
		attron(COLOR_PAIR(3));
    	mvaddch(line_num, 0, '~');
    	attroff(COLOR_PAIR(3));
		return;
	}
	if(type == C_FILE){
		lines_node *p = l.head;
		int col_num = 0;

		move(line_num, 0);
		clrtoeol();
		
		int j = 0;
		int comment_flag = 0, color;
		char highlight[1000], ch_prev = ' ';
		struct TrieNode *root = getNode();
		init_trie(root);
		start_color();
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_YELLOW, 900, 950, 0);
		init_color(COLOR_BLUE, 257, 386, 600);
		init_color(COLOR_BLACK, 50, 50, 50);
		
		init_color(COLOR_RED, 1000, 0, 250);
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_CYAN, COLOR_BLACK);
		init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(5, COLOR_GREEN, COLOR_BLACK);
		
		while(p){
		    for(int i=0; i<NODES_SIZE; i++){
		        if(i==p->gap_left && p->gap_size != 0){
		            i=p->gap_right;
		            continue;
		        }
		        /*
		        start_color();
		        init_pair(1, COLOR_RED, COLOR_BLACK);
		        attron(COLOR_PAIR(1));
		        mvaddch(line_num, col_num++, p->arr[i]);
		        attroff(COLOR_PAIR(1));
		        
		       
		        */
		        if(comment_flag){
		        	attron(COLOR_PAIR(3));
		        	mvaddch(line_num, col_num++, p->arr[i]);
		        	attroff(COLOR_PAIR(3));
		        	continue;
		        }
		        
		        
				if(p->arr[i] == '/' && ch_prev == '/'){
		       		comment_flag = 1;
		        	highlight[j] = '\0';
		       		for(int k=0; k<strlen(highlight); k++)
		       			mvaddch(line_num, col_num++, highlight[k]);
		        	j=0;
		        	attron(COLOR_PAIR(3));
		        	mvaddch(line_num, col_num - 1, '/');
		       		mvaddch(line_num, col_num++, '/');
		       		attroff(COLOR_PAIR(3));
		       		continue;
		        	
		        }
		         
		        if(p->arr[i] == ' ' || p->arr[i] == '('|| p->arr[i] == ';'){
		        	highlight[j] = '\0';
		        	
		        	if(search(root, highlight)){
		        		
		        		if(highlight[0] == 'i' || highlight[0] == 'c'){
							attron(COLOR_PAIR(1));
							color = 1;	
						}
						else if(highlight[0] == 'f' || highlight[0] == 'v'){
							attron(COLOR_PAIR(2));
							color = 2;	
						}
						else if(highlight[0] == 'w' || highlight[0] == 's'){
							attron(COLOR_PAIR(4));
							color = 4;	
						}
						else {
							attron(COLOR_PAIR(5));
							color = 5;	
						}
						
		        		for(int k=0; k< strlen(highlight); k++){
							mvaddch(line_num, col_num++, highlight[k]);
						}
						attroff(COLOR_PAIR(color));
						
		        	}
		        	else{
		        	
		        		for(int k=0; k<strlen(highlight); k++)
		        			mvaddch(line_num, col_num++, highlight[k]);
		        	}
		        	j = 0;
		        	
		        	
		        	mvaddch(line_num, col_num++, p->arr[i]);
		        	
		        }
		        else	
		        	highlight[j++] = p->arr[i];
		        if(j == 999)
		        	j=0;
		        	
		        
		        ch_prev = p->arr[i];
		    }
		    p = p->next;
		}
		if(j){
			highlight[j] = '\0';
		        	
		    if(search(root, highlight)){
		    		int color;
		        	if(highlight[0] == 'i' || highlight[0] == 'c'){
						attron(COLOR_PAIR(1));
						color = 1;	
					}
					else if(highlight[0] == 'f' || highlight[0] == 'w'){
						attron(COLOR_PAIR(2));
						color = 2;	
					}
					else if(highlight[0] == 'v' || highlight[0] == 's'){
						attron(COLOR_PAIR(2));
						color = 2;	
					}
					else {
						attron(COLOR_PAIR(3));
						color = 3;	
					}
		    
		        	for(int k=0; k< strlen(highlight); k++){
						mvaddch(line_num, col_num++, highlight[k]);
					}
					attroff(COLOR_PAIR(color));
						
			}
			else{
		        	
		        for(int k=0; k<strlen(highlight); k++)
		        	mvaddch(line_num, col_num++, highlight[k]);
		  	}
		        	
		
		}
	}	
	else{
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
		
	}
	
    return;
}

void print_page_ncurses(buffer b){

    for(int i=0; i < b.size; i++){
        print_line_ncurses(b.head_array[(b.head_index + i) % b.size], i, b.filetype);
    }
}
