/***********************************************************************
* Program:
*    Project 3, Mad Lib
*    Brother Grimmett, CS124
* Author:
*    Ryan McLean
* Summary: 
*    This is a child's game. The user will enter certain random words
*    which will then get read into a story format which the computer
*    provides. 
*    Estimated:  4.0 hrs   
*    Actual:     3.0 hrs
*      Getting the j for loop and getting rid of the spaces was the most
*      confusing part for me.
************************************************************************/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cctype>
#include <cstdlib>
using namespace std;

void getReadFileName(char fileName[]);
int readFile(char fileName[], char data[][256]);
void display(char story[][256], int size);
void parseWord(char story[][256], int size);

/**********************************************************************
 * Main will call the functions to read a file and send and receive 
 * information to print out the story. 
 ***********************************************************************/
int main(int argc, char **argv)
{
   char fileName[256];
   char story[1000][256];
   char repeat;
   do
   {
      if (argc == 1)
         getReadFileName(fileName);
      else
         strcpy(fileName, argv[1]);
       
      int size = readFile(fileName, story);
      if (size < 0)
      {
         cout << "Yahoo" << endl;
      }
      else
      {
         parseWord(story, size);
         cout << endl;
         display(story, size);
      }
      cout << endl;
      cout << "Do you want to play again (y/n)? ";
      cin >> repeat;
   }
   while (repeat == 'y');
   cout << "Thank you for playing.\n";
   
   return 0;
}

/*************************************************************************
 * Prompts the user for the file name which includes a Mad Lib.
 ************************************************************************/
void getReadFileName(char fileName[])
{
   cout << "Please enter the filename of the Mad Lib: ";
   cin >> fileName;
   cin.ignore();
   return;
}

/*************************************************************************
 * This function will read the file and puts the information into a
 * variable data to then be converted to size
 ************************************************************************/
int readFile(char fileName[], char data[][256])
{
   ifstream input;
   int i = 0;

   input.open(fileName);

   if (input.fail())
      return -1;

   while (input >> data[i])
      i++;

   input.close();
   
   return i;
}

/*************************************************************************
 * This function will display character by character the story
 ************************************************************************/
void display(char story[][256], int size)
{
   for (int i = 0; i < size; i++)
      if ((ispunct(story[i + 1][0]) && story[i + 1][0] != '(')
          || story[i][0] == '\n'
          || story[i + 1][0] == '\n'
          || i == size - 1
          || story[i][1] == '"'
          || story[i + 1][1] == '"'
          || story[i + 1][1] == '\''
          || story [i][1] == '\''
          )
      {
         cout << story[i];
      }
      else
         cout << story[i] << " ";
   
   return;
}

/*************************************************************************
 * This will take certain characters and convert them to things that are
 * readable by the user and child.
 ************************************************************************/
void parseWord(char story[][256], int size)
{
   int i = 0;
   int j = 0;
   for (i; i < size; i++)
   {
      if (story[i][0] == '<')
      {
         switch (story[i][1])
         {
            case '#':
               story[i][0] = '\n';
               story[i][1] = 0;
               break;
            case '{':
               story[i][0] = ' ';
               story[i][1] = '\"';
               story[i][2] = 0;
               break;
            case '}':
               story[i][0] = '\"';
               story[i][1] = 0;
               break;
            case '[':
               story[i][0] = ' ';
               story[i][1] = '\'';
               story[i][2] = 0;
               break;
            case ']':
               story[i][0] = '\'';
               story[i][1] = 0;
               break; 
            default:
               for (j = 1; story[i][j] != '>'; j++)
               {
                  if (story[i][j] == '_')
                     story[i][j - 1] = ' ';
                  else
                     story[i][j - 1] = story[i][j];
               }
               story[i][j - 1] = 0;
               story [i][0] = toupper(story[i][0]);
               cout << "\t" << story[i] << ": ";
               cin.getline(story[i], 256);
         }
      }
   }
   return;
}

