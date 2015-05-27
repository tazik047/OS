// lab10(5).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#define MyCreateThread(threadFunc, value) CreateThread(0, 0, threadFunc, value, 0, 0);
#define min 100
#define max 500
#define count 4

const unsigned int PHILOSOPHERS_NUMBER = 5;
CRITICAL_SECTION cs;

struct PhilWithForks {
	int phil;
	HANDLE forks[2];
};

void hungry(int num) {
	EnterCriticalSection(&cs);
	_tprintf(_T("Philosopher %d is hungry.\n"), num);
	LeaveCriticalSection(&cs);
}

void eating(int num, int left, int right) {
	EnterCriticalSection(&cs);
	_tprintf(_T("Philosopher %d starts eating with %d and %d forks.\n"), num, left, right);
	LeaveCriticalSection(&cs);
}

void thinking(int num) {
	EnterCriticalSection(&cs);
	_tprintf(_T("Philosopher %d starts thinking.\n"), num);
	LeaveCriticalSection(&cs);
}

DWORD WINAPI dining(LPVOID p) {
	PhilWithForks *philWithForks = static_cast<PhilWithForks*>(p);

	for (unsigned int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
		SetEvent(philWithForks->forks[i]);
	}

	for (int i = 0; i < count; i++) {
		Sleep(min + rand() % (max - min));
		hungry(philWithForks->phil);
		/*
		//WaitForMultipleObjects(2, philWithForks->forks, true, INFINITE);
		WaitForSingleObject(philWithForks->forks[0], INFINITE);
		EnterCriticalSection(&cs);
		ResetEvent(philWithForks->forks[0]);
		_tprintf(_T("Philosopher %d takes %d folk.\n"), i, i);
		//LeaveCriticalSection(&cs);

		WaitForSingleObject(philWithForks->forks[1], INFINITE);
		//EnterCriticalSection(&cs);
		ResetEvent(philWithForks->forks[1]);
		_tprintf(_T("Philosopher %d takes %d folk.\n"), i, i+1);
		LeaveCriticalSection(&cs);

		eating(philWithForks->phil, philWithForks->phil, philWithForks->phil + 1);
		Sleep(min + rand() % (max - min));

		//EnterCriticalSection(&cs);
		SetEvent(philWithForks->forks[0]);
		SetEvent(philWithForks->forks[1]);
		//LeaveCriticalSection(&cs);
		*/

		//чужой код

		//вот тут - что ждем, что нет
		// нужно поочередно брать левую (i) и правую (i+1) вилки
		// set/reset лучше погуглить, посмотреть что за что отвечает. я не уверена	
		DWORD res = WaitForMultipleObjects(2, philWithForks->forks, true, 1);

		if (res != WAIT_OBJECT_0) {
			ResetEvent(philWithForks->forks[0]);
			ResetEvent(philWithForks->forks[1]);

			Sleep(min + rand() % (max - min));
			SetEvent(philWithForks->forks[0]);
			SetEvent(philWithForks->forks[1]);
		}
		Sleep(min + rand() % (max - min));

		thinking(philWithForks->phil); 
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	PhilWithForks philWithForks[PHILOSOPHERS_NUMBER];
	HANDLE forks[PHILOSOPHERS_NUMBER];
	HANDLE phils[PHILOSOPHERS_NUMBER];
	InitializeCriticalSection(&cs);

	for (unsigned int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
		forks[i] = CreateEvent(0, true, false, NULL);
		if (forks[i] == 0) {
			_tprintf(_T("Error.\n"));
		}
	}

	for (unsigned int i = 0; i < PHILOSOPHERS_NUMBER; i++) {
		philWithForks[i].phil = i;
		philWithForks[i].forks[0] = forks[i];

		if (i < 4)
			philWithForks[i].forks[1] = forks[i + 1];
		else
			philWithForks[i].forks[1] = forks[0];

		phils[i] = MyCreateThread(dining, &philWithForks[i]);
	}
	WaitForMultipleObjects(PHILOSOPHERS_NUMBER, phils, true, INFINITE);

	return 0;
}
