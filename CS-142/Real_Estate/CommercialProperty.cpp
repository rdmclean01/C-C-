//
//  CommercialProperty.cpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#include "CommercialProperty.h"
#include <sstream>

using std::stringstream;
using std::string;


#define _rentalTax .012
#define _nonRentalTax .01

ComercialProperty::ComercialProperty(string address, bool isRental, double value, bool isDiscounted, double discount):
	Property(address, isRental, value),
	discounted(isDiscounted),
	discount(discount) {
		
		
		
}


double ComercialProperty::getPropertyTax() {
	
	double tax;
	
	double discount = 0;
	
	if (isDiscounted())
	{
		discount = getDiscount();
	}
	
	// Discount is a percentage, calculate remaining percentage of 100% and multiply
	if (isRental()) {
		tax = getValue() * _rentalTax * (1 - discount);
	}
	else {
		tax = getValue() * _nonRentalTax * (1 - discount);
	}
	
	return tax;
}

std::string ComercialProperty::toString() {
	
	stringstream stringStream;
	
	// Use toString from base class as starting point
	stringStream << Property::toString();
	stringStream << "\t";
	
	if (isDiscounted()) {
		stringStream << "Discounted" << "\t";
		stringStream << "Discount " << getDiscount();
	}
	else {
		stringStream << "NOT Discounted";
	}
	
	return stringStream.str();

}

const bool ComercialProperty::isDiscounted() {
	return discounted;
}

const double ComercialProperty::getDiscount() {
	return discount;
}