#include "Cleric.h"
using namespace std;

Cleric::Cleric(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in)
	:Fighter(name_in, type_in, Hit_Point_in, Strength_in, Speed_in, Magic_in)
{
	max_Mana = 5 * Magic;
}

int Cleric::getDamage()
{
	return Magic;
}
void Cleric::reset()
{
	Hit_Point = max_HP;
	Mana = max_Mana;
}
bool Cleric::useAbility()
{	
	int temp = Magic / 3;
	if (temp < 1)
		temp = 1;
	if (Mana >= CLERIC_ABILITY_COST)
	{ 
		Mana -= CLERIC_ABILITY_COST;
		if ((Hit_Point + temp) <= max_HP) // QUESTION? MAYBE HERE
			Hit_Point += temp;
		return true;
	}
		return false;
}
void Cleric::regenerate()
{
	Fighter::regenerate(); // do something in addition to the other one
	int temp = (Magic / 5);
	if (Mana < max_Mana)
	{
		if (temp < 1)
			Mana++;
		else
			Mana += temp;
	}
	
}