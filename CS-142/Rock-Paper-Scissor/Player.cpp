#include "Player.h"
using namespace std;
//-------------------------------------------------------------------
Player::Player(string name_in) // constructor for new players
{
	name = name_in;
	num_Win = 0;
	num_Loss = 0;
	num_Draw = 0;
	num_Games = num_Win + num_Loss + num_Draw;
	if (num_Games == 0)
		win_Record = 100;
	else
		win_Record = (num_Win / num_Games) * 1.0;
	

}
Player::~Player(){}
//---------------------------------------------------------------------------------------------------
int Player::getRPSThrow() // will get the throw
{
	int hand_sign;
	hand_sign = (rand() % 3) + 1;

	return hand_sign;
}
//-----------------------------------------------------------------------------------------------------
void Player::getWinRecord() // gets the win record
{
	win_Record = 0;
	/*
	if (num_Games == 0)
		win_Record = 100; // not working here
	else 
	//	win_Record = num_Win / num_Games;
		win_Record = 0;
	return win_Record;
	*/
}
//-----------------------------------------------------------------------------------------------------
string Player::toString() // able to print out all of the information
{
	stringstream ss;

	ss << "Name: " << name << endl;
	ss << "\tNumber of Wins " << num_Win << endl;
	ss << "\tNumber of Losses " << num_Loss << endl;
	ss << "\tNumber of Ties " << num_Draw << endl;
	if (num_Win + num_Loss + num_Draw == 0)
		ss << "\tWin Record: " << 100 << " %" << endl;
	else
		ss << "\tWin Record: " << (num_Win * 1.0 / (num_Win + num_Loss + num_Draw)) * 100.0  << " %" << endl;
	//ss << "\tNum Games: " << num_Win + num_Loss + num_Draw << endl;
	
	return ss.str();
}
//------------------------------------------------------------------------------------
string Player::getName() // will display the name of the car
{
	return name;
}
//------------------------------------------------------------------------------------
void Player::fightWin()
{
	num_Win++;
	return;
}
//------------------------------------------------------------------------------------
void Player::fightLoss()
{
	num_Loss++;
	return;
}
//------------------------------------------------------------------------------------
void Player::fightDraw()
{
	num_Draw++;
	return;
}
//------------------------------------------------------------------------------------
