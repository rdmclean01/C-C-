//
//  main.cpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "ResidentialProperty.h"
#include "CommercialProperty.h"
#include <vector>
#include <sstream>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::ifstream;
using std::endl;
using std::stringstream;

vector<Property*> propertyVector;


ResidentialProperty* parseResidentialProperty(stringstream* inStream);
ComercialProperty* parseCommercialProperty(stringstream* inStream);
bool parseBoolFlag(int boolFlag);

// Returns true if loaded any valid properties
bool parseInputFile(ifstream* inputStream);
void printTaxReport();


int main(int argc, const char * argv[]) {
	
	string filename;
	
	cout << "Please enter file name: ";
	cin >> filename;
	
	ifstream inputStream;
	
	inputStream.open(filename);
	
	if (inputStream.is_open()) {
		
		bool parseSuccessful = parseInputFile(&inputStream);
		
		inputStream.close();
		
		if (parseSuccessful) {
			cout << endl << "All valid properties:" << endl << endl;
			
			for (int i = 0; i < propertyVector.size(); i++) {
				cout << propertyVector.at(i)->toString() << endl;
			}
			
			printTaxReport();
		}
		else {
			cout << "No valid properties found" << endl;
		}
		
	}
	else {
		cout << "File could not be opened" << endl;
	}
	
	system("pause");
    return 0;
}


bool parseInputFile(ifstream* inputStream) {
	
	
	int addedProperties = 0;
	
	while (!inputStream->eof()) {
		
		string line;
		
		// Feed algorithm one full line as a seperate stringstream so it can find eol for each line
		getline(*inputStream, line);
		
		try {
			
			stringstream lineStream(line);
			string type;
			
			lineStream >> type;
			
			if (type == "Residential") {
				
				try {
					
					propertyVector.push_back(parseResidentialProperty(&lineStream));
					addedProperties++;
					
				} catch (...) {
					cout << "Ignoring bad RESIDENTIAL property in file: " << line << endl;
				}
				
			}
			else if (type == "Commercial") {
				
				try {
					
					propertyVector.push_back(parseCommercialProperty(&lineStream));
					addedProperties++;
					
				} catch (...) {
					cout << "Ignoring bad COMMERCIAL property in file: " << line << endl;
				}
			}
			// Report empty line
			else if (type == "") {
				
				//  Unless it's just a trailing line at end of file
				if (!inputStream->eof()) {
					cout << "Ignoring empty line from file" << endl;
				}
			}
			else {
				cout << "Ignoring invalid property type: " << line << endl;
			}
		} catch(...) {
			cout << "Ignoring bad input from file: " << line << endl;
		}
	}
	return addedProperties != 0;
}



void printTaxReport() {
	
	cout << "NOW PRINTING TAX REPORT:" << endl << endl;
	
	for (int i = 0; i < propertyVector.size(); i++) {
		
		Property* currentProperty = propertyVector.at(i);
		
		cout << "** Taxes due for the property at:\t\t" << currentProperty->getAddress() << endl;
		cout << "Property id:\t\t\t\t\t" << currentProperty->getPropertyID() << endl;
		cout << "This property has an estimated value of:\t" << currentProperty->getValue() << endl;
		cout << "Taxes due on this property are:\t\t\t" << currentProperty->getPropertyTax() << endl << endl;
	}
	
	
}




ResidentialProperty* parseResidentialProperty(stringstream* inStream) {
	
	int flag;
	double value;
	string address;
	
	
	*inStream >> flag;
	
	bool isRental = parseBoolFlag(flag);

	*inStream >> value;
	
	if (inStream->fail()) {
		throw "Failed to read value";
	}
	
	*inStream >> flag;
	
	bool isOccupied = parseBoolFlag(flag);
	
	string wordOfAddress;
	
	while (!inStream->eof()) {
		*inStream >> wordOfAddress;
		if (inStream->fail()) {
			// Certain file encodings don't trigger eof until after a bad read. Sanity check
			if (inStream->eof()) {
				break;
			}
			throw "Failed reading address";
		}
		address += wordOfAddress + " ";
	}
	
	if (address == "" || address == " ") {
		throw "No Address";
	}
	
	return new ResidentialProperty(address, isRental, value, isOccupied);
}





ComercialProperty* parseCommercialProperty(stringstream* inStream) {
	
	int flag;
	
	double value;
	double discount;
	string address;
	
	*inStream >> flag;
	
	bool isRental = parseBoolFlag(flag);
	
	*inStream >> value;
	*inStream >> flag;
	
	bool isDiscounted = parseBoolFlag(flag);

	*inStream >> discount;
	
	if (inStream->fail()) {
		throw "Failed to read value";
	}
	
	string wordOfAddress;
	
	while (!inStream->eof()) {
		*inStream >> wordOfAddress;
		if (inStream->fail()) {
			// Certain file encodings don't trigger eof until after a bad read. Sanity check
			if (inStream->eof()) {
				break;
			}
			throw "Failed reading address";
		}
		address += wordOfAddress + " ";
	}
	
	if (address == "" || address == " ") {
		throw "No Address";
	}
	
	return new ComercialProperty(address, isRental, value, isDiscounted, discount);
}

bool parseBoolFlag(int boolFlag) {
	if (boolFlag == 0) {
		return false;
	}
	else if (boolFlag == 1) {
		return true;
	}
	else {
		throw "Expected 1 or 0";
	}
}



