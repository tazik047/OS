// lab5_8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

// a - path
// b - address of STARTUPINFO
// c - address of PROCESS_INFORMATION
#define CreateUnsuspendedProcess(a, b, c) CreateProcess(0, a, 0, 0, 0, 0, 0, 0, b, c);

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));

	si.cb = sizeof(STARTUPINFO);
	TCHAR ProcName[] = _T("Notepad.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName, &si, &pi);
	if (!b) 
	{
		_tprintf(_T("Oops! Something went wront..."));
		return -1;
	}
	
	// wait while program not closed
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}



