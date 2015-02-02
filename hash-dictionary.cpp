
// Implementation of a dictionary using a hash table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
  int i;
  for(i = 0; i < MaxBuckets; i++){
  	buckets[i] = NULL;
  }	
}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += (i + 1)*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  
  int hash_code = hash(key);
  HashNode* current = buckets[hash_code];

  if(buckets[hash_code] == NULL){
  	//current bucket is empty
  	current = new HashNode;
  	current->key = strdup(key);
  	current->data = record;
  	current->next = NULL;
  	buckets[hash_code] = current;
  	nElements++;
  }else{
  	//current bucket is not empty
  	// make sure key is not in table
  	while( current != NULL ){
		if(!strcmp(key, current->key)){
			// found match
			current->data = record;
			return false;
		}else{
			// did not match; move to next element in list
			current = current->next;
		}
	}
  	current = buckets[hash_code];
  	while(current->next != NULL){
  		current = current->next;
  	}
  	HashNode* n = new HashNode;
  	n->key = strdup(key);
  	n->data = record;
  	n->next = NULL;
  	current->next = n;
  	nElements++;
  }
  return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	int hash_code = hash(key);
	HashNode* current = buckets[hash_code];

	while( current != NULL){
		if(strcmp(key, current->key) == 0){
			// found match
			return current->data;
		}else{
			// did not match; move to next element in list
			current = current->next;
		}
	}
	//did not find key in hash table
	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  // Add your code here
  	int hash_code = hash(key);
  	HashNode* n = buckets[hash_code];
  	HashNode* prev = NULL;
  	
  	while(n != NULL){
  		if (!strcmp(n->key, key)){
  			// found key in table
  			if( prev != NULL){
  				prev->next = n->next;
  			} else {
  				buckets[hash_code] = n->next;
  			}
  			nElements--;
  			delete n;
  			return true;
  		}
  		prev = n;
  		n = n->next;
  	}	
	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}

