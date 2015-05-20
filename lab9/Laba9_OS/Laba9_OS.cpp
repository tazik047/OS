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
}


