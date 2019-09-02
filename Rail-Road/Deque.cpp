/*
 * Deque.cpp
 *
 *  Created on: Oct 20, 2014
 *      Author: rdmclean
 */

#include "Deque.h"
using namespace std;


void Deque::addLeft(int car)
{
	My_list.insertHead(car);
}

void Deque::addRight(int car)
{
	My_list.insertTail(car);
}

void Deque::remove(int car)
{
	My_list.remove(car);
	return;
}

int Deque::size()
{
	return My_list.size();
}

bool Deque::empty()
{
	if(My_list.size() == 0)
		return true;
	else
		return false;
}

int Deque::top()
{
	return My_list.at(0);
}

int Deque::at(int car)
{
	return My_list.at(car);
}

int Deque::find(int car)
{
	return My_list.at(car);
}

bool Deque::check(int car)
{
	if(My_list.check(car) == true)
		return true;
	else
		return false;
}



