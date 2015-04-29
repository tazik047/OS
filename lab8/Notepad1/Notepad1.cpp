// Notepad1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\Main3\RunProc.h"


int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si = { 0 }; //сбрасываем все поля | memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	/*TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);*/
	TCHAR ProcName[] = _T("Notepad.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName, &si, &pi, 0);
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

