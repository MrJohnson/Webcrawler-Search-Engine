
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"
#define START_MAX 10000


/*
 * @ author Taylor T. Johnson
 * CS251 - Summer 2014
 */


// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
  maxNumber = START_MAX;
  array = (ArrayDictionaryNode*)malloc(sizeof(ArrayDictionaryNode)* maxNumber);
  currentNumber = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
	// Check if key already exists in array
	int i = 0;
    for(i = 0; i < currentNumber; i++){
    	if(strcmp(key, array[i].key) == 0){
    		//found matched key; update data
    		array[i].data = record;
    		return false;
    	}
    }

	// Key does not exist in array; add to array
	array[currentNumber].key = strdup(key);
	array[currentNumber].data = record;
	currentNumber++;


    //check if array needs to be resized
	if(currentNumber == maxNumber){
		// Array full; Resize array
		maxNumber *= 2;			//double the maxNumber of array entries
		// reallocate array to be double the current size
		array = (ArrayDictionaryNode*)realloc(array, sizeof(ArrayDictionaryNode)* maxNumber);
	}

	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
    int i = 0;
    for(i = 0; i < currentNumber; i++){
    	if(strcmp(key, array[i].key) == 0){
    		//found matched key
    		return array[i].data;
    	}
    }

	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
	int i = 0; 
	while( i < currentNumber){
		if(strcmp(key, array[i].key) == 0){
			// Found key in table; remove key
			while(i < currentNumber - 1){
				// Copy elements from next node
				array[i].key = array[i+1].key;
				array[i].data = array[i+1].data;
				i++;
			}
			currentNumber--;		// Reduce the current number by 1
			return true;
		}
		i++;
	}
	// Key was not in array
	return false;

}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
	if(currentNumber == 0){
		//array is empty
		return NULL;
	}
    int i = 0;
    *n = currentNumber;
    KeyType * all_keys = (KeyType *)malloc(sizeof(char*) * (currentNumber));
    
    for(i = 0; i < currentNumber; i++){
    	all_keys[i] = array[i].key;
    }

	return all_keys;
}
