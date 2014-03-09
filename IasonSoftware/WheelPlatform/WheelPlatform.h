/*
 * WheelPlatform.h
 *
 *  Created on: Dec 28, 2013
 *      Author: potato
 */

#ifndef WHEELPLATFORM_H_
#define WHEELPLATFORM_H_

#include <iostream>
#include <sys/time.h>
#include "WheelEngine.h"
#include "STMCommunicator.h"

class WheelPlatform {
private:
	float _currentAngularVelocity; // degree/s
	float _currentLinearVelocity; // m/s
	float _targetAngularVelocity; // degree/s
	float _targetLinearVelocity; // m/s
	WheelEngine *_engineLeft;
	WheelEngine *_engineRight;
	STMCommunicator _stmCom;
	struct timeval _oldSysTime;
	struct timeval _newSysTime;
	float _elapsedTime; // elapsed Time since last update() was called (in seconds)
	float _wheelBase; // m

	char _sentPulseWithLeft;
	char _sentPulseWithRight;

	int setWheelVelocities();

public:

	WheelPlatform();
	int update();
	virtual ~WheelPlatform();

	float getTargetAngularVelocity() const {
		return _targetAngularVelocity;
	}

	void setTargetAngularVelocity(float angularVelocity) {
		_targetAngularVelocity = angularVelocity;
	}

	float getTargetLinearVelocity() const {
		return _targetLinearVelocity;
	}

	void setTargetLinearVelocity(float linearVelocity) {
		_targetLinearVelocity = linearVelocity;
	}
};

#endif /* WHEELPLATFORM_H_ */
