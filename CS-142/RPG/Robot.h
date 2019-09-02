#include "Fighter.h"

class Robot:public Fighter
{
	protected:
		int max_Electricity; 
		int Energy;
		double bonus_Damage_d;
		int bonus_Damage;
	public:
		Robot(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in);
		int getDamage();
		void reset();
		bool useAbility();
};