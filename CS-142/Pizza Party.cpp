/***********************************************************************
* Program:
*    Lab 2, BYUSA Pizza count
* Author:
*    Ryan McLean
* Summary:
*    Do a series of calculations and things
*    I still need to work on the decimal points and the table spacing
*    round to the nearest dollar using integers and not double
*************************************************************************/

#include <iostream>
#include <iomanip>
using namespace std;

void calcPizza(int numguest, int numpeople);
void serving(int large, int medium, int small, int numpeople);
void calcTip(int large, int medium, int small, float tip);
/**********************************************************************
 * Prompts the user for the number of guests that are attending
 * and then sends to calculating function
 ***********************************************************************/
int main()
{

	int numguest;
	int numpeople = 0;
	
	cout << endl;
	cout << "How many guests will be attending your pizza party? ";
	cin >> numguest;
	numpeople = numguest;
	calcPizza(numguest, numpeople);
	
   return 0;
}

/**********************************************************************
 * Prompts the user for the percent of tip, then calculates number of pizza type
 * Sends the user to the calculate area function
 * Sends the user to the calculate tip/ price function
 ***********************************************************************/
void calcPizza(int numguest, int numpeople)
{
	int large;
	int medium;
	int small;
	float tip = 0;
	
	cout << "Please enter the percent given as a tip on the price? ";
	cin >> tip;

	
	large = numguest / 7; 
	numguest = (numguest - (7 * large));
	

	medium = numguest / 3;
	numguest = (numguest - (3 * medium));
	
	small = numguest;

	


	cout << "\tLarge Pizzas: " << setw(4) << large << endl;
	cout << "\tMedium Pizzas: " << setw(3) << medium << endl;
	cout << "\tSmall Pizzas: " << setw(4) << small << endl;
	serving(large, medium, small, numpeople);	
	calcTip(large, medium, small, tip);
	return;
}

/**********************************************************************
 * Calculates and displays both the area of pizza and area per person
 ***********************************************************************/
void serving(int large, int medium, int small, int numpeople)
{
	float asum = 0; // area = pi Dsquared over 4
	float alarge;
	float amed;
	float asmall;

	alarge = ((3.14159 * 400) / 4);
	asum += alarge * large;

	amed = ((3.14159 * 256) / 4);
	asum += amed * medium;

	asmall = ((3.14159 * 144) / 4);
	asum += asmall * small; 
	

	//cout << "You must buy " << asum << " square inches of pizza.\n";
	//cout << "Each person can eat " << asum / numpeople << " square inches of pizza.\n";
	cout << "\tin^2 of pizza: " << setw(7) << asum << endl;
	cout << "\tin^2 per person: " << setw(5) << asum / numpeople << endl;
	return;

}

/**********************************************************************
 * Uses the tip to calculate the total price of the pizza at the party
 ***********************************************************************/
void calcTip(int large, int medium, int small, float tip)
{
	float price = 0;
	int total = 0;
	

	price += large * 14.68;
	price += medium * 11.48;
	price += small * 7.28;
	//cerr << price << endl;
	tip = (tip / 100) * price;
	//cerr << tip << endl;
	price = price + tip + 0.5;

	total = price;

	cout << "\tTotal price: $" << setw(8) << total << endl; 

	system("pause");

}