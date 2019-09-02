#include "Fighter.h"
using namespace std;

//----------------------------------------------------------------------------------------
Fighter::Fighter(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in)
{
	name = name_in;
	type = type_in;
	Hit_Point = Hit_Point_in;
	Strength = Strength_in;
	Speed = Speed_in;
	Speed_begin = Speed_in;
	Magic = Magic_in;
	max_HP = Hit_Point_in; // make sure this is not changing
}
Fighter::~Fighter(){}
//----------------------------------------------------------------------------------------
string Fighter::getName()
{
	return name;
}
int Fighter::getMaximumHP()
{
	return max_HP;
}
int Fighter::getCurrentHP()
{
	return Hit_Point;
}
int Fighter::getStrength()
{
	return Strength;
}
int Fighter::getSpeed()
{
	return Speed;
}
int Fighter::getMagic()
{
	return Magic;
}
void Fighter::takeDamage(int damage)
{
	damage = damage-(Speed / 4); // I dont understand why this is wrong
	if (damage < 1)
		Hit_Point--;
	else
		Hit_Point -= damage;
}
void Fighter::regenerate()
{
	int temp = (Strength / 6);
	//cout << "Increase " << temp << endl;
		if (temp < 1)
			Hit_Point++;
		else
			Hit_Point += temp;
		if (Hit_Point > max_HP)
			Hit_Point = max_HP;
}
int Fighter::getDamage()
{
	return 0;
}
void Fighter::reset()
{
	return;
}
bool Fighter::useAbility()
{
	return false;
}


/*
int main()
{
	system("pause");
	return 0;
}
*/
