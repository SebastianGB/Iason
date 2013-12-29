/*
 * Steering.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#include "Steering.h"


Steering::Steering(WheelPlatform *wheelPlatform, GCSCommunicator *communicator) {
	// TODO Auto-generated constructor stub
	_wheelPlatform = wheelPlatform;
	_communicator = communicator;
	_linearAcceleration = 0.1f;
	_angularAcceleration = 2.0f;

	splaMethodInstance = &Steering::steerPositiveLinearAcceleration;
}

void Steering::update()
{
	float linAcc, angAcc;
	linAcc = angAcc = 0.f;

	if(_positiveLinearAcceleration)
	{
		linAcc += _linearAcceleration;

	}

	if(_negativeLinearAcceleration)
		linAcc -= _linearAcceleration;

	if(_positiveAngularAcceleration)
		angAcc += _linearAcceleration;
	if(_negativeAngularAcceleration)
		angAcc -= _linearAcceleration;

	_wheelPlatform->setAngularAcceleration(angAcc);
	_wheelPlatform->setLinearAcceleration(linAcc);
}

void Steering::steerPositiveLinearAcceleration()
{
	_positiveLinearAcceleration = _communicator->getComObjValue(STEERING_EVENT, KEY_UP);
	//update();
}
void Steering::steerNegativeLinearAcceleration()
{
	_negativeLinearAcceleration = _communicator->getComObjValue(STEERING_EVENT, KEY_DOWN);
	update();
}
void Steering::steerPositiveAngularAcceleration()
{
	_positiveAngularAcceleration = _communicator->getComObjValue(STEERING_EVENT, KEY_LEFT);
	update();
}
void Steering::steerNegativeAngularAcceleration()
{
	_negativeAngularAcceleration = _communicator->getComObjValue(STEERING_EVENT, KEY_RIGHT);
	update();
}

Steering::~Steering() {
	// TODO Auto-generated destructor stub
}

