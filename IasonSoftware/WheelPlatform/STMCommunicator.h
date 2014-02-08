/*
 * STMCommunicator.h
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#ifndef STMCOMMUNICATOR_H_
#define STMCOMMUNICATOR_H_

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <iostream>
#include <sstream>

class STMCommunicator {
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

#endif /* STMCOMMUNICATOR_H_ */
