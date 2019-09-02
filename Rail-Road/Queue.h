/*
 * queue.h
 *
 *  Created on: Oct 17, 2014
 *      Author: rdmclean
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include "LinkedList.h"

class Queue
{
private:
	LinkedList<int> My_list;
public:
	Queue(){}
	~Queue(){}

	void add(int car);

	int size();

	bool empty();

	int top();

	void remove(int car);

	bool check(int car);

};





#endif /* QUEUE_H_ */
