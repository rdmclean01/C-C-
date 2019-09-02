/*
 * Stack.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: rdmclean
 */

#include "Stack.h"
using namespace std;

void Stack::add(int car)
{
	My_list.insertHead(car);
}

int Stack::size()
{
	return My_list.size();
}

bool Stack::empty()
{
	if(My_list.size() == 0)
		return true;
	else
		return false;
}

int Stack::top()
{
	return My_list.at(0);
}

void Stack::remove(int car)
{
	My_list.remove(car);
		return;
}

bool Stack::check(int car)
{
	if(My_list.check(car) == true)
		return true;
	else
		return false;
}




