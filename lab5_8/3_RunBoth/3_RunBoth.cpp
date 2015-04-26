// 3_RunBoth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#define CreateUnsuspendedProcess(a, b, c) CreateProcess(0, a, 0, 0, 0, 0, 0, 0, b, c);
int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	
	SYSTEMTIME st; 
	FILETIME ft;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);

	//run 1_Notepad
	TCHAR ProcName[] = _T("1_Notepad.exe");
	//BOOL b = CreateUnsuspendedProcess(ProcName, &si, &pi);

	unsigned _int64 minTime = ((_int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	TCHAR value[MAX_PATH];
	*((_int64*)value) = minTime;
	SetEnvironmentVariable(_T("NotepadTime"), value);
	_tprintf(value);
	TCHAR ProcName1[] = _T("2_FindAndGetInfo.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName1, &si, &pi);
	//run 2_FindAndGetInfo

	return 0;
}

