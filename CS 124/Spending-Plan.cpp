/***********************************************************************
* Program:
*    Project 1, Budget Your Life            
*    Brother Grimmett, CS124
* Author:
*    Ryan McLean
* Summary: 
*   This program will ask you for your monthly budget and also display how
*   much you actually spend during the year. 
*
*    Estimated:  3.0 hrs   
*    Actual:     3.0 hrs
*      The style checker was the most difficult part, but also the tax part was
*      very complicated to me. It just seemed really bizarre what it wanted me
*      to do. 
************************************************************************/

#include <iostream>
using namespace std;
#include <iomanip>

/***********************************************************************
 * Prompts user for his income
************************************************************************/
float getIncome()
{
   float income;
   cout << "\tYour monthly income: "; // this is a comment
   cin  >> income;
   return income;
}

/***********************************************************************
 * Prompts user for his budgeted living expenses
************************************************************************/
float getBudgetLiving()
{
   float budget;
   cout << "\tYour budgeted living expenses: ";
   cin  >> budget;
   return budget;
}

/***********************************************************************
 * Prompts user for his actual living expenses
************************************************************************/
float getActualLiving()
{
   float living;
   cout << "\tYour actual living expenses: ";
   cin >> living;
   return living;
}

/***********************************************************************
 * Uses the user's income to find tax bracket and declare tax 
************************************************************************/
float computeTax(double monthlyIncome)
{
   float yearlyIncome = monthlyIncome * 12;
   float yearlyTax = 0;
   
   if (yearlyIncome > 0 && yearlyIncome <= 15100)
   {
      yearlyTax = yearlyIncome * 0.1;
   }
   if (yearlyIncome > 15100 && yearlyIncome <= 61300)
   {
      yearlyTax = 1510 + 0.15 * (yearlyIncome - 15100);
   }
   if (yearlyIncome > 61300 && yearlyIncome <= 123700)
   {
      yearlyTax = 8440 + 0.25 * (yearlyIncome - 61300);
   }
   if (yearlyIncome > 123700 && yearlyIncome <= 188450)
   {
      yearlyTax = 24040 + 0.28 * (yearlyIncome - 123700);
   }
   if (yearlyIncome > 188450 && yearlyIncome <= 336550)
   {
      yearlyTax = 42170 + 0.33 * (yearlyIncome - 188450);
   }
   if (yearlyIncome > 336550)
   {
      yearlyTax = 91043 + 0.35 * (yearlyIncome - 336550);
   }
   float monthlyTax = yearlyTax / 12;
   return monthlyTax;
}

/***********************************************************************
 * Prompts user for other expenses
************************************************************************/
float getActualOther()
{
   float other;
   cout << "\tYour actual other expenses: ";
   cin >> other;
   return other;
}

/***********************************************************************
 * Uses user's income to compute tithing
************************************************************************/
float computeTithing(float income)
{
   float fullTithe = income * .10;
   
   return fullTithe;
}

/***********************************************************************
 * Prompts user for withheld taxes
************************************************************************/
float getActualTax()
{
   float tax;
   cout << "\tYour actual taxes withheld: ";
   cin >> tax;
   return tax;
}

/***********************************************************************
 * Prompts user for paid Tithing
************************************************************************/
float getActualTithing()
{
   float tithing;
   cout << "\tYour actual tithe offerings: ";
   cin >> tithing;
   return tithing;
}

/***********************************************************************
 * Displays the information from the program on the screen
************************************************************************/
void display(float income, float budgetLiving, float actualLiving,
             float actualTax, float actualTithing, float actualOther)
{
   cout.setf(ios::fixed);
   cout.setf(ios::showpoint);
   cout.precision(2);
   float budgetTax = computeTax(income);
   float budgetTithing = computeTithing(income);
   float budgetOther = income - budgetTax - budgetTithing - budgetLiving;
   float actualDifference = income  - actualTax - actualTithing -
      actualLiving - actualOther;

   cout << endl
        << "The following is a report on your monthly expenses\n" 
        << "\tItem                  Budget          Actual\n"
        << "\t=============== =============== ===============\n"
        << "\tIncome          $" << setw(11) << income << "    $"
        << setw(11) << income << endl
        << "\tTaxes           $" << setw(11) << budgetTax << "    $"
        << setw(11) << actualTax << endl
        << "\tTithing         $" << setw(11) << budgetTithing << "    $"
        << setw(11) << actualTithing << endl
        << "\tLiving          $" << setw(11) << budgetLiving << "    $"
        << setw(11) << actualLiving << endl
        << "\tOther           $" << setw(11) << budgetOther << "    $"
        << setw(11) << actualOther << endl
        << "\t=============== =============== ===============\n"
        << "\tDifference      $" << setw(11) << 0.00 << "    $"
        << setw(11) << actualDifference << endl;
   return;
}

/**********************************************************************
 * Main in this program calls all of the functions and declares the variables 
 * for the display.
 ***********************************************************************/
int main()
{
   cout << "This program keeps track of your monthly budget\n";
   cout << "Please enter the following:\n";
   float income = getIncome();
   float budgetLiving = getBudgetLiving();
   float actualLiving = getActualLiving();
   float actualTax = getActualTax();
   float actualTithing = getActualTithing();
   float actualOther = getActualOther();

   display(income, budgetLiving, actualLiving, actualTax,
           actualTithing, actualOther);

   return 0;
}
