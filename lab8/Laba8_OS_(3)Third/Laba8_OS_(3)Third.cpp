// Laba8_OS_(3)Third.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

void runProcess(TCHAR fullPath[MAX_PATH])
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {0};
	si.cb = sizeof (si);

	TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory (MAX_PATH, CurrentPath);

	BOOL b = CreateProcess (0, fullPath,
		0, 0, false, 0, 0, CurrentPath, &si, &pi);

	if (!b)
	{
		printf ("Error\n");
		//return 1;
	}
	WaitForSingleObject (pi.hProcess, INFINITE);
	CloseHandle (pi.hThread);
	CloseHandle (pi.hProcess);
}
int _tmain(int argc, _TCHAR* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	while(true)
	{
		TCHAR first [MAX_PATH] = _T("C:/Users/Ira/Documents/Visual Studio 2012/Projects/Laba8_OS_(1)Notepad/Debug/Laba8_OS_(1)Notepad.exe");
		TCHAR second [MAX_PATH] = _T("C:/Users/Ira/Documents/Visual Studio 2012/Projects/Laba8_OS(2)Find/Debug/Laba8_OS(2)Find.exe");
		runProcess(first);
		runProcess(second);

	}
	return 0;
}

