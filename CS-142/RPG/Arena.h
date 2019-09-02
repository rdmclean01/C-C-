#pragma once
#include "ArenaInterface.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "FighterInterface.h"
#include "Fighter.h"
using namespace std;

class Arena : public ArenaInterface
{
	private:
		vector<Fighter*> Fighter_all;
	public:
		Arena(){}
		virtual ~Arena(){}
		
		/*
		 *	addFighter(string)
		 *
		 *	Adds a new fighter to the collection of fighters in the arena. Do not allow 
		 *	duplicate names.  Reject any string that does not adhere to the format 
		 *	outlined in the lab specs.
		 *
		 *	Return true if a new fighter was added; false otherwise.
		 */
		virtual bool addFighter(string info); 

		/*
		 *	removeFighter(string)
		 *
		 *	Removes the fighter whose name is equal to the given name.  Does nothing if 
		 *	no fighter is found with the given name.
		 *
		 *	Return true if a fighter is removed; false otherwise.
		 */
		virtual bool removeFighter(string name);
		
		/*
		 *	getFighter(string)
		 *
		 *	Returns the memory address of a fighter whose name is equal to the given 
		 *	name.  Returns NULL if no fighter is found with the given name.
		 *
		 *	Return a memory address if a fighter is found; NULL otherwise.
		 */
		virtual FighterInterface* getFighter(string name);
		
		/*
		 *	getSize()
		 *
		 *	Returns the number of fighters in the arena.
		 *
		 *	Return a non-negative integer.
		 */
		virtual int getSize();
};