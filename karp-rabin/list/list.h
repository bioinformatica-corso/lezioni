#ifndef LIST_H

	#define LIST_H

	#include <stdlib.h>
	#include <stdio.h>
	

	// List Declaration	
	// First Element Holds The Elements Number
	struct list {
	
		int* list_arr; // Array Used To Store Elements
   
	};
	
	
	// Initialise The List
	void init_list ();
	
	
	// Add An Element To The List
	// Return The Element Position In The List
	int add_element (int element);
	
	
	// Return The Position Of The Passed Element In The List
	// If Not Present, Returns -1
	int get_element_pos (int element);
	
	
	// Returns The Array Associated To The List
	int* get_list_arr ();
	
	
	// Prints The List
	void print_list ();
	
	
#endif
