#include "Fighter.h"

class Archer : public Fighter
{
	protected:

	public:
		Archer(string name_in, string type_in, int Hit_Point_in, int Strength_in, int Speed_in, int Magic_in);
		int getDamage();
		void reset();
		bool useAbility();
};