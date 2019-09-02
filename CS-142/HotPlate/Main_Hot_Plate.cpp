/***********************************************************************
* Program:
*    Lab 06, Hot Plate 
* Author:
*    Ryan McLean
* Summary:
*    Enter a brief description of your program here!
*************************************************************************/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;

void initialMatrix(double matrix[][20]);
void display(double matrix[][20]);
double calcAverage(double matrix[][20]);
void writeFile(double matrix[][20]);

/**********************************************************************
 * Add text here to describe what the function "main" does.
 ***********************************************************************/
int main()
{
	double matrix[20][20];
	
	initialMatrix(matrix);
	display(matrix);
	cout << endl << endl; // just to make it look better
	calcAverage(matrix);
	display(matrix);
	while (calcAverage(matrix) > 0.1);
	cout << endl;
	display(matrix);
	writeFile(matrix);

	system("pause");
   return 0;
}

void display(double matrix[][20])
{
	for (int row = 0; row < 20; row++)
	{
		for (int col = 0; col < 20; col++)
		{
			cout << fixed << setprecision(1) << setw(6) << matrix[row][col] << " ";
		}
		cout << endl;
	}
	return;
}

void initialMatrix(double matrix[][20])
{
	for (int row = 0; row < 20; row++) // sets all the values in the left column to 0
		matrix[row][0] = 0;

	for (int row = 0; row < 20; row++) // sets all the values in the right column to 0
		matrix[row][19] = 0;

	for (int col = 1; col < 19; col++) // sets first row to 100
		matrix[0][col] = 100;

	for (int col = 1; col < 19; col++) // sets last row to 100
		matrix[19][col] = 100;

	for (int row = 1; row < 19; row++) // sets everything on the inside on the matrix to 0
	{
		for (int col = 1; col < 19; col++)
		{
			matrix[row][col] = 0;
		}
	}
	return;
}

double calcAverage(double matrix[][20])
{
	double change = 1;
	double previousValue = 0;
	double newChange = 0;
	double newValue = 0;
	int numTime = 0;

		change = 0;
		for (int row = 1; row < 19; row++)
		{
			for (int col = 1; col < 19; col++)
			{
				previousValue = matrix[row][col];
			//	cout << "Before: " << matrix[row][col] << endl;
				matrix[row][col] = (matrix[row - 1][col] + matrix[row + 1][col] + matrix[row][col - 1] + matrix[row][col + 1]) / 4;
				newValue = matrix[row][col];
				newChange = previousValue - newValue;
			//	cout << "After: " << matrix[row][col] << endl;
			//	cout << "Previous: " << previousValue << endl; // shows the value at the beginning
			//	cout << "New Value: " << newValue << endl;
				if (abs(newChange) > change)
					change = abs(newChange);
				//cout << "Change: " << abs(change) << endl;
			}
		}
	return change;
}

void writeFile(double matrix[][20])
{
	string fileName;
	ofstream fout;
	fout.open("Hot_Plate.csv"); 
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				fout << setprecision(2) << matrix[i][j] << ",";
			}

			fout << endl;
		}
		fout.close();
	return;
}