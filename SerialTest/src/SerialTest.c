/*
 ============================================================================
 Name        : SerialTest.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int fd1;
int fd2;
int wr, nbytes, tries;
int rd;

int configure_port(int fd)      // configure the port
{
	struct termios port_settings;      // structure to store the port settings in

	cfsetispeed(&port_settings, B38400);    // set baud rates
	cfsetospeed(&port_settings, B38400);

//	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
//	port_settings.c_cflag &= ~CSTOPB;
//	port_settings.c_cflag &= ~CSIZE;
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
	}

	fd1 = configure_port(fd1);
	fcntl(fd1, F_SETFL, O_NONBLOCK);

//	With following code we send to device some bits:

	char message[1];
	message[0] = 7;
	printf("%i\n", (int)(message[0]));

//	And for reading response from device:
	char buff[10];
	int k;
	for(k = 0; k < 5; k++){
//		printf("Try to send byte ...\n");
		fflush(stdout);
		wr = write(fd1, message, 1);

//		printf("Try to receive byte ...\n");
		fflush(stdout);

		rd = read(fd1, (void*)buff, 10);

		if(rd == -1){
			k--;
			usleep(100000);
			continue;
		}

		printf("Bytes read are %i: ", rd);
		fflush(stdout);

		int i;
		for(i = 0; i < rd; i++){
			printf("%i \n", buff[i]);
		}
		printf("\n");
	}

//	At the end, close the connection:

	close(fd1);
	return 0;
}

//TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;


