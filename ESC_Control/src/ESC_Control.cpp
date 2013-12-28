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

#include <iostream>
#include <sstream>


using namespace std;

int fd1;
int fd2;
int numberWrittenBytes, nbytes, tries;
int numberReadBytes;

//TODO: check terminos flags
int configure_port(int fd)      // configure the port
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

int main() {

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

//	With following code we send to device some bitsiostream:

//	And for reading response from device:
	char readBuffer[10];
	char run = 1;
	string consoleInput;
	while(run){

		cin >> consoleInput;
		cout << "Console input is: " << consoleInput << endl;

		uint16_t pwmWidth;
		istringstream buffer(consoleInput);
		buffer >> pwmWidth;

		cout << "pwmWidth: " << pwmWidth << endl;

		cout << "Try to send pulse width ..." << endl;

		char writeBytes[1];
		memcpy((void *)writeBytes, (void*)&pwmWidth, 1);

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

		cout << "Try to receive byte ...\n";

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

		uint8_t receivedPWMWidth = 0;
		memcpy((void *)&receivedPWMWidth, (void*)readBuffer, 1);

		if(receivedPWMWidth == pwmWidth){
			cout << "Same value!" << endl;
		} else {
			cout << "Not the same value! receivedPWMWidth: " << (uint)(receivedPWMWidth) << endl;
		}
	}

//	At the end, close the connection:
	close(fd1);
	return 0;
}

//TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

