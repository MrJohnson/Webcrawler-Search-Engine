#include <assert.h>
#include "heap.h"
#include "stdio.h"
#include <string.h>


/*
 * @ author Taylor T. Johnson
 * CS251 - Summer 2014
 */


Heap::Heap(int maxSize) {
  this->maxSize = maxSize;
  heap = new ArrayDictionaryNode[maxSize];
  last = 0;
}

void 
Heap::insert(ArrayDictionaryNode* n) {
  // Make sure there is space in the heap.
  assert(last < maxSize);
  
  heap[last].key = n->key;
  heap[last].data = n->data;
  last++;
  
  // Apply upheap.
  int child = last-1;
  int parent = theParent(child);
  while(child > 0) {  // while we don't reach the root
    if(strcmp(heap[child].key, heap[parent].key) > 0) {
      // No swap needed
      break;
    }
    // Swap is needed. Swap parent and child.

    char* tmp_key = heap[child].key;
    void* tmp_data = heap[child].data;

    heap[child].key = heap[parent].key;
    heap[child].data = heap[parent].data;

    heap[parent].key = tmp_key;
    heap[parent].data = tmp_data;
    
    child = parent;
    parent = theParent(child);
  }
}

void Heap::print_heap(){
  printf("\nHEAP TEST:\n");
  int i = 0;
  for(i = 0; i < 4; i++){
    printf("Name: %s\tPlace of Residence: %s\n", (char*)heap[i].key,(char*)heap[i].data);
  }
}

ArrayDictionaryNode Heap::removeMin() {
  
  // Make sure there are elements in the heap.
  assert(last > 0);
  
  // Get minimum key from the root of the heap.
  ArrayDictionaryNode minKey = heap[0];
  
  // Replace root with (last-1) element.
  heap[0] = heap[last-1];
  last--;
  
  // Apply downheap.
  int parent = 0;
  int ileft = left(parent);
  int iright = right(parent);
  
  // Downheap until we reach the bottom or swap not needed.
  while(ileft < last) {
    // Get minimum child.
    int minChild = ileft;
    
    // If right child exists and is less than the left,
    // make minChild the right child.
    
    if(iright < last && strcmp(heap[iright].key , heap[ileft].key) < 0) {
      minChild = iright;
    }
    
    // Check if swap is needed.
    if(strcmp(heap[parent].key, heap[minChild].key) < 0) {
      // No swap is needed.
      break;
    }
    // Swap is needed.

    char* tmp_key = heap[minChild].key;
    void* tmp_data = heap[minChild].data;

    heap[minChild].key = heap[parent].key;
    heap[minChild].data = heap[parent].data;
    heap[parent].key = tmp_key;
    heap[parent].data = tmp_data;
   
    
    // Update parent, ileft, and iright.
    parent = minChild;
    ileft = left(parent);
    iright = right(parent);
  }
  return minKey;
}

Heap::~Heap() {
  delete []heap;
}

