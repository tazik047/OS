#include "stdafx.h"
#include <windows.h>


TCHAR path[] = _T("THREADS.txt");
TCHAR starts[] = _T("starts\r\n");
TCHAR ends[] = _T("ends\r\n");

DWORD size = wcslen(starts) * sizeof(TCHAR);
DWORD size2 = wcslen(ends) * sizeof(TCHAR);
BOOL fromScratch = true;

DWORD written;

DWORD WINAPI ThreadFunc(LPVOID p) {
	HANDLE hMutex = CreateMutex(0, FALSE, _T("mutex!"));
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), _T("CountCall"));
	
	long* i = (long *)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

#ifdef _DEBUG
	WaitForSingleObject(hMutex, INFINITE);
	HANDLE hFileStart = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	/*if (fromScratch) { 
		SetFilePointer(hFileStart, 0, 0, FILE_BEGIN); 
		_tprintf(_T("first\n"));
		fromScratch = false; 
	}
	else { 
		//SetFilePointer(hFileStart, 0, 0, FILE_END); 
	}*/
	SetFilePointer(hFileStart, 0, 0, FILE_END);
	WriteFile(hFileStart, &starts, size, &written, 0);
	if (written != size){
		_tprintf(_T("start-write-error\n"));
		return -1;
	}
	ReleaseMutex(hMutex);
	CloseHandle(hFileStart);
#endif

	InterlockedIncrement(i);
	for (int z = 0; z < 1000000; z++);
	
#ifdef _DEBUG
	WaitForSingleObject(hMutex, INFINITE);
	HANDLE hFileFinish = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	SetFilePointer(hFileFinish, 0, 0, FILE_END);
	WriteFile(hFileFinish, &ends, size2, &written, 0);
	if (written != size2){
		_tprintf(_T("end-wtite-error\n"));
		return -1;
	}
	ReleaseMutex(hMutex);
	CloseHandle(hFileFinish);
#endif

	CloseHandle(hMutex);
	CloseHandle(hMap);
	return 0;
}