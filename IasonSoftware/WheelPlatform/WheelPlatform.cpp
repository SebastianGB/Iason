/*
 * WheelPlatform.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#include "WheelPlatform.h"

WheelPlatform::WheelPlatform() {
	std::cout << "starting WheelPlatform"  << std::endl;
	gettimeofday (&_newSysTime, NULL);
	_oldSysTime = _newSysTime;

	_distOfEngineWheels = 0.55f;
	_stmCom.open_STM_board();
	usleep(10000);

	_engineLeft = new WheelEngine(LEFT);
	_engineRight = new WheelEngine(RIGHT);
	_engineLeft->initializeEngine(&_stmCom);
	_engineRight->initializeEngine(&_stmCom);

	std::cout << "WheelPlatform created"  << std::endl;
}

int WheelPlatform::controlWheels()
{
	_engineLeft->setLinearVelocity(
		_linearVelocity - ((_angularVelocity*M_PI*_distOfEngineWheels)/360)
		);

	_engineRight->setLinearVelocity(
		_linearVelocity + ((_angularVelocity*M_PI*_distOfEngineWheels)/360)
		);

	int errorstateLeft = _engineLeft->deploy(&_stmCom);
	int errorstateRight = _engineRight->deploy(&_stmCom);
	if(	errorstateLeft != 0
	 || errorstateRight != 0)
	{
		std::cout << "Engine Left is in Errorstate: " << errorstateLeft << std::endl;
		std::cout << "Engine Right is in Errorstate: " << errorstateRight << std::endl;
		return -1;
	}
	else
	{
		return 0;
	}

}

int WheelPlatform::update()
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

	//std::cout << _linearVelocity << "    " << _angularVelocity << "\n";

	return controlWheels();
}

WheelPlatform::~WheelPlatform() {
	// TODO Auto-generated destructor stub
}

