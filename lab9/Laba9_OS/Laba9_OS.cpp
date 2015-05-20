// Laba9_OS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "TheThreadFunc.h"
#include <vector>
#include <mutex>

#define MyCreateThread(threadFunc, id) CreateThread(0, 0, threadFunc, 0, 0, id);

HANDLE hThread[10];
DWORD id[10];
/*
int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_tprintf(_T("debug\n"));
#else
	_tprintf(_T("not debug\n"));
#endif

	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), _T("CountCall"));
	long* i = (long *)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	*i = 0;
	for (int j = 0; j < 10; j++) {
		hThread[j] = MyCreateThread(ThreadFunc, id + j);
	}

	WaitForMultipleObjects(10, hThread, true, INFINITE);

	_tprintf(_T("i = %d\n"), i);
	for (int j = 0; j < 10; j++) {
		CloseHandle(hThread[j]);
	}

	return 0;
}*/






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
	//DWORD retVal;
	//HANDLE** threadHandles;// [MAXIMUM_WAIT_OBJECTS];

	int i = 0;
	int maxThread = 20;
	while (true) {
		/*threadHandles = new HANDLE*[maxThread / MAXIMUM_WAIT_OBJECTS];
		for (int ind = 0; ind <= maxThread / MAXIMUM_WAIT_OBJECTS; ind++)
		threadHandles[ind] = new HANDLE[MAXIMUM_WAIT_OBJECTS];*/
		std::vector < std::vector <HANDLE> > threadHandles(maxThread / MAXIMUM_WAIT_OBJECTS + 1, std::vector<HANDLE>(MAXIMUM_WAIT_OBJECTS, NULL));
		threadCount = 0;
		mx.lock();
		for (int ind = 0; ind < maxThread; ind++){
			int ind1 = ind / MAXIMUM_WAIT_OBJECTS;
			int ind2 = ind%MAXIMUM_WAIT_OBJECTS;
			threadHandles[ind1][ind2] = CreateThread(NULL, 0, SleepThread, (LPVOID)ind, 0, &threadID);
		}
		mx.unlock();
		if (maxThread>64)
			int a = 2;
		for (int ind = 0; ind < maxThread / MAXIMUM_WAIT_OBJECTS + 1; ind++){
			HANDLE* t = threadHandles[ind].data();
			int wait;
			if (ind == maxThread / MAXIMUM_WAIT_OBJECTS)
				wait = maxThread % MAXIMUM_WAIT_OBJECTS;
			else
				wait = MAXIMUM_WAIT_OBJECTS;
			WaitForMultipleObjects(wait, t, TRUE, INFINITE);
		}
		//WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, threadHandles[ind].data(), TRUE, INFINITE);

		for (int ind = 0; ind < maxThread; ind++){
			CloseHandle(threadHandles[ind / MAXIMUM_WAIT_OBJECTS][ind % MAXIMUM_WAIT_OBJECTS]);
		}
		_tprintf(_T("max=%d threads=%d\n"), maxThread, threadCount);
		//system("pause");
		if (threadCount != maxThread){
			break;
		}
		maxThread += 10;

		//Sleep(50);
		//i++;
		//if (i != threadCount) break;
	}

	_tprintf(_T("end"));
	//int numThreads = MAXIMUM_WAIT_OBJECTS;
	/*retVal = WaitForMultipleObjects(i,    // number of threads to wait for
	threadHandles, // handles for threads to wait for
	TRUE,          // wait for all of the threads
	INFINITE       // wait forever
	);*/

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

