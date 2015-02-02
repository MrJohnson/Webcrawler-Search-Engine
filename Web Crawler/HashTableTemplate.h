
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  // Add implementation here
  _buckets = new HashTableTemplateEntry<Data>*[TableSize];
  int i = 0;

  for(i = 0; i < TableSize; i++){
    //create sentinel nodes in each bucket
    _buckets[i] = new HashTableTemplateEntry<Data>();
    _buckets[i]->_next = NULL;
    _buckets[i]->_key = NULL;
    _buckets[i]->_data = 0;
  }
     
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  /// Add implementation here
  int hash_code = hash(key);                //obtain hash code of "key"
  HashTableTemplateEntry<Data>* current;
  current = _buckets[hash_code]->_next;         //set current to first element in "key's" corresponding bucket 
  int i = 0;

  while(current != NULL){
    if(strcmp(current->_key, key) == 0){
      //key already exists; substitute data
      current->_data = data;

      return true;
    }
    current = current->_next;   //key did not match; iterate to next entry 
  }
  
  //no match was found; insert new item at beginning of list
  HashTableTemplateEntry<Data>* newEntry = new HashTableTemplateEntry<Data>;
  newEntry->_key = strdup(key);
  newEntry->_data = data;
  newEntry->_next = _buckets[hash_code]->_next;
  _buckets[hash_code]->_next = newEntry;

  return false; //no match was found;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
   //Add implementation here
  int hash_code = hash(key);          //obtain hash code for "key"
  HashTableTemplateEntry<Data>* current = _buckets[hash_code]->_next;

  while(current != NULL){
    if(strcmp(current->_key, key) == 0){
      //key found
      *data = current->_data;
      return true;
    }
    current = current->_next;
  }

  return false;       //key was not found
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  Data d;

  if(find(key, &d))
    return d;

  return NULL;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
  int hash_code = hash(key);

  if(_buckets[hash_code]->_next == NULL){
    //bucket corresponding to key's hash code is empty
    return false;
  }

  HashTableTemplateEntry<Data>* previous = _buckets[hash_code];
  HashTableTemplateEntry<Data>* current = _buckets[hash_code]->_next;
 
  while(current != NULL){
    if(strcmp(current->_key, key) == 0){
      //key found; remove element
      previous->_next = current->_next;
      delete current;
      return true;
    }
    //key not found yet, advance to next element
    previous = current;
    current = current->_next;
  }

  return false;   //key does not exist
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
  _currentBucket = 0;
  _hashTable = hashTable;
  _currentEntry = _hashTable->_buckets[0];
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  // Add implementation here
  //find next non-empty bucket
  while(_currentEntry->_next == NULL && (_currentBucket < _hashTable->TableSize -1)){
    _currentBucket++;
    _currentEntry = _hashTable->_buckets[_currentBucket];
  }

  if(_currentBucket == _hashTable->TableSize -1 && _currentEntry->_next == NULL){
    //Reached end; no more entries
    return false;
  }else{
    //found a non-empty bucket
    key = _currentEntry->_next->_key;
    data = _currentEntry->_next->_data;
    _currentEntry = _currentEntry->_next;
    return true;
  }
}
