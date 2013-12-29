/*
 * WheelPlatform.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#include "WheelPlatform.h"

WheelPlatform::WheelPlatform() {
	// TODO Auto-generated constructor stub
	gettimeofday (&_newSysTime, NULL);
	_oldSysTime = _newSysTime;
}

void WheelPlatform::update()
{
	//calculate elapsed time since last update
	gettimeofday (&_newSysTime, NULL);
	int diff;
	diff = (_newSysTime.tv_sec - _oldSysTime.tv_sec) * 1000000;
    diff += _newSysTime.tv_usec - _oldSysTime.tv_usec;
    _elapsedTime = ((float)diff)/1000000;
	_oldSysTime = _newSysTime;

	//set velocitys
	_linearVelocity += _linearAcceleration * _elapsedTime;
	_angularVelocity += _angularAcceleration * _elapsedTime;

	std::cout << _linearVelocity << "    " << _angularVelocity << "\n";
}

WheelPlatform::~WheelPlatform() {
	// TODO Auto-generated destructor stub
}

