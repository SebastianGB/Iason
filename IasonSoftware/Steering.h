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

	float _linearVelo; // m/s
	float _angularVelo;// Degree/s

	bool _positiveLinearVelo;
	bool _negativeLinearVelo;
	bool _positiveAngularVelo;
	bool _negativeAngularVelo;


public:
	typedef void (Steering::*splaMethod)();
	splaMethod splaMethodInstance;
	void steerPositiveLinearVelo(); // gets called automatically
	void steerNegativeLinearVelo(); // gets called automatically
	void steerPositiveAngularVelo(); // gets called automatically
	void steerNegativeAngularVelo(); // gets called automatically

	Steering(WheelPlatform *wheelPlatform, GCSCommunicator *communicator);
	void update();
	virtual ~Steering();
};

#endif /* STEERING_H_ */
