// TheFourthTask.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <mutex>

#define CrThreads(lpStartAddress, lpParametr, dwThreadId) CreateThread(NULL,0,lpStartAddress,lpParametr,0,dwThreadId)

std::mutex mx;
int threadCount = 0;

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

unsigned long __stdcall
SleepThread(void*)
{
	mx.lock();
	threadCount++;
	mx.unlock();
	return 0;
}

void getMaxThread() {
	DWORD threadID;
	int i = 0;
	int maxThread = 20;
	while (true) {
		std::vector < std::vector <HANDLE> > threadHandles(maxThread / MAXIMUM_WAIT_OBJECTS + 1, 
			std::vector<HANDLE>(MAXIMUM_WAIT_OBJECTS, NULL));
		threadCount = 0;
		mx.lock();
		for (int ind = 0; ind < maxThread; ind++){
			int ind1 = ind / MAXIMUM_WAIT_OBJECTS;
			int ind2 = ind%MAXIMUM_WAIT_OBJECTS;
			threadHandles[ind1][ind2] = CreateThread(NULL, 0, SleepThread, (LPVOID)ind, 0, &threadID);
		}
		mx.unlock();
		for (int ind = 0; ind < maxThread / MAXIMUM_WAIT_OBJECTS + 1; ind++){
			HANDLE* t = threadHandles[ind].data();
			int wait;
			if (ind == maxThread / MAXIMUM_WAIT_OBJECTS)
				wait = maxThread % MAXIMUM_WAIT_OBJECTS;
			else
				wait = MAXIMUM_WAIT_OBJECTS;
			WaitForMultipleObjects(wait, t, TRUE, INFINITE);
		}
		for (int ind = 0; ind < maxThread; ind++){
			CloseHandle(threadHandles[ind / MAXIMUM_WAIT_OBJECTS][ind % MAXIMUM_WAIT_OBJECTS]);
		}
		_tprintf(_T("max=%d threads=%d\n"), maxThread, threadCount);
		if (threadCount != maxThread){
			break;
		}
		maxThread += 100;
	}
	_tprintf(_T("Max Thread count - %d\n"), threadCount);
}

int _tmain(int argc, _TCHAR* argv[])
{
	getMaxThread();
	system("pause");
	return 0;
}

