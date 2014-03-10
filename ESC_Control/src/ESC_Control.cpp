/*
 ============================================================================
 Name        : SerialTest.c
 Author      :
 Version     :
 Copyright   : Your copyright noticeiostream
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>

#include <math.h>

#include <iostream>
#include <sstream>

using namespace std;

float _wheelRadius = 0.27f;
float _wheelPerimeter = 2*_wheelRadius*M_PI;
float _engineMaxRotationVelocity = 2;
//float _escPWMPeriod = 10000;
float _escPWMBackwardsMax = 1000;
float _escPWMBackwardsMin = 1400;
float _escPWMForwardsMax = 2000;
float _escPWMForwardsMin = 1600;
float _escPWMBreak = 1500;
float _rotationVelocity = 0.0f;
float _linearVelocity = 0.0f;
float _sentUsartPwmValue = -1;

uint16_t pwmToUsartValue(uint16_t _pulseWith){
	uint16_t usartPwmValue;
	const uint16_t one = 1;
	//stop case
	if(_pulseWith >= _escPWMBackwardsMin && _pulseWith <= _escPWMForwardsMin)
	{
		usartPwmValue = 0;
//		usartPwmValue |= (one << 6);
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

	//for choosing wheel
//	usartPwmValue |= (one << 7);

    if(usartPwmValue == 13)
            usartPwmValue = 12;
    if(usartPwmValue == 10)
            usartPwmValue = 9;

    return usartPwmValue;
}


class STMCommunicator{

public:
	STMCommunicator();
	~STMCommunicator();
	int configure_port(int fd);
	int open_STM_board();
	int writeByte(uint16_t pwmWidth);

private:

	int fd1;
	int numberWrittenBytes;
	int numberReadBytes;

	char readBuffer[10];

};

STMCommunicator::STMCommunicator(){

}

STMCommunicator::~STMCommunicator(){
	close(fd1);
}

int STMCommunicator::configure_port(int fd)      // configure the port
{
	struct termios port_settings;      // structure tiostreamo store the port settings in
	memset(&port_settings, 0, sizeof(port_settings));

	cfsetispeed(&port_settings, B38400);    // set baud rates
	cfsetospeed(&port_settings, B38400);

//	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
//	port_settings.c_cflag &= ~CSTOPB;
//	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cc[VMIN] = 1;
	port_settings.c_cc[VTIME] = 5;
//	port_settings.c_cflag |= CS8;

	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
	return(fd);

}

int STMCommunicator::open_STM_board(){
	fd1 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd1 == -1){
		perror("open_port: Unable to open /dev/ttyUSB0 – ");
	} else {
		fcntl(fd1, F_SETFL, 0);
		printf("Successfully opened serial connection and %d is the file description\n",
				fd1);
		fflush(stdout);
	}

	fd1 = configure_port(fd1);
	fcntl(fd1, F_SETFL, O_NONBLOCK);

	return 1;
}

int STMCommunicator::writeByte(uint16_t byteToWrite){

	char writeBytes[1];
	memcpy((void *)writeBytes, (void*)&byteToWrite, 1);

	bool byteWritten = false;
	while(!byteWritten){
		usleep(1000);
		int numberBytesWritten = write(fd1, (void*)&(writeBytes[0]), 1);
		if(numberBytesWritten == 1){
			byteWritten = true;
		} else {
			cout << "numberBytesWritten byte is smaller zero: " << numberBytesWritten << endl;
			return -1;
		}
		cout << "write-check ... number bytes written: " << numberBytesWritten << endl;
	}

	cout << "Try to receive acknowledge byte ...\n";

	bool byteRead = false;
	while(!byteRead){
		usleep(10000);
		int numberBytesRead = read(fd1, (void*)&(readBuffer[0]), 1);
		if(numberBytesRead > 1){
			cout << "read more than 2 bytes at the same time!" << endl;
			return -1 ;
		}
		else if(numberBytesRead < 0){
			cout << "############# error during read ################" << endl;
			continue;
		} else if(numberBytesRead == 1){
			byteRead = true;
		}
		if(readBuffer[0] == 10 || readBuffer[0] == 13){
			cout << "Stop/Start Byte received!" << endl;
			byteRead = false;
		}
		cout << "read-check; numRead is " << numberBytesRead << endl;
	}

	uint8_t receivedAcknowledgeByte = 0;
	memcpy((void *)&receivedAcknowledgeByte, (void*)readBuffer, 1);

	if(receivedAcknowledgeByte == byteToWrite){
		cout << "Same value!" << endl;
	} else {
		cout << "Not the same value! receivedByte: " << (uint)(receivedAcknowledgeByte) << endl;
	}

	return 0;

}


int main() {

	STMCommunicator mySTMCommunicator;

	mySTMCommunicator.open_STM_board();

	char run = 1;
	string consoleInput;
	while(run){

		cin >> consoleInput;
		cout << "Console input is: " << consoleInput << endl;

		uint16_t pwmWidth;
		istringstream buffer(consoleInput);
		buffer >> pwmWidth;
		cout << "PWM value after conversion: " << pwmWidth << endl;
		pwmWidth = pwmToUsartValue(pwmWidth);
		cout << "Try to send usartPwmValue ..." << pwmWidth << endl;

//		uint16_t pwmWidth;
//		istringstream buffer(consoleInput);
//		buffer >> pwmWidth;
//		cout << "Try to send pulse width ..." << pwmWidth << endl;

		mySTMCommunicator.writeByte(pwmWidth);

	}

	return 0;
}

