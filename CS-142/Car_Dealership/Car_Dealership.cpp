/***********************************************************************
* Program:
*    Lab 08, Car Dealership 
* Author:
*    Ryan McLean
* Summary:
*    This one is messed up and doesn't work properly
*************************************************************************/

#include "Car.h"
using namespace std;
void display_Menu();

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
int main()
{
	double balance_Money = 10000.00;

	display_Menu();
	system("pause");
   return 0;
}

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
void display_Menu()
{
	cout << "Please pick one of these options: \n";
	cout << "\t1. Show current Inventory.\n";
	cout << "\t2. Show current Balance.\n";
	cout << "\t3. Buy a Car.\n";
	cout << "\t4. Sell a Car.\n";
	cout << "\t5. Paint a Car.\n";
	cout << "\t6. Load File.\n";
	cout << "\t7. Save File.\n";
	cout << "\t8. Quit Program.\n";
	return;
}


