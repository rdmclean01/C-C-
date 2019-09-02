/*
 * Station.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: rdmclean
 */
#include "Station.h"
using namespace std;

//Part 1--------------------------------------------------------------
				/**
				 * Let another car arrive at the station and become the current car.
				 * Do not allow a new car to arrive if any of the following conditions apply:
				 * 1.	There is already a current car
				 * 2.	The new car's ID already exists in any structure
				 * 3.	The new car's ID is negative
				 *
				 * @param car the ID of the car arriving at the station
				 * @return true if the car successfully arrived; false otherwise
				 */
				bool Station::addToStation(int car)
				{
					if(current != -1) 	// CHECK 1
						return false;
					if(car < 0)			//CHECK 3
						return false;
					if(my_stack.check(car) == true || my_deque.check(car) == true || my_queue.check(car) == true) // CHECK 2
						return false;
					current = car; // ASSIGN CURRENT
				//	// cout << "ADDED TO STATION: " << current << endl;
					return true;
				}

				/**
				 * Returns the ID of the current car.
				 * Return -1 if there is no current car.
				 *
				 * @return the ID of the current car; -1 if there is no current car
				 */
				int Station::showCurrentCar()
				{
				//	// cout << "SHOW CURRENT CAR: " << current << endl;
					return current; // MAY HAVE TO CHECK BETTER
				}

				/**
				 * Removes the current car from the station.
				 * Does nothing if there is no current car.
				 *
				 * @return true if the current car successfully left; false otherwise
				 */
				bool Station::removeFromStation()
				{
					if(current == -1)
						return false;
					else
					{
						current = -1;
					//	// cout << "REMOVED FROM STATION\n";
						return true;
					}
					// cout << "NOT REMOVED FROM STATION\n";
					return false;
				}

				//Part 2--------------------------------------------------------------
				/**
				 * Adds the current car to the stack.  After this operation, there should be no current car.
				 * Does nothing if there is no current car or if the stack is already full.
				 *
				 * @return true if the car is successfully added to the stack; false otherwise
				 */
				bool Station::addToStack()
				{
					if(current == -1)
						return false;
					if(my_stack.size() == CAPACITY)
						return false;

					my_stack.add(current);
					// cout << "ADDED TO STACK: " << current << endl;
					current = -1;
					return true;
				}

				/**
				 * Removes the first car in the stack and makes it the current car.
				 * Does nothing if there is already a current car or if the stack already empty.
				 *
				 * @return true if the car is successfully removed from the stack; false otherwise
				 */
				bool Station::removeFromStack()
				{
					if(current != -1)
						return false;
					if(my_stack.empty() == true)
						return false;
					current = my_stack.top();
					// cout << "REMOVED FROM STACK: " << my_stack.top() << endl;
					my_stack.remove(my_stack.top());
					return true;
				}

				/**
				 * Returns the ID of the first car in the stack.
				 *
				 * @return the ID of the first car in the stack; -1 if the stack is empty
				 */
				int Station::showTopOfStack()
				{

					if(my_stack.empty() == true)
						return -1;
					// cout << "TOP OF STACK: " << my_stack.top() << endl;
						return my_stack.top();
				}

				/**
				 * Returns the number of cars in the stack.
				 *
				 * @return the number of cars in the stack
				 */
				int Station::showSizeOfStack()
				{
					// cout << "SIZE STACK: " << my_stack.size() << endl;
					return my_stack.size();
				}

				//Part 3--------------------------------------------------------------
				/**
				 * Adds the current car to the queue.  After this operation, there should be no current car.
				 * Does nothing if there is no current car or if the queue is already full.
				 *
				 * @return true if the car is successfully added to the queue; false otherwise
				 */
				bool Station::addToQueue()
				{
					if(current == -1)
						return false;
					if(my_queue.size() == CAPACITY)
						return false;
					my_queue.add(current);
					// cout << "ADDED TO QUEUE: " << current << endl;
					current = -1;
					return true;
				}

				/**
				 * Removes the first car in the queue and makes it the current car.
				 * Does nothing if there is already a current car or if the queue already empty.
				 *
				 * @return true if the car is successfully removed from the queue; false otherwise
				 */
				bool Station::removeFromQueue()
				{
					if(current != -1)
						return false;
					if(my_queue.empty() == true)
						return false;
					current = my_queue.top();
					my_queue.remove(my_queue.top());
					// cout << "REMOVED FROM QUEUE: " << current << endl;
					return true;
				}

				/**
				 * Returns the ID of the first car in the queue.
				 *
				 * @return the ID of the first car in the queue; -1 if the queue is empty
				 */
				int Station::showTopOfQueue()
				{
					if(my_queue.empty() == true)
						return -1;
					// cout << "TOP OF QUEUE: " << my_queue.top() << endl;
						return my_queue.top();
				}

				/**
				 * Returns the number of cars in the queue.
				 *
				 * @return the number of cars in the queue
				 */
				int Station::showSizeOfQueue()
				{
					// cout << "SIZE OF QUEUE: " << my_queue.size() << endl;
					return my_queue.size();
				}

				//Part 4--------------------------------------------------------------
				/**
				 * Adds the current car to the deque on the left side.  After this operation, there should be no current car.
				 * Does nothing if there is no current car or if the deque is already full.
				 *
				 * @return true if the car is successfully added to the deque; false otherwise
				 */
				bool Station::addToDequeLeft()
				{
					if(current == -1)
						return false;
					if(my_deque.size() == CAPACITY)
						return false;
					my_deque.addLeft(current);
					// cout << "ADDED TO THE LEFT: " << current << endl;
					current = -1;
					return true;
				}

				/**
				 * Adds the current car to the deque on the right side.  After this operation, there should be no current car.
				 * Does nothing if there is no current car or if the deque is already full.
				 *
				 * @return true if the car is successfully added to the deque; false otherwise
				 */
				bool Station::addToDequeRight()
				{
					if(current == -1)
						return false;
					if(my_deque.size() == CAPACITY)
						return false;
					my_deque.addRight(current);
					// cout << "ADDED TO THE RIGHT: " << current << endl;
					current = -1;
					return true;

				}

				/**
				 * Removes the leftmost car in the deque and makes it the current car.
				 * Does nothing if there is already a current car or if the deque already empty.
				 *
				 * @return true if the car is successfully removed from the deque; false otherwise
				 */
				bool Station::removeFromDequeLeft()
				{
					if(current != -1)
						return false;
					if(my_deque.empty() == true)
						return false;
					current = my_deque.top();
					my_deque.remove(my_deque.top());
					// cout << "REMOVED FROM DEQUE: " << current << endl;
					return true;
				}

				/**
				 * Removes the rightmost car in the deque and makes it the current car.
				 * Does nothing if there is already a current car or if the deque already empty.
				 *
				 * @return true if the car is successfully removed from the deque; false otherwise
				 */
				bool Station::removeFromDequeRight()
				{
					if(current != -1)
						return false;
					if(my_deque.empty() == true)
						return false;
					current = my_deque.find(my_deque.size() - 1);
					//my_deque.remove(my_deque.size()-1);
					my_deque.remove(my_deque.find(my_deque.size() - 1));
					// cout << "\tREMOVED FROM DEQUE RIGHT: " << current << endl;
					return true;
				}

				/**
				 * Returns the ID of the leftmost car in the deque.
				 *
				 * @return the ID of the leftmost car in the deque; -1 if the deque is empty
				 */
				int Station::showTopOfDequeLeft()
				{
					if(my_deque.empty() == true)
						return -1;
					// cout << "TOP OF DEQUE LEFT: " << my_deque.top() << endl;
					return my_deque.top();
				}

				/**
				 * Returns the ID of the rightmost car in the deque.
				 *
				 * @return the ID of the rightmost car in the deque; -1 if the deque is empty
				 */
				int Station::showTopOfDequeRight()
				{
					if(my_deque.empty() == true)
						return -1;
					// cout << "\tTOP DEQUE RIGHT: " << my_deque.find(my_deque.size() - 1) << endl;
					return my_deque.find(my_deque.size() - 1);
				}

				/**
				 * Returns the number of cars in the deque.
				 *
				 * @return the number of cars in the deque
				 */
				int Station::showSizeOfDeque()
				{
					// cout << "SIZE OF DEQUE: " << my_deque.size() << endl;
					return my_deque.size();
				}




