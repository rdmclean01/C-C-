/***********************************************************************
* Program:
*    Lab 01, Basic BYU Trivia
* Author:
*    Ryan McLean
* Summary:
*    Has a lot of trivia questions and answers about BYU
*************************************************************************/

#include <iostream>
using namespace std;
#include <string.h>
void display();

/**********************************************************************
 * Main will call the display function to display the data
 ***********************************************************************/
int main()
{
	cout << endl;
	display();
	cout << "\t\t\t\tBasic BYU Trivia\n\n"
		<< "\t\t\tQuestions\t\t\t   Answers\n\n"
		<< "What was the original name of BYU?\t\t  Brigham Young Academy\n"
		 << "When was BYA established?\t\t\t  1875\n"
		 << "Who was the first \"permanent\" principal of BYA?   Karl Maeser\n"
		 << "When did BYA become BYU?\t\t\t  1903\n"
		 << "To what sports conference do we belong?\t\t  West Coast Conference\n"
		 << "When did BYU win the national football title?     1984\n"
		 << "Who won the Heisman Trophy in 1990?\t\t  Ty Detmer\n";


	system("pause");
		
   return 0;
}

void display()
{
	cout << "                                Basic BYU Trivia   \n";
	cout << endl;
	cout << "                Questions                                 Answers\n";
	cout << endl;
	cout << "What was the original name of BYU?                Brigham Young Academy\n";
	cout << "When was BYA established?                         1875\n";
	cout << "Who was the first \"permanent\" principal of BYA?   Karl Maeser\n";
	cout << "When did BYA become BYU?                          1903\n";
	cout << "To what sports conference do we belong?           West Coast Conference\n";
	cout << "When did BYU win the national football title?     1984\n";
	cout << "Who won the Heisman Trophy in 1990?               Ty Detmer\n"; 
	
}
