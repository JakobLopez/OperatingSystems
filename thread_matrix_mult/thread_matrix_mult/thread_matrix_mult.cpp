/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//NAME: 
//		Jakob Lopez
//CLASS:
//		Operating Systems
//PROGRAM: 
//		thread_matrix_mult
//DESCRIPTION: 
//		
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define HAVE_STRUCT_TIMESPEC
#include<pthread.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include <time.h>
using namespace std;

#define NUMBER_OF_THREADS 4
#define N 10000

float array1[N][N] = {};
float array2[N][N] = {};
float result[N][N] = {};


void initArray(float m[N][N]);
void* multiply_matrix_threads(void *tid);
void mult_matrix_noThread();
void changeRow(int, float);
void changeCol(int, float);
void printarr(float m[N][N], ofstream&);
void printAnalysis(int, int, clock_t,ofstream&);
void resetArray(float m[N][N]);

int main()
{
	pthread_t threads[NUMBER_OF_THREADS];
	int status;

	int rowNum, colNum;
	float rowVal, colVal;
	char thread_option;


	ofstream resultMatrix;
	ofstream analysis_w_thread;
	ofstream analysis_wo_thread;
	resultMatrix.open("result.txt");
	analysis_w_thread.open("analysis.txt");
	analysis_wo_thread.open("analysis2.txt");


	//Initializes arrays with random floats less than 1
	initArray(array1);
	initArray(array2);

	//Prompt user so answer can be verified
	printf("Enter a row:");
	cin >> rowNum;
	printf("Enter a value:");
	cin >> rowVal;

	printf("Enter a column:");
	cin >> colNum;
	printf("Enter a value:");
	cin >> colVal;


	changeRow(rowNum, rowVal);
	changeCol(colNum, colVal);

	for (int i = 0; i < 2; i++)
	{

		clock_t tStart = clock();

		if (i == 0)
		{
			cout << "Running program with threads..." << endl;
			for (int i = 0; i < NUMBER_OF_THREADS; i++)
			{
				status = pthread_create(&threads[i], NULL, multiply_matrix_threads, (void *)i);

				if (status != 0)
				{
					printf("Oops. pthreads_Create returned error code %d \n", status);
					exit(-1);
				}
			}

			//Waiting for all threads to complete 
			for (int i = 0; i < NUMBER_OF_THREADS; i++)
				pthread_join(threads[i], NULL);

			analysis_w_thread << "WITH THREADING (intel core i5)" << endl;
			printAnalysis(rowNum, colNum, tStart, analysis_w_thread);

			cout << "check analysis.txt"<<endl;
			cout << endl;

			resetArray(result);

		}
		else 
		{
			cout << "Running program without threads..." << endl;

			mult_matrix_noThread();

			analysis_wo_thread << "WITHOUT THREADING (intel core i5)" << endl;
			printAnalysis(rowNum, colNum, tStart, analysis_wo_thread);

			cout << "check analysis2.txt";


		}
	}

	printarr(result, resultMatrix);


	resultMatrix.close();
	analysis_w_thread.close();
	analysis_wo_thread.close();
}

void printAnalysis(int rowNum, int colNum,clock_t tStart, ofstream& out)
{

	out << "Result at [" << rowNum << ", " << colNum << "]: " << result[rowNum][colNum] << endl;
	out << "Left neighbor: " << result[rowNum][colNum - 1] << endl;
	out << "Right neighbor: " << result[rowNum][colNum + 1] << endl;
	out << "Top neighbor: " << result[rowNum - 1][colNum] << endl;
	out << "Bottom neighbor: " << result[rowNum + 1][colNum] << endl;
	out << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << "s";

}
void resetArray(float m[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			 m[i][j] = 0;
		}
	}
}
void printarr(float m[N][N], ofstream& out)
{

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			out << m[i][j] << " ";
		}
		out << endl;
	}
}

void changeCol(int c, float x)
{
	for (int i = 0; i < N; i++)
	{
		array2[i][c] = x;

	}
}

void changeRow(int r, float x)
{
	for (int i = 0; i < N; i++)
	{
		array1[r][i] = x;
	}
}
//Initializes arary with random values
void initArray(float m[N][N])
{
	float num = 0.0;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			//generates random number 0 to 1
			num = ((float)rand() / (RAND_MAX));

			m[i][j] = num;

		}
	}
}

void mult_matrix_noThread()
{
	// Multiplying matrix a and b and storing in array mult.
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			for (int k = 0; k < N; ++k)
			{
				result[i][j] += array1[i][k] * array2[k][j];
			}
}

void* multiply_matrix_threads(void *tid)
{
	const int n_elements = N * N;
	const int n_operations = n_elements / NUMBER_OF_THREADS;

	int start = n_operations * ((int)tid);
	int end_op = n_operations * (((int)tid) + 1);

	for (int op = start; op < end_op; ++op)
	{
		const int row = op % N;
		const int col = op / N;
		float r = 0.0f;

		for (int i = 0; i < N; ++i)
		{
			const float e1 = array1[row][i];
			const float e2 = array2[i][col];
			r += e1 * e2;
		}


		result[row][col] = r;
	}

	pthread_exit(NULL);
}