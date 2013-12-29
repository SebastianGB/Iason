/*
 * CommObject.cpp
 *
 *  Created on: Dec 27, 2013
 *      Author: potato
 */

#include "CommObject.h"

CommObject::CommObject() {
	// TODO Auto-generated constructor stub
	value = 0;
	realOnChange = NULL;
}

void CommObject::addOnChangeFunction( void (*f)() )
{
	realOnChange = f;
}

void CommObject::onChange()
{
	if(realOnChange != NULL)
	{
		(*realOnChange)();
	}
}

int CommObject::setValue(char newValue)
{
	if(value == newValue)
		return 1;
	else
	{
		value = newValue;
		onChange();
		return 0;
	}
}

char CommObject::getValue()
{
	return value;
}

CommObject::~CommObject() {
	// TODO Auto-generated destructor stub
}

