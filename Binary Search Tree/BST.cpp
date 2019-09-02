/*
 * BST.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: rdmclean
 */
#include "BST.h"

Node* BST::getRootNode() // Should this be a nodeinterface
{
	return root;
}
//--------------------------------------------------------------------
bool BST::add(int data)
{
//	cout << "\tTRYING TO ADD " << data << endl;
//	cout << "\tCHECK CALLED\n";
	if(check(root, data) == false)
		return false;
	else
	{// cout << "\tSEND TO ADDREC\n";
		root = addRec(root, data); // DANGER
	//	cout << "DATA OF THE ROOT " << root->getData() << endl;
	//	cout << "RETURNED TRUE\n";
		return true;
	}

	return false;
}

//---------------------------------------------------------------------
Node* BST::addRec(Node* cur, int data)
{
	if(cur == NULL)
	{// cout << "\tADDED\n";
		Node* added = new Node(data, NULL, NULL);
	//	cout << "NUMBER ADDED " << added->getData() << endl;
		return added;
	}
	else if(cur->getData() < data)
	{// cout << "\tADD RIGHT\n";
	//	return addRec(cur->getRightChild(), data); // DANGER RIGHT POINTER
		// Node* addRi = new Node(cur->getData(), cur->getLeftChild(), addRec(cur->getRightChild(), data));
		 cur->setRight(addRec(cur->getRightChild(), data));
		 return cur;
	}
	else
	{// cout << "\tADD LEFT\n";

	//	Node* addLe = new Node(cur->getData(), addRec(cur->getLeftChild(), data), cur->getRightChild());
		cur->setLeft(addRec(cur->getLeftChild(), data));
		return cur;
	//	return addRec(cur->getLeftChild(), data); // DANGER LEFT POINTER
	}


}
//--------------------------------------------------------------------
bool BST::remove(int data)
{
//	cout << "\tTRYING TO REMOVE " << data << endl;
//	cout << "\tCHECK CALLED\n";
	if(check(root, data) == true)
	{// cout << "\tThe value was not found, so could not be removed.\n";
		return false;
	}
	else
	{
//		cout << "\tSEND TO REMREC\n";
		root = remRec(root, data);
		return true;
	}

	return false;
}
//--------------------------------------------------------------------
Node* BST::remRec(Node* cur, int data)
{ //int i = 0;
	if(cur->getData() == data)
	{// cout << "\tFOUND THE DATA TO BE REMOVED\n";
		if(cur->getLeftChild() == NULL)
		{// cout << "\t\t\tNO LEFT CHILD" << endl;
			Node* temp = cur->getRightChild();
			delete cur;
			// cout << "here\n";
			return temp;
		}
		else if(cur->getLeftChild() != NULL) // NOT SURE IF THIS SHOULD BE ANOTHER CASE
		{// cout << "\tSENT TO IOP\n";
			cur->setLeft(IOP(cur->getLeftChild(), cur));
			return cur;

		}
	}
	else if(cur->getData() < data)
	{// cout << "\tGO RIGHT\n";
		cur->setRight(remRec(cur->getRightChild(), data));
		return cur;
	}
	else
	{// cout << "\tGO LEFT\n";
		cur->setLeft(remRec(cur->getLeftChild(), data));
		return cur;
	}

	return NULL;
}
//--------------------------------------------------------------------
Node* BST::IOP(Node* cur, Node* value)
{
//	cout << "VALUE: " << value->getData() << endl;
	if(cur->getRightChild() == NULL)
	{// cout << "\tFOUND THE IOP\n";
	//	cout << "\tIOP IS " << cur->getData() << endl;
		value->setData(cur->getData());
		Node* curleft = cur->getLeftChild();
	//	cout << "DELETED " << cur->getData() << endl;
		delete cur;
	//	cout << "\t\t\tNEW ROOT: " << root->getData() << endl;
	//	cout << "ROOT LEFT " << root->getLeftChild()->getData() << endl;
	//	cout << "ROOT RIGHT " << root->getRightChild()->getData() << endl;
		return curleft;

	}
	else
	{// cout << "\tGO RIGHT\n";
		cur->setRight(IOP(cur->getRightChild(), value));
		return cur;
	}

	return NULL;
}
//--------------------------------------------------------------------
void BST::clear()
{
	while(getRootNode() != NULL)
	{// cout << "\t\t\t\tTO REMOVE: "<< root->getData() << endl;
	//	cout << "i BEFORE: " << i << endl;
//	cin.ignore();
	//	remRec(root, root->getData());
		remove(root->getData());
	//	cout << "HERE WE ARE\n";
	//	if(getRootNode() != NULL)
	//		cout << "\t\t\t\t\tROOT IS NOT NULL\n";
	//	cout << "ROOT DATA: " << getRootNode() << endl << endl;
	//	cout << "\n\t\t i after" << i << endl;

	}
//	cout << "\t\t\t\t\tLEFT THE WHILE LOOP\n";
}
//--------------------------------------------------------------------
bool BST::check(Node* cur, int data)
{
//	if(data == 4)
//	{
	//	cin.ignore();
//		cout << "DATA IS 4\n";
//		if(cur != NULL)
//		cout << "CHECKING AGAINST " << cur->getData() << endl;
//	}
//	cin.ignore();
	if(cur == NULL)
	{// cout << "\tFOUND THE END\n";
		return true;
	}
	else if(cur->getData() == data)
	{// cout << "\tFOUND DUPLICATE Of " << data << endl;
		return false; // FOUND DUPLICATE
	}
	else if(data > cur->getData())
	{// if(data == 4)
	//	cout << "CHECKING RIGHT\n";
		return check(cur->getRightChild(), data);

	}
	else
	{// if(data == 4)
	//	cout << "CHECKING LEFT\n";
		return check(cur->getLeftChild(), data);
	}

//	return true;
}




