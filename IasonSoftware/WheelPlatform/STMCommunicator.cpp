/*
 * STMCommunicator.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#include "STMCommunicator.h"

using namespace std;

STMCommunicator::STMCommunicator() {
	// TODO Auto-generated constructor stub

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
		usleep(10000);
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
			//TODO handle special error case else print nothing..
			if(!errno == 11){ //temporarily unvailable case
				cout << "read error: " << errno;
				char buffer[256];
				char * errorMessage = strerror_r(errno, buffer, 256);
				cout << errorMessage << endl;
				//return -1;
			}
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

