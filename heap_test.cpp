#include <stdio.h>
#include "heap.h"
#define NUM_ELEMENTS 4

int main(){

	ArrayDictionaryNode nodes[] = {{(char*)"Andrew", (void *)"West Lafayete"},{(char*)"Zach", (void*)"Indianapolis"},
									{(char*)"Mary", (void*)"Chicago"}, {(char*)"Sally", (void*)"New York"}};
	ArrayDictionaryNode* ptr = nodes; 

	int i;
	printf("TEST CREATION OF NODES:\n");
	for(i = 0; i < NUM_ELEMENTS; i++){
		printf("Name: %s\tPlace of Residence: %s\n", (char*)nodes[i].key,(char*)nodes[i].data);
	}

	Heap h(NUM_ELEMENTS);

	for(i = 0; i < NUM_ELEMENTS; i++){
		h.insert(ptr+i);
	}

	h.print_heap();

	for(i = 0; i < NUM_ELEMENTS; i++){
		
		ArrayDictionaryNode tmp = h.removeMin();
		
		ptr[i].key = tmp.key;
		ptr[i].data = tmp.data;
	}

	printf("\nSORTED HEAP:\n");
	for(i = 0; i < NUM_ELEMENTS; i++){
		printf("Name: %s\tPlace of Residence: %s\n", (char*) ptr[i].key,(char*)ptr[i].data);
	}


/*
	int i = 0;
	for(i = 0; i < NUM_ELEMENTS; i++){
		h->insert(test[i]);
	}

	for(i = 0; i < NUM_ELEMENTS; i++){
		test[i] = h->removeMin();
	}

	for(i = 0; i < NUM_ELEMENTS; i++){
		printf("%d, ", test[i]);
	}
	printf("\n");

	h->print_heap();

	printf("\n");
*/
}
