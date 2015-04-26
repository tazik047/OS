// lab5_8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

// a - path
// b - address of STARTUPINFO
// c - address of PROCESS_INFORMATION
// d - Directory
#define CreateUnsuspendedProcess(a, b, c, d) CreateProcess(0, a, 0, 0, 0, 0, 0, d, b, c);
#define CreateSuspendedProcess(a, b, c,d) CreateProcess(0, a, 0, 0, 0, CREATE_SUSPENDED, 0, d, b, c);

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	TCHAR ProcName[] = _T("Notepad.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName, &si, &pi, CurrentPath);
	if (!b)
	{
		printf("Error\n");
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}



