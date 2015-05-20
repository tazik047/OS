// Laba9_OS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "TheThreadFunc.h"
#include <vector>
#include <mutex>
#include <locale.h>

#define MyCreateThread(threadFunc, value, id) CreateThread(0, 0, threadFunc, value, 0, id);
#define CreateUnsuspendedProcess(path, lpsi, lppi, directory) CreateProcess(0, path, 0, 0, 0, 0, 0, directory, lpsi, lppi);


HANDLE hThread[10];
DWORD id[10];

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	TCHAR temp[20];
	BOOL b;
	TCHAR ProcName[] = _T("Process.exe %d");

#ifdef _DEBUG
	_tprintf(_T("debug\n"));
#else
	_tprintf(_T("not debug\n"));
#endif

	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), _T("CountCall"));
	long* i = (long *)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE mutex = CreateMutex(0, FALSE, _T("mutex!"));
	*i = 0;
	//TCHAR temp[5];
	/*for (int j = 0; j < 10; j++) {
		hThread[j] = MyCreateThread(ThreadFunc, (LPVOID)j,  id + j);
	}*/

	
	for (int j = 0; j < 10; j++) {
		//hThread[j]
		_stprintf_s(temp, ProcName, j);
		b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
		if (!b)
		{
			_tprintf(_T("Не удалось запустить процесс %s"), temp);
			return -1;
		}
		hThread[j] = pi.hProcess;
		//_stprintf_s(temp, _T("%s %d"), hThread[j], j);
	}

	WaitForMultipleObjects(10, hThread, true, INFINITE);
	
	_tprintf(_T("i = %d\n"), *i);
	for (int j = 0; j < 10; j++) {
		CloseHandle(hThread[j]);
	}
	CloseHandle(mutex);
	return 0;
}