/*
 * Queue.cpp
 *
 *  Created on: Oct 20, 2014
 *      Author: rdmclean
 */
#include "Queue.h"
using namespace std;



void Queue::add(int car)
{
	My_list.insertTail(car);
}

int Queue::size()
{
	return My_list.size();
}

bool Queue::empty()
{
	if(My_list.size() == 0)
		return true;
	else
		return false;
}

int Queue::top()
{
	return My_list.at(0);
}

void Queue::remove(int car)
{
	My_list.remove(car);
	return;
}

bool Queue::check(int car)
{
	if(My_list.check(car) == true)
		return true;
	else
		return false;
}
