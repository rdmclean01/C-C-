/***********************************************************************
* Program:
*    Lab 05, Plinko 
* Author:
*    Ryan McLean
* Summary:
*    This program is a child's game that simulates a board where the user drops
*    a ball/disk into this board and it goes through an 8 by 12 board with
*    pegs that the ball can go through with spaces at the bottom for the ball
*    to go in and each space has a monetary value.
*************************************************************************/
//Switch coodinate to be a double
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#define ERROR_VALUE 10
#define IGNORE_VALUE 1000
using namespace std;

void displayMenu();
int board(int coordinate, int totalMoney, int &newMoney, int command);
int oneChip(int coordinate, int totalMoney, int newMoney, int command, int &numChip); // maybe command needs to be changed somehow
int multipleChips(int coordinate, int totalMoney, int newMoney, int command, int &numChip, int &numChips);
int allSlots(int coordinate, int totalMoney, int newMoney, int command, int &numChip, int &numChips);

/**********************************************************************
 * Main will call the display menu function and then prompt the user for a 
 * choice and based on that choice will send to other functions
 * to play the game
 ***********************************************************************/
int main()
{
	int command = 0;
	int coordinate= 0;
	int numChips = 0;
	int numChip = 0;
	int totalMoney = 0;
	int newMoney = 0;
	srand(time(NULL)); // makes it random

	while (command != 4)
	{
		displayMenu();
		cout << "Which number would you like? ";
		cin >> command;
		if (cin.fail()) {
			
			cin.clear();
			cin.ignore(IGNORE_VALUE, '\n');
			cout << "Value must be a number, not a string.\n";
		}
		else if (command == 1)
		{
			//totalMoney = oneChip(coordinate, totalMoney, newMoney, command, numChip);
			oneChip(coordinate, totalMoney, newMoney, command, numChip);
		}
		else if (command == 2)
		{
			//totalMoney = multipleChips(coordinate, totalMoney, newMoney, command, numChip, numChips);
			multipleChips(coordinate, totalMoney, newMoney, command, numChip, numChips);
		}
		else if (command == 3)
		{
			allSlots(coordinate, totalMoney, newMoney, command, numChip, numChips);
		}
	}
   return 0;
}

/**********************************************************************
 * Displays the menu options and the ways that the user can play the game.
 ***********************************************************************/
void displayMenu()
{
	cout << endl;
	cout << "\t\tMenu Options\n";
	cout << "\t1. Drop one chip into one slot \n";
	cout << "\t2. Drop multiple chips into one slot \n";
	cout << "\t3. Drop multiple chips into all slots \n";
	cout << "\t4. Quit game \n";
	cout << endl;
}

/**********************************************************************
 * Calculates if a person puts one chip into one slot
 ***********************************************************************/
int oneChip(int coordinate, int totalMoney, int newMoney, int command, int &numChip)
{
	cout << "Please select a coordinate on the plinko board? ";
	cin >> coordinate;
	if (cin.fail()) {

		cin.clear();
		cin.ignore(IGNORE_VALUE, '\n');
		cout << "Value must be a number, not a string.\n";
	}
	else if (coordinate < 0 || coordinate > 8)
	{
		command = ERROR_VALUE;
	}
	else {
		totalMoney = board(coordinate, totalMoney, newMoney, command); // success
		cout << endl;
		cout << "New Money: " << newMoney << endl;
		cout << "Total Money: " << totalMoney << endl;
		numChip++;
		// what should I do with the returns
		//return totalMoney;
	}
	return 0;
}

/**********************************************************************
 * Calculates if a person puts multiple chips into one slot
 ***********************************************************************/
int multipleChips(int coordinate, int totalMoney, int newMoney, int command, int &numChip, int &numChips)
{
	cout << "How many chips would you like to drop? ";
			cin >> numChips;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(IGNORE_VALUE, '\n');
				cout << "Num Chip input failed\n";
			}
			else if (numChips <= 0)
			{
				command = ERROR_VALUE;
			}
			else {
				cout << "Which slot would you like to put the chips in? ";
				cin >> coordinate;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(IGNORE_VALUE, '\n');
					cout << "Coordinate Input failed\n";
				}
				else if (coordinate < 0 || coordinate > 8)
					command = ERROR_VALUE;
				else {
					for (int i = 0; i < numChips; i++)
					{
						totalMoney = board(coordinate, totalMoney, newMoney, command); // problem here
					}
					cout << endl;
					cout << "Total Money:   " << totalMoney << endl; // the Total is coming up super weird, it adds on total and then board
					cout << "Average Money: " << totalMoney / (numChips + numChip) << endl; // not keep track of other chips
					//return totalMoney;
				}
			}
			return 0;
}

/**********************************************************************
 * Calculates if a person puts multiple chips into all slots
 ***********************************************************************/
int allSlots(int coordinate, int totalMoney, int newMoney, int command, int &numChip, int &numChips)
{
	cout << "How many chips would you like to drop into all slots? ";
	cin >> numChips; 
	if (cin.fail()) {
		cin.clear();
		cin.ignore(IGNORE_VALUE, '\n');
		cout << "All slot num chips failed\n";
	}
	else if (numChips <= 0)
		command = ERROR_VALUE;
	else {
		for (coordinate = 0; coordinate <= 8; coordinate++) // fix the eternal loop
		{
			for (int i = 0; i < numChips; i++)
			{
				totalMoney = board(coordinate, totalMoney, newMoney, command);
			}

		}
		cout << "Total Money:   " << totalMoney << endl; // the problem is that the average is then from 
		cout << "Average Money: " << totalMoney / (numChips * 8) << endl;
		//return totalMoney;
	}
	return 0;

}

/*************************************************************
* This will calculate random numbers and simulate what the board does
* in an actual plinko game as it randomly falls down
* also at the end, will assign money to the user based on 
* where the chip falls.
* **********************************************************/
int board(int coordinate, int totalMoney, int &newMoney, int command)
{
	coordinate *= 100;
	int x = 0; 
	const double MOVE_VALUE = 50;
	cout << "[";
	for (int i = 0; i < 12; i++)
	{
	//	cout << endl;
		switch (coordinate)
		{
			case 0:
				coordinate += MOVE_VALUE;
				break;
			case 800:
				coordinate -= MOVE_VALUE;
				break;
			default:
				//sleep(1);
			//	cout << "time " << time(0) << endl; // displays the time used for random
				x = rand() % 2; 
			//	cout << x << endl; // displays the random number for the turn
				if (x == 0)
					coordinate -= MOVE_VALUE;
					//if (x > 50)
				else if ( x==1)
					coordinate += MOVE_VALUE;
		}
		if (command == 1)
		{
			//cout << "Coordinate " << coordinate << endl; // this line displays where on the board it is
			cout << " " << coordinate << " ";
		
		}
	}
	cout << "]\n";
	switch (coordinate)
	{
		case 0:
			totalMoney += 100;
			newMoney = 100;
			break;
		case 100:
			totalMoney += 500;
			newMoney = 500;
			break;
		case 200:
			totalMoney += 1000;
			newMoney = 1000;
			break;
		case 300:
			totalMoney += 0;
			newMoney = 0;
			break;
		case 400:
			totalMoney += 10000;
			newMoney = 10000;
			break;
		case 500:
			totalMoney += 0;
			newMoney = 0;
			break;
		case 600:
			totalMoney += 1000;
			newMoney = 1000;
			break;
		case 700:
			totalMoney += 500;
			newMoney = 500;
			break;
		case 800:
			totalMoney += 100;
			newMoney = 100;
			break;
			
	}
	return totalMoney;
	// right now the coordinate is times 100 so adjust accordingly
}