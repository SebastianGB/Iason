/*#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <pthread.h>


using namespace std;

#define NUM_THREADS     5

int s;

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   cout << "Hello World! Thread ID, " << tid << endl;
   pthread_exit(NULL);
}

int main ()
{
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   for( i=0; i < NUM_THREADS; i++ ){
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], NULL,PrintHello, (void *)i);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}*/

#include <iostream>
#include <signal.h>
#include "GCSCommunication/GCSCommunicator.h"
#include "WheelPlatform/WheelEngine.h"
#include "Steering.h"



WheelPlatform *wheelPlatform;
GCSCommunicator *communicator;
Steering *steering;

void terminateSignalHandler(int signal)
{
	std::cout << "termination Handler called" << std::endl;
	communicator->~GCSCommunicator();
	exit(signal);
}
//TODO rename v1 - 4
void v1()
{
	steering->steerPositiveLinearVelo();
}
void v2()
{
	steering->steerNegativeLinearVelo();
}
void v3()
{
	steering->steerPositiveAngularVelo();
}
void v4()
{
	steering->steerNegativeAngularVelo();
}

int main()
{
	wheelPlatform = new WheelPlatform();
	communicator = new GCSCommunicator(24719);
	steering = new Steering(wheelPlatform, communicator);

	signal(SIGINT,terminateSignalHandler);
	signal(SIGQUIT,terminateSignalHandler);
	signal(SIGSTOP,terminateSignalHandler);
	signal(SIGHUP,terminateSignalHandler);
	signal(SIGTERM,terminateSignalHandler);


	communicator->addOnComObjChangeFunktion(STEERING_EVENT, KEY_UP, v1);
	communicator->addOnComObjChangeFunktion(STEERING_EVENT, KEY_DOWN, v2);
	communicator->addOnComObjChangeFunktion(STEERING_EVENT, KEY_LEFT, v3);
	communicator->addOnComObjChangeFunktion(STEERING_EVENT, KEY_RIGHT, v4);


	bool run = true;
	while(run)
	{
		communicator->update();
		steering->update();
		if(wheelPlatform->update() != 0)
		{
			std::cout << "Connection closed by main" << std::endl;
			communicator->~GCSCommunicator();
		}
		usleep(10000);
	}


	std::cout << "zum beenden bitte enter drücken";
	int wait;
	std::cin >> wait;
}

















