#include "Archer.h"
using namespace std;

Archer::Archer(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in)
	:Fighter(name_in, type_in, Hit_Point_in, Strength_in, Speed_in, Magic_in)
{  
}
int Archer::getDamage()
{
	return Speed;
}
void Archer::reset()
{
	Hit_Point = max_HP;
	Speed = Speed_begin;
}
bool Archer::useAbility()
{
	Speed++;
	return true;
}

