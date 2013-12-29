/*
 * WheelEngine.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#include "WheelEngine.h"

WheelEngine::WheelEngine() {
	// TODO Auto-generated constructor stub
	_wheelRadius = 0.27f;
	_wheelPerimeter = 2*_wheelRadius*M_PI;
	_engineMaxRotationVelocity = 2;
	_escPWMPeriod = 20000;
	_escPWMBackwardsMax = 1000;
	_escPWMBackwardsMin = 1400;
	_escPWMForwardsMax = 2000;
	_escPWMForwardsMin = 1600;
	_escPWMBreak = 1500;
	_rotationVelocity = 0.0f;
	_linearVelocity = 0.0f;
}

float WheelEngine::_linToRot(float linearVelocity)
{
	return linearVelocity/_wheelPerimeter;
}
float WheelEngine::_rotToLin(float rotationVelocity)
{
	return _wheelPerimeter*rotationVelocity;
}
void WheelEngine::_rotToPWM(float rotationVelocity, int *outPWMPeriod, int *outPWMPulseWidth)
{
	(*outPWMPeriod) = _escPWMPeriod;
	// break
	if(rotationVelocity == 0)
	{
		(*outPWMPulseWidth) = _escPWMBreak;
	}

	//forwards
	if(rotationVelocity > 0)
	{
		(*outPWMPulseWidth) = _escPWMForwardsMin + rotationVelocity*(_escPWMForwardsMax-_escPWMForwardsMin)/(float)_engineMaxRotationVelocity;
	}
	//backwards
	if(rotationVelocity < 0)
	{
		(*outPWMPulseWidth) = _escPWMBackwardsMin + rotationVelocity*(_escPWMBackwardsMin-_escPWMBackwardsMax)/(float)_engineMaxRotationVelocity;
	}

}

void WheelEngine::getPWMOutput(int *outPWMPeriod, int *outPWMPulseWidth)
{
	_rotationVelocity = _linToRot(_linearVelocity);
	_rotToPWM(_rotationVelocity,&_period,&_pulseWith);
	(*outPWMPeriod) = _period;
	(*outPWMPulseWidth) = _pulseWith;
}

WheelEngine::~WheelEngine() {
	// TODO Auto-generated destructor stub
}

