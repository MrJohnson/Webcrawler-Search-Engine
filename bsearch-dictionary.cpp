
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"
#define MAX_URLS 1000

/*
 * @ author Taylor T. Johnson
 * CS251 - Summer 2014
 */

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	currentNumber = 0;
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}
	printf("Key: %s\n", key);
	
	// Use binary search

	int low = 0;
	int high = currentNumber - 1;

	while(high >= low){
		int mid = (low + high)/2;
		int result = strcmp(key, array[mid].key);
		if(result == 0){
			//found a match
			return array[mid].data;
		} else if(result < 0){
			//key was lower then key at mid
			high = mid -1;
		} else{
			//key was higher than mid
			low = mid + 1;
		}
	}
	//no match was found

	return NULL;
}

int cmpfunc( const void * a, const void * b){
	ArrayDictionaryNode* aa = (ArrayDictionaryNode*) a;
	ArrayDictionaryNode* bb = (ArrayDictionaryNode*) b;
	return strcmp( aa->key, bb->key);
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{

	if(currentNumber == 0){
		return;
	}
	int i;
	
	//Heap* h = new Heap(currentNumber);

	Heap h(currentNumber);

	for(i = 0; i < currentNumber; i++){
		h.insert(array+i);
	}

	for(i = 0; i < currentNumber; i++){
		ArrayDictionaryNode tmp = h.removeMin();
		
		array[i].key = tmp.key;
		array[i].data = tmp.data;
	}
	//qsort(array, currentNumber, sizeof(ArrayDictionaryNode), cmpfunc);
}




