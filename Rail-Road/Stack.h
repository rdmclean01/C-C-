/*
 * Stack.h
 *
 *  Created on: Oct 17, 2014
 *      Author: rdmclean
 */

#ifndef STACK_H_
#define STACK_H_
#include "LinkedList.h"

class Stack
{
	private:
		LinkedList<int> My_list;

	public:
		Stack(){}
		~Stack(){}

		void add(int car);

		int size();

		bool empty();

		int top();

		void remove(int car);

		bool check(int car);


};





#endif /* STACK_H_ */
