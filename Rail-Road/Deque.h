/*
 * deque.h
 *
 *  Created on: Oct 17, 2014
 *      Author: rdmclean
 */

#ifndef DEQUE_H_
#define DEQUE_H_
#include "LinkedList.h"

class Deque
{
private:
	LinkedList<int> My_list;
public:
	Deque(){}
	~Deque(){}

	void addLeft(int car);

	void addRight(int car);

	//void removeLeft();
	//void removeRight();
	void remove(int car);
	int size();

	bool empty();

	int top();

	int at(int car);

	int find(int car);

	bool check(int car);


};



#endif /* DEQUE_H_ */
