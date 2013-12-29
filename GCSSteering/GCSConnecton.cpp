/*
 * GCSConnecton.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: potato
 */

#include "GCSConnecton.h"

void *inputThread(void *conn)
{
	GCSConnecton *connection;
	connection = (GCSConnecton*)conn;

	Message msg;
	char buffer[MESSAGE_LENGTH];

	while(connection)
	{
		if(!connection->_read(buffer)) // a huge part of the time the thread spends waiting for this line
		{
#ifdef LOG_OUTPUT
			std::cout << "error reading... quit connection";
#endif
			connection->~GCSConnecton();
			return 0;  // its important to kill the thread! otherwise it will kill the cpu and ram reading billions of '0'.
		}
		msg.group = buffer[0];
		msg.entity = buffer[1];
		msg.value = buffer[2];
		pthread_mutex_lock(&connection->_msginMutex);
		connection->_msgin.push_front(msg);
		pthread_mutex_unlock(&connection->_msginMutex);
	}
	return 0;
}

void *outputTread(void *conn)
{
	GCSConnecton *connection;
	connection = (GCSConnecton*)conn;

	char buffer[MESSAGE_LENGTH];
	Message msg;

    while(connection)
    {
    	pthread_mutex_lock(&connection->_msgoutMutex);
    	while(!connection->_msgout.empty())
    	{
    		msg = connection->_msgout.back();
    		connection->_msgout.pop_back();
    		buffer[0] = msg.group;
    		buffer[1] = msg.entity;
    		buffer[2] = msg.value;
    		if(!connection->_write(buffer, MESSAGE_LENGTH))
    		{
    				connection->~GCSConnecton();
    				return 0;  // its important to kill the thread!
    		}
    	}
    	pthread_mutex_unlock(&connection->_msgoutMutex);
    	usleep(100);
    }
	return 0;
}

GCSConnecton::GCSConnecton() {
	// TODO Auto-generated constructor stub

}

int GCSConnecton::_start(int newsockfd)
{
	_sockfd = newsockfd;

	pthread_t threads[2];
	int rc = pthread_create(&threads[0], NULL,inputThread, (void *)this);
	if (rc){
		std::cout << "Error:unable to create thread," << rc << std::endl;
		return 1;
	}
	rc = pthread_create(&threads[1], NULL,outputTread, (void *)this);
	if (rc){
		 std::cout << "Error:unable to create thread," << rc << std::endl;
		 return 1;
	}
	return 0;
}

int GCSConnecton::_read(char *msg)
{
	bzero(msg,MESSAGE_LENGTH);
	return read(_sockfd,msg,MESSAGE_LENGTH);
}

int GCSConnecton::_write(char *msg, int length)
{
	return write(_sockfd, msg, length);
}

void GCSConnecton::_close()
{
	close(_sockfd);
	pthread_exit(NULL);
}

GCSConnecton::~GCSConnecton() {
	// TODO Auto-generated destructor stub
}

