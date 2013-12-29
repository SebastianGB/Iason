/*
 * GCSConnecton.h
 *
 *  Created on: Dec 26, 2013
 *      Author: potato
 */

#ifndef GCSCONNECTON_H_
#define GCSCONNECTON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <pthread.h>
#include <list>
#include <unistd.h>

#include "../defines.h"

#define MESSAGE_LENGTH 3

struct Message
{
	char group;
	char entity;
	char value;
};

class GCSConnecton {
private:
	int _sockfd;
public:
	std::list<Message> _msgin;
	std::list<Message> _msgout;
	pthread_mutex_t _msginMutex;
	pthread_mutex_t _msgoutMutex;

	GCSConnecton();

	///Starts the connection
	///newsockfd: the socket number
	int _start(int newsockfd);

	///Reads a message from the Connection
	///msg: the message
	///Returns the length of the message
	int _read(char *msg);

	///Writes a message to the Connection
    ///msg: the message
	int _write(char *msg, int length);

	///Closes the connection
	void _close();
	virtual ~GCSConnecton();
};

#endif /* GCSCONNECTON_H_ */
