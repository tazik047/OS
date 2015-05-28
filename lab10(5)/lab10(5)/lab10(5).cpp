// lab10(5).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#define MyCreateThread(threadFunc, value) CreateThread(0, 0, threadFunc, value, 0, 0);
#define WaitInf(obj) WaitForSingleObject(obj, INFINITE);
#define min 100
#define max 500
#define count 5

const unsigned int PHILOSOPHERS_NUMBER = 5;

HANDLE semaphore;
HANDLE forks[PHILOSOPHERS_NUMBER];

void wait()
{
	Sleep(min + rand() % (max - min));
}

void eating(int num, int left, int right) {
	_tprintf(_T("Philosopher %d starts eating with %d and %d forks.\n"), num, left, right);
	wait();
}

void takes(int num, int ind){
	_tprintf(_T("Phil %d takes %d fork\n"), num, ind);
}

void puts(int num, int ind){
	_tprintf(_T("Phil %d puts %d fork\n"), num, ind);
}

void thinking(int num) {
	_tprintf(_T("Philosopher %d starts thinking.\n"), num);
	wait();
}

DWORD WINAPI dining(LPVOID p) {
	int index = (int)p;
	int lf = index;
	int rf = (index + 1) % PHILOSOPHERS_NUMBER;
	for (int i = 0; i < count; i++)
	{
		thinking(index);
		WaitInf(semaphore);
		WaitInf(forks[lf]);
		takes(index, lf);
		WaitInf(forks[rf]);
		takes(index, rf);
		eating(index, lf, rf);
		puts(index, rf);
		ReleaseMutex(forks[rf]);
		puts(index, lf);
		ReleaseMutex(forks[lf]);
		long* prev = NULL;
		ReleaseSemaphore(semaphore, 1, 0);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	semaphore = CreateSemaphore(0, PHILOSOPHERS_NUMBER - 1, PHILOSOPHERS_NUMBER - 1, NULL);
	HANDLE phils[PHILOSOPHERS_NUMBER];

	for (unsigned int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
		forks[i] = CreateMutex(0, false, NULL);
		if (forks[i] == 0) {
			_tprintf(_T("Error.\n"));
		}
	}

	for (unsigned int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
		phils[i] = MyCreateThread(dining, (LPVOID)i);
	}
	WaitForMultipleObjects(PHILOSOPHERS_NUMBER, phils, true, INFINITE);
	system("pause");
	return 0;
}
