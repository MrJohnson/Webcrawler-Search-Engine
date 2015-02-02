#include "array-dictionary.h"

class Heap {
    // Assume that keys are int and no elements are stored with key.
    // This data structure can be modified to support elements.

    int maxSize; // Max size of the array.
    ArrayDictionaryNode* heap;
    int last; // One after last element.

    public:
    Heap(int maxSize);
    
    inline int left(int p) {
      return 2*p+1;
    }

    inline int right(int p) {
      return 2*p+2;
    }

    inline int theParent(int c) {
      return (c-1)/2;
    }

    void print_heap();

    void insert(ArrayDictionaryNode* key);

    ArrayDictionaryNode removeMin();

    ~Heap();
};
