/***********************************************************************
* Program:
*    Lab 03, Hybrid Car
* Author:
*    Ryan McLean
* Summary:
*    This program will help the user to compare normal and hybrid cars.
*    Extra credit is 166.65 mpg
*    Prompt for which way they want to compare
*    display winners numbers first
*    
*************************************************************************/

#include <iostream>
#include <string>
using namespace std;

/**********************************************************************
 * Add text here to describe what the function "main" does. Also don't forget
 * to fill this out with meaningful text or YOU WILL LOSE POINTS.
 ***********************************************************************/
int main()
{
	float milesYear;
	float priceGallon;
	float priceHybrid;
	float mpgHybrid;
	float resaleHybrid;
	float priceCar;
	float mpgCar;
	float resaleCar;
	float galHybrid;
	float galCar;
	float costHybrid;
	float costCar;
	string compare;

	// I also need a final either gallons or total cost
	cout << "Estimated miles driven per year: ";
	cin >> milesYear;
	if (milesYear < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Estimated miles driven per year: ";
		cin >> milesYear;
	}
	cout << "Estimated price of a gallon of gas: ";
	cin >> priceGallon;
	if (priceGallon < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Estimated price of a gallon of gas: ";
		cin >> priceGallon;
	}
	cout << "Cost of a hybrid car: ";
	cin >> priceHybrid;
	if (priceHybrid < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Cost of a hybrid car: ";
		cin >> priceHybrid;
	}
	cout << "Efficiency of a hybrid car(mpg): ";
	cin >> mpgHybrid;
	if (mpgHybrid < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Efficiency of a hybrid car(mpg): ";
		cin >> mpgHybrid;
	}
	cout << "Resale of Hybrid after 5 years: ";
	cin >> resaleHybrid;
	if (resaleHybrid < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Resale of Hybrid after 5 years: ";
		cin >> resaleHybrid;
	}
	cout << "Cost of a normal car: ";
	cin >> priceCar;
	if (priceCar < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Cost of a normal car: ";
		cin >> priceCar;
	}
	cout << "Efficiency of a normal car(mpg): ";
	cin >> mpgCar;
	if (mpgCar < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Efficiency of a normal car(mpg): ";
		cin >> mpgCar;
	}
	cout << "Resale of car after 5 years: ";
	cin >> resaleCar;
	if (resaleCar < 0)
	{
		cout << "Error. Value must be positive\n";
		cout << "Resale of car after 5 years: ";
		cin >> resaleCar;
	}

	cout << "How would you like to compare the cars(gallons or cost)? ";
	cin >> compare;
		galHybrid = (milesYear * 5) / mpgHybrid;
		galCar = (milesYear * 5) / mpgCar;
		costHybrid = priceHybrid + (priceGallon * galHybrid) - resaleHybrid;
		costCar = priceCar + (priceGallon * galCar) - resaleCar;

	if (compare == "gallons")
	{
		
		if (galHybrid <= galCar)
		{
			cout << "Gallons of Hybrid " << galHybrid << endl;
			cout << "Hybrid Cost: " << costHybrid << endl;
			cout << "Gallons in Car: " << galCar << endl;
			cout << "Car Cost: " << costCar << endl;
		}	
		else
		{
			cout << "Gallons in Car: " << galCar << endl;
			cout << "Car Cost: " << costCar << endl;
			cout << "Gallons of Hybrid: " << galHybrid << endl;
			cout << "Hybrid Cost: " << costHybrid << endl;
		}
	}
	else
	{
		
		if (costHybrid <= costCar)
		{
			cout << "Hybrid Cost: " << costHybrid << endl;
			cout << "Gallons in Hybrid: " << galHybrid << endl;
			cout << "Car Cost: " << costCar << endl;
			cout << "Gallons in Car: " << galCar << endl;
		}
		else
		{
			cout << "Car Cost: " << costCar << endl;
			cout << "Gallons in Car: " << galCar << endl;
			cout << "Hybrid Cost: " << costHybrid << endl;
			cout << "Gallons in Hybrid: " << galHybrid << endl;

		}
	}

	/**************************************
	galHybrid = (milesYear * 5) / mpgHybrid;
	galCar = (milesYear * 5) / mpgCar;
	costHybrid = priceHybrid + (priceGallon * galHybrid) - resaleHybrid;
	costCar = priceCar + (priceGallon * galCar) - resaleCar;

	cout << endl;
	cout << "\t\tHybrid                          Non- Hybrid\n";
	cout << "\tTotal gallons in 5 years: " << galHybrid << "\tTotal Gallons in 5 years: " << galCar << endl;
	cout << "\tTotal cost for 5 years: " << costHybrid << "\tTotal cost for 5 years: " << costCar << endl;
************************************************/
	system("pause");
   return 0;
}


