/***************
#ifndef NODE_H_
#define NODE_H_

struct Node {
	// Data Fields
	T data;
	// The point to the next node 
	Node* next;

	// Constructor
	Node(const T& data_item, Node* next_ptr = NULL) : 
	data(data_item), next(next_ptr) {}
};
#endif************/