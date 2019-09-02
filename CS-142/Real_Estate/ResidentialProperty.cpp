//
//  ResidentialProperty.cpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#include "ResidentialProperty.h"

#include <sstream>

#define _occupiedTax .006
#define _unoccupiedTax .009

using std::stringstream;
using std::string;


ResidentialProperty::ResidentialProperty(string address, bool isRental, double value, bool occupied):
	Property(address, isRental, value),
	occupied(occupied) {
	
	
}


double ResidentialProperty::getPropertyTax() {
	
	double tax;
	
	if (isOccupied()) {
		tax = getValue() * _occupiedTax;
	}
	else {
		tax = getValue() * _unoccupiedTax;
	}
	return tax;
}

string ResidentialProperty::toString() {
	
	stringstream stringStream;
	
	// Use toString from base class as starting point
	stringStream << Property::toString();
	
	 stringStream << "\t";
	
	if (isOccupied()) {
		stringStream << "Occupied";
	}
	else {
		stringStream << "NOT Occupied";
	}
	
	return stringStream.str();
}

const bool ResidentialProperty::isOccupied() {
	return occupied;
}