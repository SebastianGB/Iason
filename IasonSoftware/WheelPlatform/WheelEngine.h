/*
 * WheelEngine.h
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#ifndef WHEELENGINE_H_
#define WHEELENGINE_H_

#include <math.h>
#include <stdlib.h>
#include "STMCommunicator.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


enum wheelPosition
{
	LEFT,
	RIGHT
};

class WheelEngine {
private:
	float _wheelRadius; // in meters
	float _wheelPerimeter; // in meters
	float _engineMaxRotationVelocity;
	int _escPWMPeriod; // in µs
	int _escPWMBackwardsMax; // PulseWith for maximum backward speed
	int _escPWMBackwardsMin;
	int _escPWMForwardsMax;
	int _escPWMForwardsMin;
	int _escPWMBreak;
	float _rotatingVelocity; // rotations per second
	float _linearVelocity; // in meters per second
	int _pulseWith;
	int _period;
	wheelPosition _position;
	uint16_t _sentUsartPwmValue;

	float _linToRot(float linearVelocity);
	float _rotToLin(float rotationVelocity);
	void _rotToPWM(float rotationVelocity, int *outPWMPeriod, int *outPWMPulseWidth);

public:
	float getLinearVelocity(){return _linearVelocity;}
	void setLinearVelocity(float value){_linearVelocity = value;}
	int getPulseWith(){return _pulseWith;}

	WheelEngine(wheelPosition position);
	int deploy(STMCommunicator *stmCom);
	int initializeEngine(STMCommunicator *stmCom);
	virtual ~WheelEngine();
};

#endif /* WHEELENGINE_H_ */
