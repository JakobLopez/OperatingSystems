```
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//NAME: 
//		Jakob Lopez
//CLASS:
//		Operating Systems
//PROGRAM: 
//		MS-SHELL
//DESCRIPTION: 
//		This program is a shell that opens executable files
//		and performs basic arithmetic provided by the user.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include<iostream>
using namespace std;

void _tmain()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	TCHAR command[35];
	int x, y;

	//Two types of input:
	//	1) + num1 num2		or		- num1 num2
	//	2) notepad.exe or C:\some\path\to\task.exe

	cout << "Enter command" << endl;
	cin >> command;
	
	//If command is '%', don't execute anymore
	while (command[0] != '%')
	{
		//If first char is '+', perform addition
		if (command[0] == '+')
		{
			cin >> x >> y;
			cout << x + y<<endl;
		}
		//If first char is '-', perform subtraction
		else if (command[0] == '-')
		{
			cin >> x >> y;
			cout << x - y << endl;
		}
		else
		{	
			//Start child process
			if (!CreateProcess(
				NULL,
				command,
				NULL,
				NULL,
				false,
				0,
				NULL,
				NULL,
				&si,
				&pi)
				)
			{
				printf("CreateProcess failed \n");
				return;
			}

			//Wait until child process exists
			WaitForSingleObject(pi.hProcess, INFINITE);

			//Close process and thread handles
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		//get next command
		cout << "Enter command" << endl;
		cin >> command;
	}

}
```
