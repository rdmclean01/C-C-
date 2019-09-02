//
//  CommercialProperty.hpp
//  CS142-Lab10
//
//  Created by Landon Smith on 1/7/16.
//  Copyright © 2016 Landon Smith. All rights reserved.
//

#ifndef CommercialProperty_h
#define CommercialProperty_h

#include "Property.h"

class ComercialProperty: public Property {
	
private:
	const bool discounted;
	const double discount;
	
public:
	
	ComercialProperty(std::string address, bool isRental, double value, bool isDiscounted, double discount);
	
	std::string toString();
	double getPropertyTax();
	
	const bool isDiscounted();
	const double getDiscount();
};



#endif /* CommercialProperty_h */
