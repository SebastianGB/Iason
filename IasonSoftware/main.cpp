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



WheelPlatform wheelPlatform;
GCSCommunicator communicator = GCSCommunicator(24698);
Steering steering = Steering(&wheelPlatform, &communicator);

void terminateSignalHandler(int signal)
{
	std::cout << "termination Handler called";
	communicator.~GCSCommunicator();
	exit(signal);
}

void v1()
{
	steering.steerPositiveLinearAcceleration();
}
void v2()
{
	steering.steerNegativeLinearAcceleration();
}
void v3()
{
	steering.steerPositiveAngularAcceleration();
}
void v4()
{
	steering.steerNegativeAngularAcceleration();
}

int main()
{
	signal(SIGINT,terminateSignalHandler);
	signal(SIGQUIT,terminateSignalHandler);
	signal(SIGSTOP,terminateSignalHandler);
	signal(SIGHUP,terminateSignalHandler);
	signal(SIGTERM,terminateSignalHandler);


	communicator.addOnComObjChangeFunktion(STEERING_EVENT, KEY_UP, v1);
	communicator.addOnComObjChangeFunktion(STEERING_EVENT, KEY_DOWN, v2);
	communicator.addOnComObjChangeFunktion(STEERING_EVENT, KEY_LEFT, v3);
	communicator.addOnComObjChangeFunktion(STEERING_EVENT, KEY_RIGHT, v4);


	bool run = true;
	while(run)
	{
		communicator.update();
		steering.update();
		wheelPlatform.update();
	}


	std::cout << "zum beenden bitte enter drücken";
	int wait;
	std::cin >> wait;
}
















