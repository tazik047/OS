// Laba10_OS_(3).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include<string>

#define CreateUnsuspendedProcess(path, lpsi, lppi) CreateProcess(0, path, 0, 0, 0, 0, 0, 0, lpsi, lppi);
#define count 5

TCHAR ProcName[] = _T("ReadWrite.exe");

#define MAX_SIZE 10000

struct threadStruct{
	int Count;
	char messages[MAX_SIZE][256];
	int length;
};

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -10000000LL;
	HANDLE map = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(threadStruct), _T("myMapView"));
	int i = GetLastError();
	if (map == 0)
		return GetLastError();
	threadStruct* q = (threadStruct*)MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE mutex = CreateMutex(0, FALSE, _T("counterLock"));
	for (int i = 0; i < count; i++) {	
		WaitForSingleObject(mutex, INFINITE);
		q->Count += 2;
		ReleaseMutex(mutex);
		CreateUnsuspendedProcess(ProcName, &si, &pi);
		hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		if (NULL == hTimer)
		{
			_tprintf(_T("Error"));
			return -1;
		}

		printf("Next process in 5 seconds...\n");
		if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
		{
			_tprintf(_T("Error"));
			return -1;
		}

		if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		{
			_tprintf(_T("Error"));
		}
		else 
		{
			printf("Timer ends.\n\n");
		}
	}
	return 0;
}

