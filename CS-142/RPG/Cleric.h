#include "Fighter.h"

class Cleric : public Fighter
{
	protected:
		int max_Mana;
		int Mana;
	public:
		Cleric(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in);
		int getDamage();
		void reset();
		bool useAbility();
		void regenerate();

};

