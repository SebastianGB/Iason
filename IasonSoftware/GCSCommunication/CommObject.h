/*
 * CommObject.h
 *
 *  Created on: Dec 27, 2013
 *      Author: potato
 */
#include <iostream>
#include <list>


#ifndef COMMOBJECT_H_
#define COMMOBJECT_H_

//#include "../Steering.h"

class CommObject {
private:
	char value;
	void ((*realOnChange)());	//holds the function, added through "addOnChangeFunction()"
	void onChange();			//gets called whenever value is changed

public:
	int setValue(char newValue);
	char getValue();

	void addOnChangeFunction( void (*f)() );

	CommObject();
	virtual ~CommObject();
};

#endif /* COMMOBJECT_H_ */
