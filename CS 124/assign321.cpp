/***********************************************************************
* Program:
*       The way assign32 was done in class, so simple....
************************************************************************/

#include <iostream>
using namespace std;

char getLetter()
{
   char letter;
   cout << "Enter a letter: ";
   cin >> letter;
   cin.ignore();
   return letter;
}
void getText(char text[])
{
   cout << "Enter text: ";
   cin.getline(text, 256);
   return;
}
void display (char text[], char letter)
{int count = 0;
   for ( int i = 0; text[i]; i++) // this is the best for printing out your word or array/string.
   {
      if (text[i] == letter)
         count++;
   }
   cout << "Number of '" << letter << "'s: " << count << endl;
   
   return;
}
/**********************************************************************
 * Add text here to describe what the function "main" does. Also don't forget
 * to fill this out with meaningful text or YOU WILL LOSE POINTS.
 ***********************************************************************/
int main()
{
   char letter;
   char text[256];
   letter = getLetter();
   getText(text);
   display (text, letter);  
   
   return 0;
}
