/***********************************************************************
* Program:
*    Project 2, Calendar 
*    Brother Grimmett, CS124
* Author:
*    Ryan McLean
* Summary: 
*    This program will be able to display a calendar for any given month
*    since the year 1753 when the calendar was reorganized.
*    Estimated:  3.0 hrs   
*    Actual:     5.0 hrs
*      Figuring out all of the offset stuff was really hard. I would get
*      it right and the spacing would be off and everything.
************************************************************************/

#include <iostream>
using namespace std;
#include <iomanip>

int getMonth();
int getYear();
void display(int month, int year);
void displayMonth(int month, int year);
void displayHeader(int month, int year);
int numDaysMonth(int month, int year);
int displayOffset(int day, int month, int year);
int isLeapYear(int year);
int calcOffSet(int month, int year);
int numDaysYear(int year);

/**********************************************************************
 * Main is designed to call certain functions to bring the month and
 * day into a place that is easy to call in the other functions.
 ***********************************************************************/
int main()
{
   int month = getMonth();
   int year = getYear();
   display(month, year);
   return 0;
}

/*****************************************************************
 * Prompts user for the number of month that he wants to see (1-12)
 ******* ********************************************************/
int getMonth()
{
   int month = 0;
   cout << "Enter a month number: ";
   cin >> month;
   while ( month < 1 || month > 12)
   {
      cout << "Month must be between 1 and 12.\n";
      cout << "Enter a month number: ";
      cin >> month;
   }
   return month;
}

/******************************************************
 * Prompts user for the year that he wants to see (>1753)
 ******************************************************/
int getYear()
{
   int year = 0;
   cout << "Enter year: ";
   cin  >> year;
   while ( year < 1753 )
   {
      cout << "Year must be 1753 or later.\n";
      cout << "Enter year: ";
      cin  >> year;
   }
   cout << endl;
   return year;
}

/***********************************************************
 * This will display everything and use it to call the display
 * functions and show the offset
 **************************************/
void display(int month,int year)
{  
   displayMonth(month, year);
   displayHeader(month, year);
   int day = numDaysMonth(month, year);
   displayOffset(day, month, year);
  
   return;
}

/***********************************************************
 * This will show the header and display the day of the week
 ************************************************************/
void displayHeader(int month, int year)
{

   cout << year << endl;
   cout << "  Su  Mo  Tu  We  Th  Fr  Sa" << endl;
   return;
}

/***********************************************************
 * This converts the number of month to the Name of the month
 ************************************************************/
void displayMonth(int month, int year)
{
   if ( month == 1)
      cout << "January, ";
   if ( month == 2)
      cout << "February, ";
   if ( month == 3)
      cout << "March, ";
   if ( month == 4)
      cout << "April, ";
   if ( month == 5)
      cout << "May, ";
   if ( month == 6)
      cout << "June, ";
   if ( month == 7)
      cout << "July, ";
   if ( month == 8)
      cout << "August, ";
   if ( month == 9)
      cout << "September, ";
   if ( month == 10)
      cout << "October, ";
   if ( month == 11)
      cout << "November, ";
   if ( month == 12)
      cout << "December, ";
   return;
}

/**********************************************************
 * This will calculate the number of Days in each Month
 ***********************************************************/
int numDaysMonth(int month, int year)
{
   int numDays;
   int leap = isLeapYear(year);
   if ( month == 1)
      numDays = 31;
   if ( month == 2)
   {
      numDays = 28;
      if (leap == 1)
         numDays++;
   }
   if ( month == 3)
      numDays = 31;
   if ( month == 4)
      numDays = 30;
   if ( month == 5)
      numDays = 31;
   if ( month == 6)
      numDays = 30;
   if ( month == 7)
      numDays = 31;
   if ( month == 8)
      numDays = 31;
   if ( month == 9)
      numDays = 30;
   if ( month == 10)
      numDays = 31;
   if ( month == 11)
      numDays = 30;
   if ( month == 12)
      numDays = 31;
   return numDays;
}

/***************************************************
 * This will calculate which day the month starts on
 * and display it in a calendar format.
 ******************************************************/
int displayOffset(int day, int month, int year)
{
   int offset = calcOffSet(month, year);
   if (offset != 6)
      for (int i = 0; i <= offset; i++)
         cout << "    ";
   for (int i = 1; i <= day; i++)
   {
      cout << setw(4) << i;
      if (((i + 1 + offset)  % 7 == 0))
      {
         if (i != day)
            cout << endl;
      }
   }
   cout << endl;
   return offset;
}

/******************************************************
 * This will calculate the day since 1753
 *******************************************************/
int calcOffSet(int month, int year)
{
   int sum = 0;
   for (int i = 1; i < month; i++)
      sum = sum + numDaysMonth(i, year);
   for (int i = 1753; i < year; i++)
      sum = sum + numDaysYear(i);
   int offset = sum % 7;
   return offset;
}

/**************************************************************
 * Calculates if the year is a leap year or not
 *************************************************************/
int isLeapYear(int year)
{
   int leapyear;
   if (year % 400 == 0)
      leapyear = 1;
   else if (year % 100 == 0)
      leapyear = 0;
   else if (year % 4 == 0)
      leapyear = 1;
   else
      leapyear = 0;
   return leapyear;
}

/*******************************************************
 * Assigns the number of days for a given year
 *****************************************************/
int numDaysYear(int year)
{
   int daysyear;
   int leap = isLeapYear(year);
   if (leap == 1)
      daysyear = 366;
   if (leap == 0)
      daysyear = 365;
   return daysyear;
}
