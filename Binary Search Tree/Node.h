/*
 * Node.h
 *
 *  Created on: Nov 7, 2014
 *      Author: rdmclean
 */

#ifndef NODE_H_
#define NODE_H_
#include "NodeInterface.h"

class Node : public NodeInterface
{


public:

//		Node* left;
//		Node* right;
//		int data;

	Node(int data_int, Node* l, Node* r) // maybe this constructor needs all 3 things, but I dont think so
	{
		data = data_int;
		left = l;
		right = r;
	}
	~Node() {}

	/*
	 * Returns the data that is stored in this node
	 *
	 * @return the data that is stored in this node.
	 */
	int getData()
	{
		return data;
	}

	/*
	 * Returns the left child of this node or null if it doesn't have one.
	 *
	 * @return the left child of this node or null if it doesn't have one.
	 */
	Node* getLeftChild()
	{ // check for NULL??????
		 return left;
	}

	/*
	 * Returns the right child of this node or null if it doesn't have one.
	 *
	 * @return the right child of this node or null if it doesn't have one.
	 */
	Node* getRightChild()
	{
		return right;
	}

	bool setData(int d)
	{
		data = d;
		return true;
	}

	bool setRight(Node* r)
	{
		right = r;
		return true;
	}
	bool setLeft(Node* l)
	{
		left = l;
		return true;
	}

//------------------------------------------------------------------------------------
private:
	Node* left;
	Node* right;
	int data;


};





#endif /* NODE_H_ */
