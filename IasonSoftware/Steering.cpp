/*
 * Steering.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#include "Steering.h"


Steering::Steering(WheelPlatform *wheelPlatform, GCSCommunicator *communicator) {
	_wheelPlatform = wheelPlatform;
	_communicator = communicator;
	_linearVelo = 2.5f;
	_angularVelo = 90.0f;

	splaMethodInstance = &Steering::steerPositiveLinearVelo;

	std::cout << "Steering created"  << std::endl;
}

void Steering::update()
{
	float linVelo, angVelo;
	linVelo = angVelo = 0.f;

	if(_positiveLinearVelo && !_negativeLinearVelo)
	{
		linVelo = _linearVelo;
	}
	else if(_negativeLinearVelo && !_positiveAngularVelo)
	{
		linVelo = -_linearVelo;
	}

	if(_positiveAngularVelo && !_negativeAngularVelo)
	{
		angVelo = _angularVelo;
	}
	if(_negativeAngularVelo && !_positiveAngularVelo)
	{
		angVelo = -_angularVelo;
	}

	_wheelPlatform->setTargetAngularVelocity(angVelo);
	_wheelPlatform->setTargetLinearVelocity(linVelo);
}

void Steering::steerPositiveLinearVelo()
{
	_positiveLinearVelo = _communicator->getComObjValue(STEERING_EVENT, KEY_UP);
	//update();
}
void Steering::steerNegativeLinearVelo()
{
	_negativeLinearVelo = _communicator->getComObjValue(STEERING_EVENT, KEY_DOWN);
	update();
}
void Steering::steerPositiveAngularVelo()
{
	_positiveAngularVelo = _communicator->getComObjValue(STEERING_EVENT, KEY_LEFT);
	update();
}
void Steering::steerNegativeAngularVelo()
{
	_negativeAngularVelo = _communicator->getComObjValue(STEERING_EVENT, KEY_RIGHT);
	update();
}

Steering::~Steering() {
	// TODO Auto-generated destructor stub
}

