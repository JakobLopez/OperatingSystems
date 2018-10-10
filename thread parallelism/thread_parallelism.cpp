/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//NAME: 
//		Jakob Lopez
//CLASS:
//		Operating Systems
//PROGRAM: 
//		thread_parallelism
//DESCRIPTION: 
//		This program declares an array with numbers 1 - 5000.
//		Threads are used to find the sum of the array. Each
//		thread computes section tid * 1000 to (tid + 1) * 1000.
//		Pthread_join is used to wait for all threads to finish,
//		otherwise our answer may not be correct. We calculate the sum
//		with and without a mutex. The mutex function always gives the 
//		right answer. The two answers are compared.
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define HAVE_STRUCT_TIMESPEC
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define NUMBER_OF_THREADS 5
#define MAX 5000

//Globals
int nums[MAX];
int total = 0;
int mutex_total = 0;
pthread_mutex_t mutex;

//Prototypes
void *sum_array(void *tid);
void *sum_array_mutex(void *tid);
void initArray(int nums[]);


int main()
{
	pthread_t threads[NUMBER_OF_THREADS];
	int status;
	

	//Initializes array with ints 1 - 5000
	initArray(nums);

	//Calls sum_array for each thread
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		status = pthread_create(&threads[i], NULL, sum_array, (void *)i);
		
		if (status != 0)
		{
			printf("Oops. pthreads_Create returned error code %d \n",status);
			exit(-1);
		}
	}

	//Waiting for all threads to complete 
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);
		

	printf("Without mutexes, the total is  %d \n", total);
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											MUTEX														   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Initialize mutex
	pthread_mutex_init(&mutex, NULL);

	//Calls sum_array_mutex for each thread
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		status = pthread_create(&threads[i], NULL, sum_array_mutex, (void *)i);

		if (status != 0)
		{
			printf("Oops. pthreads_Create returned error code %d \n", status);
			exit(-1);
		}
	}

	//Waiting for all threads to complete 
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("With mutexes, the total is  %d \n", mutex_total);

	pthread_mutex_destroy(&mutex);


	exit(NULL);
}

//Initializes array with numbers 1 through 5000
void initArray(int nums[])
{
	for (int i = 0; i < MAX; i++)
	{
		nums[i] = i + 1;
	}
}

//Threads find the total sum of an array
void *sum_array(void *tid)
{

	int thread_sum = 0;

	int bottom = ((int)tid) * 1000;
	int top = (((int)tid + 1) * 1000) - 1;
	
	for (int i = bottom; i <= top; i++)
	{
		total += nums[i];
		thread_sum += nums[i];
	}
	
	printf("Thread %d 's sum is %d \n", int(tid) + 1, thread_sum);

	pthread_exit(NULL);

}

//Uses mutexes to find the total sum of an array
void *sum_array_mutex(void *tid)
{
	int bottom = ((int)tid) * 1000;
	int top = (((int)tid + 1) * 1000) - 1;

	int thread_sum = 0;

	for (int i = bottom; i <= top; i++)
	{
		pthread_mutex_lock(&mutex);
		mutex_total += nums[i];
		pthread_mutex_unlock(&mutex);

		thread_sum += nums[i];

	}
	printf("Thread %d 's sum is %d \n", int(tid) + 1, thread_sum);
	pthread_exit(NULL);
}

