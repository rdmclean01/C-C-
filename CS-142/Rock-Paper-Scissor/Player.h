#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

class Player
{
	private:
		string name;
		int num_Win;
		int num_Loss;
		int num_Draw ;
		int num_Games;
		double win_Record;



public:
	//---------------------------------------------------------
	/*
	 * Constructor/ Destructor
	 *
	 * Handles creation and deletion of Player pointer objects
	 * 
	 *
	 */

	Player(string name_in);
	virtual ~Player();
	//---------------------------------------------------------
	/*
	 * Simulate the players turn
	 *
	 * Randomize the selection of choosing rock, paper or scissors 
	 * 
	 * Return the chosen 'throw'
	 */
	int getRPSThrow();

	//---------------------------------------------------------
	/*
	 * A function to show the person record
	 *
	 * Get the player's win record
	 * 
	 * number of wins / all matches the player has fought
	 */
	void getWinRecord();

	//---------------------------------------------------------
	/*
	 * Displays all the information about the player
	 *
	 * Will return a string that has all of the info
	 * about a player (including win record
	 *
	 */
	string toString();

	//-------------------------------------------------------
	/*
	*
	* getName
	*
	* Returns the name of the player
	*/
	string getName(); 
	void fightWin(); // If someone wins, call this function to add it to their record
	void fightLoss(); // If someone loses, call to record it
	void fightDraw(); // If someone draws, call to record it
	//double getMatch(); // When someone plays, call to record it. Might need it
	 
};

