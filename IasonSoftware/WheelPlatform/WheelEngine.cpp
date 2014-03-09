/*
 * WheelEngine.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#include "WheelEngine.h"

WheelEngine::WheelEngine(wheelPosition position) {
	// TODO Auto-generated constructor stub
	_position = position;
	_wheelRadius = 0.27f;
	_wheelPerimeter = 2*_wheelRadius*M_PI;
	_engineMaxRotationVelocity = 2;
	_escPWMPeriod = 20000;
	_escPWMBackwardsMax = 1000;
	_escPWMBackwardsMin = 1400;
	_escPWMForwardsMax = 2000;
	_escPWMForwardsMin = 1600;
	_escPWMBreak = 1500;
	_rotatingVelocity = 0.0f;
	_linearVelocity = 0.0f;
	_sentUsartPwmValue = -1;

}

int WheelEngine::initializeEngine(STMCommunicator *stmCom)
{
	return deploy(stmCom);
}

float WheelEngine::_linToRot(float linearVelocity)
{
	return linearVelocity/_wheelPerimeter;
}
float WheelEngine::_rotToLin(float rotationVelocity)
{
	return _wheelPerimeter*rotationVelocity;
}
void WheelEngine::_rotToPWM(float rotatingVelocity, int *outPWMPeriod, int *outPWMPulseWidth)
{
	(*outPWMPeriod) = _escPWMPeriod;
	// break
	if(fabs(rotatingVelocity) < 0.1f)
	{
		(*outPWMPulseWidth) = _escPWMBreak;
	}
	else
	{
		//forwards
		if(rotatingVelocity > 0)
		{
			(*outPWMPulseWidth) = _escPWMForwardsMin + rotatingVelocity*(_escPWMForwardsMax-_escPWMForwardsMin)/(float)_engineMaxRotationVelocity;
			//bounding
			if((*outPWMPulseWidth) > _escPWMForwardsMax)
			{
				(*outPWMPulseWidth) = _escPWMForwardsMax;
			}
		}
		//backwards
		if(rotatingVelocity < 0)
		{
			(*outPWMPulseWidth) = _escPWMBackwardsMin + rotatingVelocity*(_escPWMBackwardsMin-_escPWMBackwardsMax)/(float)_engineMaxRotationVelocity;
			//bounding
			if((*outPWMPulseWidth) < _escPWMBackwardsMax)
			{
				(*outPWMPulseWidth) = _escPWMBackwardsMax;
			}
		}
	}

}

int WheelEngine::deploy(STMCommunicator *stmCom)
{
	_rotatingVelocity = _linToRot(_linearVelocity);

	std::cout << "Wheel: " << _position << "lin V: " << _linearVelocity << std::endl;
	std::cout << "Wheel: " << _position << "rot V: " << _rotatingVelocity << std::endl;

	_rotToPWM(_rotatingVelocity,&_period,&_pulseWith);
	std::cout << "Wheel: " << _position << "pwm: " << _pulseWith << std::endl;



	uint16_t usartPwmValue;
	const uint16_t one = 1;
	//stop case
	if(_pulseWith >= _escPWMBackwardsMin && _pulseWith <= _escPWMForwardsMin)
	{
		usartPwmValue = 0;
		usartPwmValue |= (one << 6);
	}
	//backward
	else if(_pulseWith <= _escPWMBackwardsMin)
	{
		usartPwmValue = 63*(_escPWMBackwardsMin - _pulseWith)/(float)400;
		// check bound
		if(usartPwmValue >= (one << 6))
		{
			usartPwmValue = (one << 6) -1;
		}
	}
	//forward
	else if(_pulseWith >= _escPWMForwardsMin)
	{
		usartPwmValue = 63*(_pulseWith - _escPWMForwardsMin)/(float)400;
		//check bound
		if(usartPwmValue >= (one << 6))
		{
			usartPwmValue = (one << 6) -1;
		}
		usartPwmValue |= (one << 6);
	}

	if(_position == RIGHT)
		usartPwmValue |= (one << 7);


	if(usartPwmValue == 13)
		usartPwmValue = 12;
	if(usartPwmValue == 10)
		usartPwmValue = 9;
	if(usartPwmValue != _sentUsartPwmValue)
	{
		_sentUsartPwmValue = usartPwmValue;
		if(stmCom->writeByte(usartPwmValue) != 0)
		{
			std::cout << "(WheelEngine): Write to PWM failed" << std::endl;
			return -1;
		}

		std::cout << "Try to send pulse width:" << _pulseWith << " sending: " << usartPwmValue << std::endl;
	}
	return 0;
}

WheelEngine::~WheelEngine() {
	// TODO Auto-generated destructor stub
}

