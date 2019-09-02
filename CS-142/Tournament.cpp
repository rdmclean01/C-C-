/***********************************************************************
* Program:
*    Lab 06, Restaurant Competition 
* Author:
*    Ryan McLean
* Summary:
*    Allows the user to pick where to go for dinner by allowing him to choose
*	between 2 options at a time of all his favorite restaurants
*************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

void display_Menu();
//void initial(vector<string>& restaurants);
vector<string> initial(); //changed everything so it didnt overload me
void display_restaurant(vector<string> restaurants, int size);
void add_restaurant(vector<string>& restaurants, int size);
void remove_restaurant(vector<string>& restaurants, int size);
void shuffle_vector(vector<string>& restaurants, int size);
bool tournament(vector<string>& restaurants, int size);
double power_2(vector<string>& restaurants);
int check_Match(vector<string>& restaurants, string input);

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
int main()
{
	int command = 0;
	vector<string> restaurants(16);
	int size = restaurants.size();
	restaurants = initial(); 
	while(command != 6) // for some reason, it does not stop unless I assign it to six a second time
	{
		//power_2(restaurants); // this is just for debugging
		size = restaurants.size();
		display_Menu();
		cin >> command;
		//getline(cin, command);
		if (command == 1)
			display_restaurant(restaurants, size); // for some reason it gives me some weird vector subscript out of range
		if (command == 2)
			add_restaurant(restaurants, size);
		if (command == 3)
			remove_restaurant(restaurants, size);
		if (command == 4) {
			shuffle_vector(restaurants, size);
			display_restaurant(restaurants, size); }
		if (command == 5) 
		{
			if (tournament(restaurants, size) == true)
				return 0;
			else
				command = -1;
		}
		if (command == 6)
			command = 6;
		else
			command = -1; // will just make it repeat because the command was invalid
	}

	//system("pause");
   return 0;
}

/**********************************************************************
 * Displays the menu options.
 ***********************************************************************/
void display_Menu()
{
	cout << endl;
	cout << "1. Display all restaurants.\n";
	cout << "2. Add a restaurant.\n";
	cout << "3. Remove a restaurant.\n";
	cout << "4. Shuffle the vector.\n";
	cout << "5. Begin the tournament.\n";
	cout << "6. Quit program.\n";
	return;
}

/**********************************************************************
 * Initially declares 16 restaurants to choose from
 ***********************************************************************/
//void initial(vector<string>& restaurants)
vector<string> initial()
{
	vector<string> names(16);
	names[0] = "Cheese Cake Factory";
	names[1] = "Sonic";
	names[2] = "Tucanos";
	names[3] = "Burger King";
	names[4] = "Arbys";
	names[5] = "Jamba Juice";
	names[6] = "In and Out";
	names[7] = "Chick' Fil A";
	names[8] = "Pizza Pie Cafe";
	names[9] = "Zupas";
	names[10] = "Five Guys";
	names[11] = "KFC";
	names[12] = "Wendy's";
	names[13] = "Cafe Rio";
	names[14] = "Panda Express";
	names[15] = "Sizzler";

	return names; // needs to return something if I change it back 
}

/**********************************************************************
 * Displays the names of the restaurants
 ***********************************************************************/
void display_restaurant(vector<string> restaurants, int size)
{
	size = restaurants.size();
	cout << endl;
	for (int i = 0; i < size; i++) 
		cout << "\t" << restaurants[i] << endl;
	return;
}

/**********************************************************************
 * Adds new restaurants to the tournament. 
 * needs to check if already in tournament
 ***********************************************************************/
void add_restaurant(vector<string>& restaurants, int size)
{
	string input;
	string input2;
	string ask = "y";
	while (ask == "y")
	{
		cout << "Please add a restaurant. ";
		cin.ignore();
		getline(cin, input);
		if (check_Match(restaurants, input) == 0)
		{
		restaurants.push_back(input);
		cout << "Sucessfully added.\n";
		cout << "Would you like to add a second restaurant?(y/n)";
		cin >> ask;
		}
		else 
			cout << "Error, your Restaurant is already included in the tournament.\n";
		/*if (ask == 'y')
		{
			cout << "Please add a restaurant: ";
			cin.ignore();
			getline(cin, input);
			restaurants.push_back(input);
		}
		else*/
	}
//	if (restaurants.size() % 2 != 0)
//	if (power_2(restaurants) == false)
	if (fmod(power_2(restaurants), 1) != 0)
		cout << endl << "Please remember to do something to make your restaurants have a power of 2 for the tournament. \n";
	//cout << "Log math " << setprecision(4) << power_2(restaurants);
	size = restaurants.size();
	display_restaurant(restaurants, size); // just display it here in the program
	

	return;
}

/**********************************************************************
 * Can take restaurants out of the tournament
 * need a getline
 ***********************************************************************/
void remove_restaurant(vector<string>& restaurants, int size)
{
	string input;
	string ask = "y";
	display_restaurant(restaurants, size);
	while (ask == "y")
	{
		cout << "Which restaurant would you like to take out of the mix? ";
		cin.ignore();
		getline(cin, input);
		//size = restaurants.size();
		if (check_Match(restaurants, input) == 1)
		{
			for (int i = 0; i < restaurants.size(); i++)
			{
			if (input == restaurants[i])
			restaurants.erase(restaurants.begin() + i); 
			//cout << restaurants[i] << endl;
			}
			cout << "Sucessfully removed.\n"; 
		}
		else
			cout << "Restaurant was not found in the tournament.\n";
		cout << "Would you like to remove another? ";
		cin >> ask;
	}
	//if (power_2(restaurants) == false)
	if (fmod(power_2(restaurants), 1) != 0) // problem here maybe
		cout << endl << "Please remember to do something to make your restaurants have a power of 2 for the tournament. \n";
	display_restaurant(restaurants, size);
	
	return;
}

/**********************************************************************
 * This will shuffle the vector and change the order to make the tournament
 * more random
 ***********************************************************************/
void shuffle_vector(vector<string>& restaurants, int size)
{
	random_shuffle (restaurants.begin(), restaurants.end() );
	//display_restaurant(restaurants, size);

	return;
}

/**********************************************************************
 * This is a tournament which will be a series of choices for the user
 * He will choose between 2 things until he has chosen all of them
 * then it will repeat with only the winners every time until there is
 * only one left
 ***********************************************************************/
bool tournament(vector<string>& restaurants, int size)
{
	int input;
	int round = 1; // this may have to use some math to calculate how many per game
	double num_Round = power_2(restaurants);
	int match = 1;
	int num_Match = restaurants.size() / 2;
	if (fmod(power_2(restaurants), 1) != 0) // switch back to false
	{
		cout << endl << "Error: Cannot start tournament unless the number of restaurants is a power of 2. \n";
		system("pause");
		return false;
		
	}

	cout << "\tPlease pick between the following restaurants: \n";
	cout << "\tPlease input a 1 or a 2. \n";

	while (restaurants.size() > 1) // if it is not 2 to a power, it does not work
	{ 
		match = 1;
		num_Match = restaurants.size() / 2;
		shuffle_vector(restaurants, size);
		size = restaurants.size();
		for (int i = 0; i < restaurants.size(); i++)
		{
			cout << "\nRound " << round << "/" << num_Round << "  Match " << match << "/" << num_Match << endl;
			cout << "Restaurant 1: " << restaurants[i] << "\tRestaurant 2: " << restaurants[i+1] << endl;
			cin >> input;
			if (input == 1)
				restaurants.erase(restaurants.begin() + i + 1);
			else if (input == 2)
				restaurants.erase(restaurants.begin() + i);
			else {
				cout << "Error: You must input a 1 or a 2. \n";
				return false; }
			match++;
		}
		round++;
	} 
	cout << "Congratulations the winner is " << restaurants[0] << endl;
	cout << "Enjoy your dinner :) \n";
	system("pause");

	
	return true;
}

/**********************************************************************
 * Checks to see if the restaurant is already in the vector
 ***********************************************************************/
int check_Match(vector<string>& restaurants, string input)
{
	int match = 0;
	//int size = restaurants.size();
	for (int i = 0; i < restaurants.size(); i++)
	{
		if (input == restaurants[i])
			match++;
	}
	return match;
}

/**********************************************************************
 * Checks to see if the vector size is a power of 2
 ***********************************************************************/
double power_2(vector<string>& restaurants) // try taking out int size
{
	double size = restaurants.size();
	//double num = log10(restaurants.size());
	double num = log10(size);
	double log_2 = log10(2.0);
	double answer = 0;
	answer = (num / log_2);
	//cout << "Answer: " << answer; for debugging
	return answer;
	//answer = fmod(answer, 1);
	/************
	if(answer == 0)
		return true;
	else
		return false;
	**************/
}