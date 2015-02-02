// Laba8_OS_(1)Notepad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {0};
	si.cb = sizeof (si);
	
	
	// Вызов текстового редактора
	char CurrentPath[MAX_PATH];
	GetCurrentDirectory (MAX_PATH, CurrentPath);
	BOOL b = CreateProcess (0, "NotePad.exe",
		0, 0, false, 0, 0, CurrentPath, &si, &pi);

	if (!b)
	{
		printf ("Error\n");
		return 1;
	}
	// Ждем завершения работы редактора
	WaitForSingleObject (pi.hProcess, INFINITE);
	CloseHandle (pi.hThread);
	CloseHandle (pi.hProcess);
	return 0;
}

