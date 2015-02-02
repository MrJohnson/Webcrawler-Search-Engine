
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{

	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	// Add your implementation here

	//ensure that key is not in tree already
	AVLNode* current = root;
	AVLNode* previous = NULL;
	while(current != NULL){
		previous = current;
		int result = strcmp(key, current->key);
		if(result == 0){
			//FOUND MATCH; update data
			//printf("FOUND MATCH!\n current record: %d\n", current->data);
			current->data = record;
			//printf("UPDATATED RECORD: %d\n", current->data);
			assert(current->data == record);
			return false;
		}else if( result < 0){
			// key should be left of current node
			current = current->left;
		}else{
			// key should be right of current node
			current = current->right;
		}
	}
	// did not find key in tree
	// previous points to the node parent at which we must insert the new node
	AVLNode* n = new AVLNode;
	n->key = strdup(key);
	n->data = record;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	n->parent = previous;
	if(previous == NULL){
		// tree is empty. Assign new node to root
		//printf("FIRST: key: %s, data: %d\n", n->key, n->data);
		root = n;
		return true;
	}else if(strcmp(key, previous->key) < 0){
		// attach to previous's left
		//printf("LEFT: key: %s, data: %d\n", n->key, n->data);
		nElements++;
		previous->left = n;
	}else{
		// attach to previous's right
		//printf("RIGHT: key: %s, data: %d\n", n->key, n->data);
		nElements++;
		previous->right = n;
	}

	// adjust height of ancestor nodes of new node
	AVLNode* m = n->parent;
	while(m != NULL){
		//Get max height of left and right children
		int max_height = 0;
		if(m->left != NULL){
			max_height = m->left->height;
		}
		if(m->right != NULL && m->right->height > max_height){
			max_height = m->right->height;
		}
		// update height of m
		m->height = max_height + 1;
		m = m->parent;
	}

	restructure(n);

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
		
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	//Go upwards until root is found

        // See class notes
	AVLNode* z = n->parent;
	while( z != NULL){
		//while we haven't gone past root
		int hleft = 0;
		if(z->left != NULL){
			hleft = z->left->height;
		}
		int hright = 0;
		if(z->right != NULL){
			hright = z->right->height;
		}
		int hdiff = hleft - hright;
		if(hdiff < 0){
			hdiff = -hdiff;
		}
		// Recompute height of z just in case it was modified with restructure
		if(hleft > hright){
			z->height = 1 + hleft;
		} else {
			z->height = 1 + hright;
		}
		// check if z is parent of unbalanced subtree
		if(hdiff <= 1){
			// z is parent of balanced subtree; skip it by going up in tree
			z = z->parent;
			continue;		//continue going up tree
		}
		// At this point, z is parent of unbalanced subtree
		// find y: the child of z with the largest height

		AVLNode* y = NULL;
		int maxh = 0;
		if(z->left != NULL){
			y = z->left;
			maxh = y->height;
		}
		if(z->right != NULL && maxh < z->right->height){
			y = z->right;
			maxh = y->height;
		}
		assert(y != NULL); 	// this should always be true
		// find x: the child of y with the largest height
		AVLNode* x = NULL;
		maxh = 0;
		if(y->left != NULL){
			x = y->left;
			maxh = x->height;
		}
		if(y->right != NULL && maxh < y->right->height){
			x = y->right;
			maxh = x->height;
		}
		assert(x != NULL);		//this should always be true

		AVLNode *t0, *t1, *t2, *t3;
		AVLNode *a, *b, *c;

		// case 1
		if( y == z->right){
			if(x == y->right){
				//case 1
				a = z;
				b = y;
				c = x;
				t0 = a->left;
				t1 = b->left;
				t2 = c->left;
				t3 = c->right;
			}else{
				//case 2
				a = z;
				b = x;
				c = y;
				t0 = a->left;
				t1 = b->left;
				t2 = b->right;
				t3 = c->right;
			}
		}else{
			if(x == y->left){
				//case 3
				a = x;
				b = y;
				c = z;
				t0 = a->left;
				t1 = a->right;
				t2 = b->right;
				t3 = c->right;
			}else{
				//case 4
				a = y;
				b = x;
				c = z;
				t0 = a->left;
				t1 = b->left;
				t2 = b->right;
				t3 = c->right;
			}
		}
		//get the value of the parent
		AVLNode* p = z->parent;
		//Check if z is the root
		if(p == NULL){
			root = b;
		}else{
			// z was not at the root. Attach b either on the left or right of p
			// depending on where z was attached
			if(p->left == z){
				// z was attached on the left
				p->left = b;
			} else {
				// z was attached on the right
				p->right = b;
			}
		}
		//make the a, b, c subtree
		b->parent = p;
		b->left = a;
		b->right = c;
		a->parent = b;
		a->left = t0;
		a->right = t1;
		c->parent = b;
		c->left = t2;
		c->right = t3;
		// connect t0, t1, t2, t3 to their respective parents
		if(t0 != NULL){
			t0->parent = a;
		}
		if(t1 != NULL){
			t1->parent = a;
		}
		if(t2 != NULL){
			t2->parent = c;
		}
		if(t3 != NULL){
			t3->parent = c;
		}
		//now fix the heights of a, b, c
		//fix height of a
		maxh = 0;
		if(a->left != NULL){
			maxh = a->left->height;
		}
		if(a->right != NULL && a->right->height > maxh){
			maxh = a->right->height;
		}
		a->height = 1 + maxh;
		//fix height of b
		maxh = 0;
		if(b->left != NULL){
			maxh = b->left->height;
		}
		if(b->right != NULL && b->right->height > maxh){
			maxh = b->right->height;
		}
		b->height = 1 + maxh;
		//fix height of a
		maxh = 0;
		if(c->left != NULL){
			maxh = c->left->height;
		}
		if(c->right != NULL && c->right->height > maxh){
			maxh = c->right->height;
		}
		c->height = 1 + maxh;
		z  = p;
	}
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here
	if(nElements == 0){
		// tree is empty
		return NULL;
	}
	AVLNode* current = root;
	while(current != NULL){
		int result = strcmp(key, current->key);
		if(result == 0){
			//FOUND MATCH
			//printf("FOUND MATCH FOR KEY: %s\nRETURNING DATA: %d\n",current->key, current->data);
			return current->data;
		}else if( result < 0){
			// key should be left of current node
			current = current->left;
		}else{
			// key should be right of current node
			current = current->right;
		}
	}
	return NULL;
}



// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

