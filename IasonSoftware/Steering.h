/*
 * Steering.h
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#ifndef STEERING_H_
#define STEERING_H_

#include "WheelPlatform/WheelPlatform.h"
#include "GCSCommunication/GCSCommunicator.h"

class Steering {
	WheelPlatform *_wheelPlatform;
	GCSCommunicator *_communicator;

	float _linearAcceleration, _angularAcceleration;

	bool _positiveLinearAcceleration;
	bool _negativeLinearAcceleration;
	bool _positiveAngularAcceleration;
	bool _negativeAngularAcceleration;


public:
	typedef void (Steering::*splaMethod)();
	splaMethod splaMethodInstance;
	void steerPositiveLinearAcceleration(); // gets called automatically
	void steerNegativeLinearAcceleration(); // gets called automatically
	void steerPositiveAngularAcceleration(); // gets called automatically
	void steerNegativeAngularAcceleration(); // gets called automatically

	Steering(WheelPlatform *wheelPlatform, GCSCommunicator *communicator);
	void update();
	virtual ~Steering();
};

#endif /* STEERING_H_ */
