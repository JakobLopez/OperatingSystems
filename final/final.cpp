/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//NAME: 
//		Jakob Lopez
//CLASS:
//		Operating Systems
//PROGRAM: 
//		final_exam
//DESCRIPTION: 
//						RUN USING G++ COMPILER
//		This program uses 3 threads to count from 1 to 10. This is done
//		two different ways: with and without a mutex. When counting 
//		without a mutex, the output is variable and uncontrolled. An
//		array of mutexes is used to control the order of the count and
//		threads. The array is the size of number of threads, so each
//		mutex corresponds to a thread. To ensure the threads alternate
//		after each count, the current thread locks its mutex, updates
//		count and then unlocks the next threads mutex. Because it never
//		unlocked its own mutex, it has to wait until it can have access.
//		This access will be granted when thread (tid - 1) % NUMBERTHREADS
//		unlockes the mutex for it.
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define HAVE_STRUCT_TIMESPEC
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define NUMBER_OF_THREADS 3
#define MAX 10

//Globals
int count = 1;
pthread_mutex_t mutex[NUMBER_OF_THREADS];

//Prototypes
void destroyMutex();
void *increment(void *tid);
void *increment_with_mutex(void *tid);
void initMutex();

int main()
{
	pthread_t threads[NUMBER_OF_THREADS];
	int status;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									      WITHOUT MUTEX													   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("\nWithout mutex \n");

	//Calls increment for each thread
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		status = pthread_create(&threads[i], NULL, increment, (void *)i);

		if (status != 0)
		{
			printf("Oops. pthreads_Create returned error code %d \n", status);
			exit(-1);
		}
	}

	//Waiting for all threads to complete 
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											MUTEX														   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("\nWith mutex \n");

	//Initializes mutex array
	initMutex();

	//Reinitialize count to 0
	count = 1;

	//Calls increment_with_mutex for each thread
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		status = pthread_create(&threads[i], NULL, increment_with_mutex, (void *)i);

		if (status != 0)
		{
			printf("Oops. pthreads_Create returned error code %d \n", status);
			exit(-1);
		}
	}

	//Waiting for all threads to complete 
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);

	//Destroy the mutex array
	destroyMutex();

	exit(0);
}

//Destroys the mutex array
void destroyMutex()
{
	//Initialize mutex
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		
		pthread_mutex_destroy(&mutex[i]);
	}

}


//Threads count until 10 with no restrictions
void *increment(void *tid)
{
	while (count <= MAX)
	{
		printf("Display %d Thread %d \n", count, int(tid) + 1);
		count = count + 1;
	}

	printf("Counter is already 10 Thread %d \n", int(tid) + 1);
	pthread_exit(NULL);

}

//Increments count with mutex
//Count updates in order while alternating threads
void *increment_with_mutex(void *tid)
{
	while(count <= MAX)
	{
		//Current thread gets a lock.
		//This lock will block the current thread from continuing after it
		//increments
		pthread_mutex_lock(&mutex[(int(tid))]);
		
		//Update count while less than or equal to 10
		if (count <= MAX)
		{
			printf("Display %d Thread %d \n", count, int(tid) + 1);
			count = count + 1;
		}
		
		//Unlock the next thread's mutex
		//Next thread acquires it's lock again and increments count 
		pthread_mutex_unlock(&mutex[(int(tid) + 1) % NUMBER_OF_THREADS]);
	}


	printf("Counter is already 10 Thread %d \n", int(tid) + 1);
	pthread_exit(NULL);
}

//Initialize mutex array
void initMutex()
{
	
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);
	}
}


