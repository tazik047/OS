// Laba10_OS_(3).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#define CreateUnsuspendedProcess(path, lpsi, lppi) CreateProcess(0, path, 0, 0, 0, 0, 0, 0, lpsi, lppi);
#define count 5

TCHAR ProcName[] = _T("ReadWrite.exe");

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -50000000LL;

	for (int i = 0; i < count; i++) {
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

