#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include "LinkedListInterface.h"
#include <iostream>
#include <string>
using namespace std;
/*******************
This might not work here, but it is the correct and working solution to the problem on a linux machine
*******************/

template <class T>
class LinkedList : public LinkedListInterface<T>
{


	public:
		struct Node {
	/**** Data Fields****/
	T data;
	/**** The pointer to the next node****/
	Node* next;

	// Constructor
	Node(const T& data_item, Node* next_ptr = NULL) : 
	data(data_item), next(next_ptr) {}
};

		LinkedList() : head(NULL){}
		~LinkedList(){ clear();}

		/*
		insertHead

		A node with the given value should be inserted at the beginning of the list.

		Do not allow duplicate values in the list.
	 */
	void insertHead(T value)
	{

		if(check(value) == false)
		{
			if(head == NULL)
			{
				Node* ptr = new Node(value);
				head = ptr;
			}
			else
			{
				Node* ptr = new Node(value);
				Node* temp = head;
				ptr-> next = temp;
				head = ptr;
			//	temp = NULL;
			}
		}
	//	temp = NULL; //DANGEROUS
	//	ptr = NULL;//DANGEROUS
	}
 
		
	/*
		insertTail

		A node with the given value should be inserted at the end of the list.

		Do not allow duplicate values in the list.
	 */
	void insertTail(T value)
	{
		if(check(value) == true)
			return;
		Node* temp = head;
		if (head == NULL)
		{
			head = new Node(value);
			return;
		}
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		if(temp->next == NULL)
		temp->next = new Node(value);
		temp = NULL;//DANGEROUS
	}

	/*
		insertAfter

		A node with the given value should be inserted immediately after the
		node whose value is equal to insertionNode.

		A node should only be added if the node whose value is equal to
		insertionNode is in the list. Do not allow duplicate values in the list.
	 */
	void insertAfter(T value, T insertionNode)
	{
		if (check(value) == true || check(insertionNode) == false)
			return;
		Node* ptr = head;
		Node* my_node = new Node(value);
		while (ptr-> next != NULL)
		{
			if (ptr->data == insertionNode)
			{
				my_node-> next = ptr-> next;
				ptr->next = my_node;
				return;
			}
			ptr = ptr-> next;
		}
		if(ptr->data == insertionNode)
		{
			ptr->next = my_node;
		}
		ptr = NULL;//DANGEROUS
		my_node = NULL;// DANGEROUS
	}
	
	/*
		remove

		The node with the given value should be removed from the list.

		The list may or may not include a node with the given value.
	 */
	void remove(T value)
	{
		if(check(value) == false)
			return;
		Node* temp = head;
		Node* toDelete;
		if (head->data == value)
		{
			toDelete = head;
			head = head->next;
			delete toDelete;
		//	toDelete = NULL;
			return;
		}
		while (temp->next->data != value)
		{
			temp = temp->next;
		}
		toDelete = temp->next;
		temp->next = temp->next->next;

		delete toDelete;
	//	toDelete = NULL;
		temp = NULL;//DANGEROUS
	}

	/*
		clear

		Remove all nodes from the list.
	 */
	void clear()
	{
		Node* ptr = head; 
		while(head != NULL)
		{
			ptr = head;
			head = head->next;
			delete ptr;
		}
	}

	/*
		at

		Returns the value of the node at the given index. The list begins at
		index 0.

		If the given index is out of range of the list, return NULL;
	 */
	T at(int index)
	{
		Node* ptr = head;
		if(index < 0 || index >= size())
			return NULL; // RETURN NULL IF BREAKS
		else
		{
			for(int i = 0; i < index; i++)
			{
				ptr = ptr->next;
			}
			return ptr->data;
		}
		//ptr = NULL; //DANGEROUS
	}

	/*
		size

		Returns the number of nodes in the list.
	 */
	int size()
	{
		Node* ptr = head;
		int counter = 1;
		if (ptr == NULL)
			return 0;
		while(ptr->next != NULL)
		{
			ptr = ptr-> next;
			counter++;
		}
		ptr = NULL;//DANGEROUS
		return counter; 
	}
	bool check(T value)
	{
		Node* temp = head;
		while(temp != NULL)
		{
			if(temp->data == value)
			{
				return true;
			}
			temp = temp->next;
		}
		//	temp = NULL;//DANGEROUS
		return false;
	}

	private:
		Node* head = NULL;//DANGEROUS
};
#endif
