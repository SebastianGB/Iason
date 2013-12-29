/*
 * GCSCommunicator.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: potato
 */

#include "GCSCommunicator.h"

GCSCommunicator::GCSCommunicator(int port) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = port;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listenForConnections();
}

GCSCommunicator::GCSCommunicator(char *hostIP, int port)
{
   portno = port;
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
       error("ERROR opening socket");
   server = gethostbyname(hostIP);
   if (server == NULL) {
       fprintf(stderr,"ERROR, no such host\n");
       exit(0);
   }
   connectToServer();
}

void GCSCommunicator::error(const char *msg)
{
    perror(msg);
    exit(1);
}

//this method is for servers only
void GCSCommunicator::listenForConnections()
{
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	_connection._start(newsockfd);
}

//this method is for clients only
void GCSCommunicator::connectToServer()
{
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    _connection._start(sockfd);
}

void GCSCommunicator::setComObjValue(char objectGroup, char object, char newValue)
{
	if(!comObj[objectGroup][object].setValue(newValue))
	{
		createMessageForComObj(objectGroup, object);
	}
}
char GCSCommunicator::getComObjValue(char objectGroup, char object)
{
	return comObj[objectGroup][object].getValue();
}
void GCSCommunicator::addOnComObjChangeFunktion(char objectGroup, char object, void (*f)())
{
	comObj[objectGroup][object].addOnChangeFunction(*f);
}

void GCSCommunicator::readIncommingMessages() // gets called in the update function
{
	Message msg;
	pthread_mutex_lock(&(_connection._msginMutex));
		while(!_connection._msgin.empty())
		{
			msg = _connection._msgin.back();
			_connection._msgin.pop_back();
			comObj[msg.group][msg.entity].setValue(msg.value);
		}
	pthread_mutex_unlock(&(_connection._msginMutex));
}
void GCSCommunicator::createMessageForComObj(char objectGroup, char object) //gets called automatically whenever a value is changed
{
	Message msg;
	msg.group = objectGroup;
	msg.entity = object;
	msg.value = comObj[objectGroup][object].getValue();

	pthread_mutex_lock(&(_connection._msgoutMutex));
	_connection._msgout.push_front(msg);
    pthread_mutex_unlock(&(_connection._msgoutMutex));
}

void GCSCommunicator::update()
{
	readIncommingMessages();
}

GCSCommunicator::~GCSCommunicator() {
	// TODO Auto-generated destructor stub
}

