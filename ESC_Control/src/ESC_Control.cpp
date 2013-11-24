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
	char buff[10];
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

		numberWrittenBytes = 0;
		int currentWritten = 0;
		while(numberWrittenBytes < 1){
			currentWritten = write(fd1, (void*)&(writeBytes[numberWrittenBytes]), 1 - numberWrittenBytes);
			numberWrittenBytes += currentWritten;
			usleep(1000000);
			cout << "write-check" << endl;
		}

		cout << "Try to receive byte ..." << endl;

		numberReadBytes = 0;
		int numRead = 0;
		while(numberReadBytes < 1){
			numRead = read(fd1, (void*)&(buff[numberReadBytes]), 1 - numberReadBytes);
			numberReadBytes += numRead;
			usleep(1000);
			cout << "read-check" << endl;
		}

		cout << "Bytes read are %i: " << numberReadBytes << endl;

		uint8_t receivedPWMWidth = 0;
		memcpy((void *)&receivedPWMWidth, (void*)buff, 1);

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

