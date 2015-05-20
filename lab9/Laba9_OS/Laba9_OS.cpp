// Laba9_OS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mutex>
//#define DEBUG
#define CrThreads(lpStartAddress, lpParametr, dwThreadId) CreateThread(NULL,0,lpStartAddress,lpParametr,0,dwThreadId)

int count = 0;
FILE*  fileHandle;
DWORD WINAPI tenThreads(LPVOID parametr)
	{
		count++;
#ifdef DEBUG
		fprintf(fileHandle,"Begin thread: %s\n",(char*) parametr);
#endif
		for(long i = 0;i < 10000000;i++)
		{
			
		}
#ifdef DEBUG
		fprintf(fileHandle,"End thread: %s\n",(char*)parametr);
#endif
		return 0;
	}
std::mutex mx;
int threadCount = 0;


unsigned long __stdcall
SleepThread(void*)
{
	mx.lock();
	threadCount++;
	mx.unlock();
	//while (true);
	return 0;
}
DWORD CALLBACK ThreadProc(void*)
{
	Sleep(INFINITE);
	return 0;
}

void getMyMaxThread() {
	int i = 0;
	while (true) {
		DWORD id;
		HANDLE h = CreateThread(NULL, 0, ThreadProc, NULL, 0, &id);
		if (!h) break;
		CloseHandle(h);
	}
	printf("Created %d threads\n", i);
}

void getMaxThread() {
	DWORD threadID;
	DWORD retVal;
	HANDLE* threadHandles;// [MAXIMUM_WAIT_OBJECTS];
	int i = 0;
	int max = 20;
	while (true) {
		threadHandles = new HANDLE[max];
		threadCount = 0;
		mx.lock();
		for (int ind = 0; ind < max; ind++){
			threadHandles[ind] = CreateThread(NULL, 0, SleepThread, NULL, 0, &threadID);
		}
		mx.unlock();
		WaitForMultipleObjects(max, threadHandles, TRUE, INFINITE);
		_tprintf(_T("%d\n",threadCount));
		for (int ind = 0; ind < max; ind++){
			CloseHandle(threadHandles[ind]);
		}
		if (threadCount != max) break;
		max++;
		
		//Sleep(50);
		//i++;
		//if (i != threadCount) break;
	}

	_tprintf(_T("end"));
	int numThreads = MAXIMUM_WAIT_OBJECTS;
	retVal = WaitForMultipleObjects(i,    // number of threads to wait for 
		threadHandles, // handles for threads to wait for
		TRUE,          // wait for all of the threads
		INFINITE       // wait forever
		);

	Sleep(1000);
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*char *threads[] = {"1", "2","3", "4", "5","6","7","8","9", "10"};
	HANDLE threadHandles[10];
	DWORD dwThreadId;
	fileHandle = fopen("file.txt","w");
	if(fileHandle == NULL)
		return 1;
	for(int i = 0;i < 10;i++)
	{
		threadHandles[i] = CrThreads(tenThreads, threads[i], &dwThreadId);
	if (!threadHandles[i])
		{
			printf ("CreateThread Error\n");
			continue;
		}
	}
	WaitForMultipleObjects (10, threadHandles, false, INFINITE);
	for (int i = 0; i < 10; i++)
		CloseHandle (threadHandles[i]);
	fprintf(fileHandle,"Threads: %d\n",count);
	fclose(fileHandle);*/
	getMaxThread();
	system("pause");
	return 0;
}

