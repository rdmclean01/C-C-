/*
 * QS.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: rdmclean
 */
/*
 * QS.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: rdmclean
 */

#include "QS.h"
//--------------------------------------------------------------------------------------
void QS::sortAll() 
{
	if(getSize() == 0)
	{
		createArray(50); 
		addToArray(20); 
		addToArray(15);  
		addToArray(35); 
		addToArray(100);
		addToArray(49); 
		addToArray(10);  
		addToArray(8); 
		addToArray(27); 
		addToArray(111);
		addToArray(12);

		addToArray(7); 
		addToArray(19);  
		addToArray(28); 
		addToArray(3);
		addToArray(111); 
		addToArray(657);  
		addToArray(1353); 
		addToArray(981); 
		addToArray(76);
		addToArray(5);

		addToArray(76); 
		addToArray(75);  
		addToArray(10); 
		addToArray(119);
		addToArray(4); 
		addToArray(47);  
		addToArray(9); 
		addToArray(112); 
		addToArray(1067);
		addToArray(5);

		addToArray(00); 
		addToArray(7);  
		addToArray(24); 
		addToArray(109);
		addToArray(225); 
		addToArray(176);  
		addToArray(587); 
		addToArray(89); 
		addToArray(28);
		addToArray(98);

		addToArray(25); 
		addToArray(68);  
		addToArray(32); 
		addToArray(57);
		addToArray(1300); 
		addToArray(62);  
		addToArray(65); 
		addToArray(365); 
		addToArray(73);
		addToArray(42);
	}
//        cout << "ENTERED SORT_ALL()\n";
        if(getSize() == 0)
                return;
	//	cout << "\n\t" << getArray() << "\n";
		recurse(0, index - 1);
		cout << "\n\t" << getArray() << "\n";
	//	recurse(0, index -1);
	//	cout << "\n\t" << getArray() << "\n";
		if(compare() == true)
			cout << "\t\t\t\tSUCESSEFULLY SORTED\n";
		else
			cout << "\t\t\t\tSORT FAILED\n";

}
//--------------------------------------------------------------------------------------
void QS::recurse(int left, int right)
{ 
//	cout << "\tRECURSION\n";
//	cout << "Left: " << left << "\tRight: " << right << endl;
//	cout << "\n\t" << getArray() << "\n";
	if((right - left) < 2) // BASE CASE OF ONLY 2 ELEMENTS
	{
		if(myArray[right] < myArray[left])
		{
			swap(left, right);
//			cout << "\tto switch the base case\n";
		}
		return;
	}
	int pivot = medianOfThree(left, right);
	int pivot2 = partition(left, right, pivot); // does this need to equal something
	recurse(left, pivot2);
	recurse(pivot2 + 1, right);
	return;

}
//--------------------------------------------------------------------------------------
int QS::medianOfThree(int left, int right)
{
 //       cout << "ENTERED MEDIAN OF 3()\n";
//		 cout << "Left V" << myArray[left] << "\tRight V" << myArray[right] << endl;
        if(left < 0 || right < 0 || left > getSize() || right > getSize()
                        || getSize() == 0 || left > right)
        { cout << "MEDIAN OF THREE NOT VALID\n";        return -1; }

        int pivotIndex = (right + left) / 2;
        while(myArray[left] > myArray[pivotIndex] || myArray[right] < myArray[pivotIndex])
        {
        	if(myArray[left] > myArray[pivotIndex])
        		swap(left, pivotIndex);
        	if(myArray[right] < myArray[pivotIndex])
        		swap(right, pivotIndex);
        }
//		cout << "Left " << left << "\tPivot " << pivotIndex << "\tRight " << right << endl;
 //       cout << "Left V" << myArray[left] << "\tPivot V" << myArray[pivotIndex] << "\tRight V" << myArray[right] << endl;
//		cout << "\n\t" << getArray() << "\n";
        return pivotIndex;
}
//--------------------------------------------------------------------------------------
int QS::partition(int left, int right, int pivotIndex)
{
 //       cout << "ENTERED PARTITION()\n";
        if(left < 0 || right < 0 || left > getSize() || right > getSize()|| getSize() == 0
                        || left > right || pivotIndex < left || pivotIndex > right)
        { cout << "PARTITION NOT VALID\n";      return -1; }
//		cout << "\t\t\tLeft " << left << "\tPivot " << pivotIndex << "\tRight " << right << endl;
        swap(pivotIndex, right);
//		cout << "\tto take pivot to end\n"; // TAKES PIVOT TO THE RIGHT
	//	cout << "\n\t" << getArray() << "\n";
        int i = 0;
        int j = (right - 1);

       while(j >= i)
       {
    	   while(myArray[i] < myArray[right])
    	   {
    		   i++;
    	   }
    	   while(myArray[j] >= myArray[right])
    	   {
    		   j--;
    	   }
//		   cout << "i " << myArray[i] << "\tj " << myArray[j] << "\tpivot " << myArray[right] << endl;
		   if(j >= i) {
    		swap(i, j); 
//			cout << " switch i and j for order\n"; 
		   } 
//			cout << "\n\t" << getArray() << "\n";
		//	if(myArray[i] == myArray[right])
		//		i++;
		//	if(myArray[j] == myArray[right])
		//		j--;
       }

        swap(i, right);
//		cout << "to put pivot back in its place\n";// switch i
		return i;
}
//--------------------------------------------------------------------------------------
string QS::getArray()
{
//	cout << "ENTERED GETARRAY()\n";
        if(getSize() == 0)
                return "";
        string toreturn;
        string temp;
     //  print();
        for(int i = 0; i < index; i++)
        {
        	stringstream convert;
        	convert << (myArray[i]);
        	convert >> temp;
        	if(toreturn.size() == 0)
        		toreturn += temp;
        	else
        		toreturn += "," + temp;
        }
//		cout << toreturn << endl;
        return toreturn;
}
//--------------------------------------------------------------------------------------
int QS::getSize()
{
//	cout << "ENTERED GETSIZE()\n";
        return Arraysize;
}
//--------------------------------------------------------------------------------------
void QS::addToArray(int value)
{
//	cout << "ENTERED ADDTOARRAY()\n";
//	int temp = index;
//	cout << "\tSIZE OF ARRAY\t" << getSize() << endl;
	if(getSize() == 0)
	{
		cout << "\tARRAY OF SIZE 0\n";
		return;
	}
	myArray[index] = value;
//	print();
	++index;
}
//--------------------------------------------------------------------------------------
bool QS::createArray(int size)
{
//	cout << "ENTERED CREATEARRAY()\n";
        if(size < 0)
                return false;
        myArray = new int[size]; // WHAT TYPE SHOULD IT BE?
        Arraysize = size;
        index = 0;
        return false;
}
//--------------------------------------------------------------------------------------
void QS::clear() // TA
{
//	cout << "ENTERED CLEAR()\n";
	delete[] myArray;
	myArray = NULL;
//	print();
	Arraysize = 0;

//	print();
//	myArray = NULL;
}
//--------------------------------------------------------------------------------------
void QS::print()
{
	for(int i = 0; i < index; i++)
	{
		cout << myArray[i] << " ";
	}
	cout << "\n";
}

void QS::swap(int ele1, int ele2)
{ 
	if(ele1 < 0 || ele2 > getSize() || ele2 < 0 || ele1 > getSize())
		return;
//	cout << "pos" << ele1 << " & pos" << ele2 << " switched\n";
	int temp = myArray[ele1];
	myArray[ele1] = myArray[ele2];
	myArray[ele2] = temp;
}

bool QS::compare()
{
	for(int i = 0; i < index - 1; i++)
	{
		if(myArray[i] > myArray[i+1])
			return false;
	}
	return true;
}



