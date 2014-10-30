#include "list.h"

// List Instantation Declaration
struct list list_inst;


// Initialise The List
void init_list () {
	
	list_inst.list_arr = malloc (sizeof(int));
	list_inst.list_arr[0] == 0;
	
}	


// Add An Element To The List
// Return The Element Position In The List
int add_element (int element) {
	
	int dim = list_inst.list_arr[0];
	
	list_inst.list_arr = realloc (list_inst.list_arr, (dim+1)*sizeof(int));
	
	list_inst.list_arr[0] = dim + 1;
	
	list_inst.list_arr[dim+1] = element;
	
	return dim;
	
}


// Remove All The Occurence  Of Element From The List
int remove_element (int element) {
	
	int i = 0;
	
	int j = 1;
	
	int occurrences = 0;
	
	int dim = list_inst.list_arr[0];
	
	int* arr_tmp = malloc ((dim+1)*sizeof(int));
	
	arr_tmp[0] = list_inst.list_arr[0];
	
	for (i = 1; i < dim+1; i++) {		
		
		if (list_inst.list_arr[i] != element) {
		
			arr_tmp[j] = list_inst.list_arr[i];
			
			j++;
			
		} else {
			
			occurrences++;
		
		}
		
	}
	
	if (occurrences > 0) {
	
		arr_tmp = realloc (arr_tmp, (dim - occurrences + 1)*sizeof(int));
		arr_tmp[0] = dim - occurrences;
		free(list_inst.list_arr);
		list_inst.list_arr = arr_tmp;
		
	 }		
	
	return occurrences;
	
}


// Return The Position Of The Passed Element In The List
// If Not Present, Returns -1
int get_element_pos (int element) {
	
	int i = 1;
	
	for (i = 1; i < list_inst.list_arr[0]+1; i++) {
		
		if (list_inst.list_arr[i] == element) {
			
			return i;
			
		}
		
	}
	
	return -1;
	
}


// Returns The Array Associated To The List
int* get_list_arr () {
	
	return list_inst.list_arr;	
	
}


// Prints The List
void print_list () {
	
	int i = 0;
	
	for (i = 0; i < (list_inst.list_arr[0]+1); i++) {
		
		if (i == 0) {
			
			printf ("List Element Number = %i\n", list_inst.list_arr[0]);
			
		} else {
		
			printf ("List[%i] = %i\n", i, list_inst.list_arr[i]);
			
		}
		
	}	
	
}



void main () {
	
	printf("\n\n");
   
   init_list();
   
   add_element(1);
   add_element(2);
   add_element(3);
   add_element(4);
   
   print_list();
   
   printf ("\n2 is at post %i\n", get_element_pos(2));
   
   remove_element(1);
   
   print_list();   
   
}
