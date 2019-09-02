#include <iostream>
#include <string>
#include <sstream>
#include "FighterInterface.h"
using namespace std;

/*
 *	WARNING: It is expressly forbidden to modify any part of this document, including its name
 * THIS HEADER FILE IS JUST FOR DEBUGGING
 */
//=======================================================================================
FighterInterface* fight(FighterInterface*, FighterInterface*, bool);
void takeTurn(FighterInterface*, FighterInterface*, bool);
void speak(string);
//=======================================================================================
/*
 *	fight(FighterInterface, FighterInterface, bool)
 *
 *	Runs through the fighting algorithm for the two given fighters.
 *
 *	Each fighter takes a turn using its special ability, attacking, and regenerating. If 
 *	the other fighter's current hit points falls to or drops below 0, then the other 
 *	fighter has lost.  This process repeats until one fighter has fallen or 100 rounds 
 *	have occurred with neither fighter falling.
 *
 *	If the given boolean is true, this function will print a great deal of information 
 *	during the battle.  This can be useful for testing.
 *
 *	Returns the winner of the fight; null if the fight ends in a draw.
 */
FighterInterface* fight(FighterInterface* c1, FighterInterface* c2, bool verbose)
{
	if(verbose)
	{
		stringstream stream;
		stream << "----------";
		stream << c1->getName();
		stream << " vs ";
		stream << c2->getName();
		stream << "----------";
		speak(stream.str());
	}
	
	c1->reset();
	c2->reset();

	for(int i=0; i<100; i++)
	{
		//C1's turn
		takeTurn(c1, c2, verbose);
		if(c2->getCurrentHP() <= 0)
		{
			if(verbose)
			{
				stringstream stream;
				stream << c1->getName();
				stream << " wins! (";
				stream << c1->getCurrentHP();
				stream << "/";
				stream << c1->getMaximumHP();
				stream << " HP left)";
				speak(stream.str());
			}
			return c1;
		}
		
		//C2's turn
		takeTurn(c2, c1, verbose);
		if(c1->getCurrentHP() <= 0)
		{
			if(verbose)
			{
				stringstream stream;
				stream << c2->getName();
				stream << " wins! (";
				stream << c2->getCurrentHP();
				stream << "/";
				stream << c2->getMaximumHP();
				stream << " HP left)";
				speak(stream.str());
			}
			return c2;
		}
	}
	if(verbose)
	{
		speak("After 100 rounds, neither fighter has fallen.  It's a draw!");
	}
	return NULL;
}
//=======================================================================================
/*
 *	takeTurn(FighterInterface*, FighterInterface*, bool)
 *
 *	Runs through a single turn for [attacker] attacking [defender].  Each turn consists of
 *	[attacker] trying to use its special ability, attacking [defender], and regenerating.
 */
void takeTurn(FighterInterface* attacker, FighterInterface* defender, bool verbose)
{
	//Header
	if(verbose)
	{
		stringstream stream;
		stream << "It's ";
		stream << attacker->getName();
		stream << "'s turn! (";
		stream << attacker->getCurrentHP();
		stream << "/";
		stream << attacker->getMaximumHP();
		stream << " HP).";
		speak(stream.str());
	}

	//Use Ability
	bool ability = attacker->useAbility();
	if(ability  &&  verbose)
	{
		stringstream stream;
		stream << "\t";
		stream << attacker->getName();
		stream << " uses a special ability!";
		speak(stream.str());
	}

	//Attack
	int damage = attacker->getDamage();
	int before_attack = defender->getCurrentHP();
	defender->takeDamage(damage);
	int after_attack = defender->getCurrentHP();
	if(verbose)
	{
			stringstream stream;
			stream << "\t";
			stream << attacker->getName();
			stream << " attacks with ";
			stream << damage;
			stream << " damage, and ";
			stream << defender->getName();
			stream << " takes ";
			stream << (before_attack-after_attack);
			stream << " damage.";
			speak(stream.str());
	}

	//Regenerate
	int before_regen = attacker->getCurrentHP();
	attacker->regenerate();
	int after_regen = attacker->getCurrentHP();
	if(verbose)
	{
		stringstream stream;
		stream << "\t";
		stream << attacker->getName();
		stream << " regenerates ";
		stream << (after_regen-before_regen);
		stream << " HP.";
		speak(stream.str());
	}
}
//=======================================================================================
/*
 *	speak(string)
 *
 *	Displays a given message.
 */
void speak(string message)
{
	cout << "[Battle.h Testing] " << message << endl;
}
//=======================================================================================