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

class WheelPlatform {
private:
	float _angularVelocity; // degree/s
	float _linearVelocity; // m/s
	float _angularAcceleration; // degree/s²
	float _linearAcceleration; // m/s²
	WheelEngine _engine;
	struct timeval _oldSysTime;
	struct timeval _newSysTime;
	float _elapsedTime; // elapsed Time since last update() was called (in seconds)

public:

	WheelPlatform();
	void update();
	virtual ~WheelPlatform();

	float getAngularVelocity() const {
		return _angularVelocity;
	}

	void setAngularVelocity(float angularVelocity) {
		_angularVelocity = angularVelocity;
	}

	float getLinearVelocity() const {
		return _linearVelocity;
	}

	void setLinearVelocity(float linearVelocity) {
		_linearVelocity = linearVelocity;
	}

	float getAngularAcceleration() const {
		return _angularAcceleration;
	}

	void setAngularAcceleration(float angularAcceleration) {
		_angularAcceleration = angularAcceleration;
	}

	float getLinearAcceleration() const {
		return _linearAcceleration;
	}

	void setLinearAcceleration(float linearAcceleration) {
		_linearAcceleration = linearAcceleration;
	}
};

#endif /* WHEELPLATFORM_H_ */
