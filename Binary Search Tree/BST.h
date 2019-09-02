/*
 * BST.h
 *
 *  Created on: Nov 7, 2014
 *      Author: rdmclean
 */

#ifndef BST_H_
#define BST_H_
#include "BSTInterface.h"
#include "Node.h"

using namespace std;

class BST : public BSTInterface
{
public:
	BST() {}
	~BST() {}

	//Please note that the class that implements this interface must be made
	//of objects which implement the NodeInterface

	/*
	 * Returns the root node for this tree
	 *
	 * @return the root node for this tree.
	 */
	Node * getRootNode();

	/*
	 * Attempts to add the given int to the BST tree
	 *
	 * @return true if added
	 * @return false if unsuccessful (i.e. the int is already in tree)
	 */
	bool add(int data);

	/*
	 * Attempts to remove the given int from the BST tree
	 *
	 * @return true if successfully removed
	 * @return false if remove is unsuccessful(i.e. the int is not in the tree)
	 */
	bool remove(int data);

	/*
	 * Removes all nodes from the tree, resulting in an empty tree.
	 */
	void clear();

	bool check(Node* cur, int data);

	Node* addRec(Node* cur, int data);

	Node* remRec(Node* cur, int data);

	Node* IOP(Node* cur, Node* value);

private:
	Node* root = NULL;
};





#endif /* BST_H_ */
