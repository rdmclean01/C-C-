//
//  Property.hpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#ifndef Property_h
#define Property_h

#include <string>

class Property {
	
private:
	
	static unsigned int s_nextPropertyID;
	
	const std::string address;
	const unsigned int propertyID;
	const bool rental;
	const double value;

	
public:
	
	Property(std::string address, bool isRental, double value);
	
	virtual double getPropertyTax() = 0;
	virtual std::string toString();
	
	const unsigned int getPropertyID();
	const std::string getAddress();
	const bool isRental();
	const double getValue();

};


#endif /* Property_h */
