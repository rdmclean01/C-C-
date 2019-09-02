#include "Robot.h"
using namespace std;

Robot::Robot(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in)
	:Fighter(name_in, type_in, Hit_Point_in, Strength_in, Speed_in, Magic_in)		   	
{
	max_Electricity = 2 * Magic;
	Energy = max_Electricity;
	bonus_Damage = 0;
}
int Robot::getDamage()
{
	int temp = 0;
	//cout << "Bonus " << bonus_Damage << endl;
		temp = Strength + bonus_Damage;
		bonus_Damage = 0;
		return temp;
}
void Robot::reset()
{
	Hit_Point = max_HP;
	Energy = max_Electricity;
	bonus_Damage = 0;
}
bool Robot::useAbility()
{
	//cout << "Current " << Energy << endl;
	if (Energy >= ROBOT_ABILITY_COST)
	{
		double current_Energy = Energy;
		double maximum = max_Electricity;
		bonus_Damage_d = Strength * pow(current_Energy / maximum, 4.0); //does the crazy arithmetic
		bonus_Damage = bonus_Damage_d; // to cast into an integer
		Energy -= ROBOT_ABILITY_COST;
	//	cout << "BONUS D " << bonus_Damage << endl;
		return true;
	}
	else 
		return false;
}