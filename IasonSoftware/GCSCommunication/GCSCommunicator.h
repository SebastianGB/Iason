/*
 * GCSCommunicator.h
 * This Class handles the communication to the GCS (Ground Control Station)
 * It uses tcp-sockets to communicate over the Network
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#ifndef GCSCOMMUNICATOR_H_
#define GCSCOMMUNICATOR_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "GCSConnecton.h"
#include "CommObject.h"


enum COM_OBJ_GROUP{
	STEERING_EVENT = 1
};

enum COM_OBJ_STEERING_EVENTS
{
	KEY_UP = 1,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
};

class GCSCommunicator {
private:
	GCSConnecton _connection;

	//for server
	socklen_t clilen;
	//for client
	struct hostent *server;
	//for client and server
	int sockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;

	CommObject comObj[20][20];

	void error(const char *msg);
	void listenForConnections();
	void connectToServer();
	void readIncommingMessages(); // gets called in the update function
	void createMessageForComObj(char objectGroup, char object);	//gets called automatically whenever a value is changed

public:
	void setComObjValue(char objectGroup, char object, char newValue);
	char getComObjValue(char objectGroup, char object);
	void addOnComObjChangeFunktion(char objectGroup, char object, void (*f)());

	GCSCommunicator(int port); //constructor for the Server
	void update();
	GCSCommunicator(char *hostIP, int port); //constructor for the Client
	virtual ~GCSCommunicator();
};

#endif /* GCSCOMMUNICATOR_H_ */
