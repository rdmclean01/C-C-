/***********************************************************************
* Program:
*    Lab 04, Plinko 
* Author:
*    Ryan McLean
* Summary:
*    This program is a child's game that simulates a board where the user drops
*    a ball/disk into this board and it goes through an 8 by 12 board with
*    pegs that the ball can go through with spaces at the bottom for the ball
*    to go in and each space has a monetary value.
*    Needs to display the number of the spot
*		I need to send command into the board and if command == 1 then print off the coordinate
*************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
//#include <stdio.h>
using namespace std;

void displayMenu();
int board(int coordinate, int totalMoney, int &newMoney, int command);
/**********************************************************************
 * Main will call to display the menu and allow the user to pick an option
 * Depending on what their choice is will have ceratin options
 * After doing the calculations, it will display the monetary values
 ***********************************************************************/
int main()
{
	int command = 0;
	int coordinate= 0;
	int numChips = 0;
	int numChip = 0;
	int totalMoney = 0;
	int newMoney = 0;

	srand(time(NULL));
	//board(coordinate, money);
	while (command != 3)
	{
		displayMenu();
		cout << "Which number would you like? ";
		cin >> command;
		if (command == 1)
		{
			cout << "Please select a coordinate on the plinko board? ";
			cin >> coordinate;
			if (coordinate < 0 || coordinate > 8)
			{
				command = 4;
			}
			totalMoney = board(coordinate, totalMoney, newMoney, command);
			cout << endl;
			cout << "New Money: " << newMoney << endl;
			cout << "Total Money: " << totalMoney << endl;
			numChip++;
		}

		if (command == 2)
		{
			cout << "How many chips would you like to drop? ";
			cin >> numChip;
			if (numChip < 0)
			{
				command = 4;
			}
			cout << "Which slot would you like to put the chips in? ";
			cin >> coordinate;
			if (coordinate < 0 || coordinate > 8)
				command = 4;
			for (int i = 0; i < numChip; i++)
			{
				totalMoney = board(coordinate, totalMoney, newMoney, command);
				numChips++;
				//cout << coordinate << endl;
			}
			cout << endl;
			cout << "Total Money:   " << totalMoney << endl;  
			cout << "Average Money: " << totalMoney / (numChips + numChip) << endl; // it does not continue the average, the average is just from that run. Problem??
		}
	}
	//system("pause");
   return 0;
}

/******************************************************************
* Will display the options when it needs to
*******************************************************************/
void displayMenu()
{
	cout << endl;
	cout << "\t\tMenu Options\n";
	cout << "\t1. Drop one chip into one slot \n";
	cout << "\t2. Drop multiple chips into one slot \n";
	cout << "\t3. Quit game \n";
	cout << endl;
}

/*************************************************************
* It is a simulation of the game Plinko with a random number
* generator and will do certain things based on how the 
* numbers come in and go out
* **********************************************************/
int board(int coordinate, int totalMoney, int &newMoney, int command)
{
	coordinate *= 100;
	int x = 0; 
//	srand(time(NULL)); // this makes it go completely randomly
	for (int i = 0; i < 12; i++)
	{
	//	cout << endl;
		switch (coordinate)
		{
			case 0:
				coordinate += 50;
				break;
			case 800:
				coordinate -= 50;
				break;
			default:
				//sleep(1);
			//	cout << "time " << time(0) << endl; // displays the time used for random
				x = rand() % 2;
			 
	//			cout << x << endl; // displays the random number for the turn
				if (x == 0)
					coordinate -= 50;
					//if (x > 50)
				else if (x == 1)
					coordinate += 50;
		}
		if (command == 1)
			cout << "Coordinate " << coordinate << endl; // this line displays where on the board it is 

	}
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
	
}