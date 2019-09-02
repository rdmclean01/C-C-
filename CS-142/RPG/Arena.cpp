#include "Arena.h"
#include "Fighter.h"
#include "Archer.h"
#include "Cleric.h"
#include "Robot.h"
#include "FighterInterface.h"
using namespace std;

bool Arena::addFighter(string info) // it comes in a single string which I need to break it down
{
	stringstream ss(info);
	string name;
	string type;
	int HP = 0;
	int strength = 0;
	int speed = 0;
	int magic = 0;
	int random = 0;

	ss >> name >> type >> HP >> strength >> speed >> magic;
	ss >> random;
	if (random != 0 || magic == 0 || speed == 0 || strength == 0 || HP == 0)
	{ return false;}
	for(int i = 0; i < Fighter_all.size(); i++)
	{ 
		if (name == Fighter_all[i]->getName()) // checks to make sure the name is not already there
		{ return false;}
	}
	if (type == "R")
	{
		Fighter_all.push_back(new Robot(name, type, HP, strength, speed, magic));
		return true;
	}
	else if(type == "C")
	{
		Fighter_all.push_back(new Cleric(name, type, HP, strength, speed, magic));
		return true;
	}
	else if(type == "A")
	{
		Fighter_all.push_back(new Archer(name, type, HP, strength, speed, magic));
		return true;
	}
	else 
		return false;
}
bool Arena::removeFighter(string name)
{
	for (int i = 0; i < Fighter_all.size(); i++)
	{
		if (name == Fighter_all[i]->getName())
		{
			Fighter_all.erase(Fighter_all.begin() + i);
			return true;
		}
	}
	return false;
}
FighterInterface* Arena::getFighter(string name)
{
	for (int i = 0; i < Fighter_all.size(); i++) // why does it not see it?
	{
		if (name == Fighter_all[i]->getName())
			return Fighter_all[i];
	}
	return NULL;
}
int Arena::getSize()
{
	return Fighter_all.size();
}

