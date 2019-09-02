#include "QSInterface.h"
#include "QS.h"


void displayMenu();

int main()
{
	int command = 0;
	QS a;
	while(command != 8)
	{
		
		displayMenu();
		cout << "Please pick:\n";
		cin >> command;
		cin.ignore();
		if(command ==1)
			a.sortAll();
		if(command == 2)
			cout << "ARRAY: " << a.getArray() << endl;
		if(command == 3)
		{	int value;
			cout << "What is the integer to add?\n";
			cin >> value;
			a.addToArray(value);
		}
		if(command ==4)
		{
			int value;
			cout << "Size of Array?\n";
			cin >> value;
			a.createArray(value);
		}
		if(command == 5)
			a.clear();
	}
	return 0;
}

//-------------------------------------------------------------------------
void displayMenu()
{
	cout << "\t1. Sort All\n";
	cout << "\t2. Get Array\n";
	cout << "\t3. Add to Array\n";
	cout << "\t4. Create Array\n";
	cout << "\t5. Clear\n";
	cout << "\t8. Quit\n";
}