//
//  Property.cpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#include "Property.h"
#include <sstream>

using namespace std;

unsigned int Property::s_nextPropertyID = 0;


Property::Property(string address, bool isRental, double value):
	address(address),
	propertyID(s_nextPropertyID++),
	rental(isRental),
	value(value) {
		
		
}


const string Property::getAddress() {
	return address;
}

string Property::toString() {
	
	stringstream stringStream;
	
	stringStream << "Property id: " << getPropertyID() << "\t";
	stringStream << "Address: " << getAddress() << "\t";
	if (isRental()) {
		stringStream << "Rental\t";
	}
	else {
		stringStream << "NOT Rental\t";
	}
	stringStream << "Estimated Value: " << getValue();

	
	return stringStream.str();
}

const unsigned int Property::getPropertyID() {
	return propertyID;
}

const bool Property::isRental() {
	return rental;
}

const double Property::getValue() {
	return value;
}