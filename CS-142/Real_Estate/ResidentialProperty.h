//
//  ResidentialProperty.hpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#ifndef ResidentialProperty_h
#define ResidentialProperty_h

#include "Property.h"

class ResidentialProperty: public Property {
	
private:
	
	const bool occupied;
	
public:
	
	ResidentialProperty(std::string address, bool isRental, double value, bool occupied);
	
	std::string toString();
	double getPropertyTax();
	
	const bool isOccupied();
	
};

#endif /* ResidentialProperty_h */
