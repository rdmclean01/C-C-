/***********************************************************************
* Program:
*    Lab 09, Rock, Paper, Scissors 
* Author:
*    Ryan McLean
* Summary:
*    Introduction to writing and implementing classes
*	 This program will have a class to make new players
*	 Then it will have a rock, paper scissor contest
*************************************************************************/

#include "Player.h"
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

void initial(vector<Player*>& player_all);
void initial_Fight(vector<Player*> player_all, vector<Player*>& player_LineUp);
void display_Menu();
void show_Players(vector<Player*> player_all);
bool check_Match(vector<Player*> player_all, string check_Name);
void add_Player(vector<Player*>& player_all);
void add_Line_Up(vector<Player*>& player_all, vector<Player*>& player_LineUp);
void show_LineUp(vector<Player*> player_LineUp);
void fight(vector<Player*> player_all, vector<Player*>& player_LineUp);
double power_2(vector<string>& player_LineUp);

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
int main()
{
	vector<Player*> player_all;
	vector<Player*> player_LineUp;
	int command = 0;
	srand(time(NULL)); // makes it random

	initial(player_all);
	while (command != 10)
	{
		display_Menu();
		cin >> command;
		cin.ignore();
		if (command == 1) // Will display the players
			show_Players(player_all);
		else if (command == 2) // Will add a player
			add_Player(player_all);
		else if (command == 3) // Will add a player to line up
			add_Line_Up(player_all, player_LineUp);
		else if (command == 4) // Will show the line- up
			show_LineUp(player_LineUp);
		else if (command == 5) // Will play the game
			fight(player_all, player_LineUp);
		else if (command ==6) // Quits game
			command = 10;
		else
			command = -1;
	}

	//system("pause");
   return 0;
}

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
void initial(vector<Player*>& player_all)
{
	// player a(name, win, loss, draw)
	player_all.push_back(new Player ("Ryan"));
	player_all.push_back(new Player ("Ben"));
	return;
}
void initial_Fight(vector<Player*> player_all, vector<Player*>& player_LineUp)
{
	for (int i = 0; i < player_all.size(); i++)
	{
		Player* a = player_all[i];
		player_LineUp.push_back(a);
	}
}

/**********************************************************************
 * Displays the menu options
 ***********************************************************************/
void display_Menu()
{
	cout << "\t1. Show players\n";
	cout << "\t2. Add a player\n";
	cout << "\t3. Add to line-Up\n";
	cout << "\t4. Show line-Up\n";
	cout << "\t5. Play Rock, Paper, Scissors\n";
	cout << "\t6. Quit Game\n";
	cout << "\nPlease pick one of the following options: ";

	return;
}

/**********************************************************************
 * Will display the players in the all player vector.
 ***********************************************************************/
void show_Players(vector<Player*> player_all)
{
	if (player_all.size() == 0)
	{
	
		cout << "No one has signed up to play yet, Sorry.\n";
	}
	else
	{
		for (int i = 0; i < player_all.size(); i++)
		{
			cout << player_all[i]->toString() << endl;
		}
	}
	return;
}

/**********************************************************************
 * Here it checks if something is already in the vector and returns true
 * if it is.
 ***********************************************************************/
bool check_Match(vector<Player*> player_all, string check_Name)
{
	bool check;
	for (int i = 0; i < player_all.size(); i++)
	{
		if (player_all[i]->getName() == check_Name) 
		{
			check = true;
			return check;
		}
		else 
			check = false;
	}
	return check;
}

/**********************************************************************
 * Will add a player to the vector if not already in it
 ***********************************************************************/
void add_Player(vector<Player*>& player_all)
{
	string check_Name;
	cout << "What is the name you would like to add? ";
	getline(cin, check_Name);

	if (check_Match(player_all, check_Name) == true)
		cout << "Sorry, that player is already on the list.\n";
	else
	{
		
		player_all.push_back(new Player (check_Name));
		cout << "New player sucessfully added.\n";
	}
	return;
}

/**********************************************************************
 * Adds a player from the all player vector into the player line up vector
 ***********************************************************************/
void add_Line_Up(vector<Player*>& player_all, vector<Player*>& player_LineUp)
{
	for (int i = 0; i < player_all.size(); i++)
		cout << player_all[i]->getName() << endl;

	string check_Name;
	cout << "Which player would you like to add to the line up? ";
	getline(cin, check_Name);
	
	if (check_Match(player_all, check_Name) == true)
	{
		for (int i = 0; i < player_all.size(); i++)
		{
			if (player_all[i]->getName() == check_Name)
			{
				Player* a = player_all[i];
				player_LineUp.push_back(a);
				cout << "Player Successfully added to the Line Up.\n";
			}
		}
	}
	else
		cout << "Sorry, Player could not be found and was not added to the Line Up.\n";
}

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
void show_LineUp(vector<Player*> player_LineUp)
{
	cout << endl;
	if (player_LineUp.size() == 0)
		cout << "Everyone is still too scared to fight. Encourage them please.\n";
	else
		for (int i = 0; i < player_LineUp.size(); i++)
		{
			cout << player_LineUp[i]->toString() << endl;
		}
	return;
}

/**********************************************************************
 * Here we have a the fight ring of the contestants
 ***********************************************************************/
void fight(vector<Player*> player_all, vector<Player*>& player_LineUp)
{
	int x = 0; // For Player[0]
	int y = 0; // For Player[1]
	
	if (player_LineUp.size() == 0)
 		initial_Fight(player_all, player_LineUp);
	//initial_Fight(player_all, player_LineUp);
	cout << "Welcome to the Playground.\n";
	while (player_LineUp.size() != 0)
	{
		if (player_LineUp.size() % 2 != 0)
		{
			cout << "Error. Not enough players to continue.\n";
			return;
		}
		cout << player_LineUp[0]->getName() << " and " << player_LineUp[1]->getName();
		cout << " have begun to fight.\n";
		x = player_LineUp[0]->getRPSThrow();
		y = player_LineUp[1]->getRPSThrow();
		//cout << x << "\t" << y << endl; // DEBUG LINE!!!
		// 1 = Rock, 2 = Paper, 3 = Scissors
		if (x == y)
		{
			cout << "Both players threw the same sign. It is a draw.\n";
			player_LineUp[0]->fightDraw();
			player_LineUp[1]->fightDraw();
		}
		else if (player_LineUp[0]->getName() == player_LineUp[1]->getName())
		{
			cout << "It is a draw because they are the same person.\n";
			player_LineUp[0]->fightDraw();
			player_LineUp[1]->fightDraw();
		}
		else if (x == 1 && y == 3)
		{
			cout << player_LineUp[0]->getName() << " throws rock and " << player_LineUp[1]->getName() << " throws scissors. ";
			cout << player_LineUp[0]->getName() << " wins.\n";
			player_LineUp[0]->fightWin();
			player_LineUp[1]->fightLoss();
		}
		else if (x==2 && y == 1)
		{
			cout << player_LineUp[0]->getName() << " throws paper and " << player_LineUp[1]->getName() << " throws rock. ";	
			cout << player_LineUp[0]->getName() << " wins.\n";
			player_LineUp[0]->fightWin();
			player_LineUp[1]->fightLoss();
		}
		else if (x== 3 && y == 2)
		{
			cout << player_LineUp[0]->getName() << " throws scissors and " << player_LineUp[1]->getName() << " throws paper. ";	
			cout << player_LineUp[0]->getName() << " wins.\n"; // edit stats
			player_LineUp[0]->fightWin();
			player_LineUp[1]->fightLoss();
		}
		else if (y== 1 && x == 3)
		{
			cout << player_LineUp[1]->getName() << " throws rock and " << player_LineUp[0]->getName() << " throws scissors. ";
			cout << player_LineUp[1]->getName() << " wins.\n";
			player_LineUp[1]->fightWin();
			player_LineUp[0]->fightLoss();
		}
		else if (y==2 && x == 1)
		{
			cout << player_LineUp[1]->getName() << " throws paper and " << player_LineUp[0]->getName() << " throws rock. ";	
			cout << player_LineUp[1]->getName() << " wins.\n";
			player_LineUp[1]->fightWin();
			player_LineUp[0]->fightLoss();
		}
		else if (y == 3 && x == 2)
		{
			cout << player_LineUp[1]->getName() << " throws scissors and " << player_LineUp[0]->getName() << " throws paper. ";	
			cout << player_LineUp[1]->getName() << " wins.\n"; 
			player_LineUp[1]->fightWin();
			player_LineUp[0]->fightLoss();
		}
		player_LineUp.erase(player_LineUp.begin());
		player_LineUp.erase(player_LineUp.begin());
		cout << endl;
	}

}

double power_2(vector<string>& player_LineUp) // try taking out int size
{
	double size = player_LineUp.size();
	double num = log10(size);
	double log_2 = log10(2.0);
	double answer = 0;
	answer = (num / log_2);
	return answer;
}