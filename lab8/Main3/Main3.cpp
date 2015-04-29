// Main3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "RunProc.h"
#include <locale.h>


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	SYSTEMTIME st;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	__int64 time = (__int64(ft.dwHighDateTime) << 32) | __int64(ft.dwLowDateTime);
	TCHAR value[MAX_PATH];
	_stprintf_s(value, _T("%I64d"), time);
	SetEnvironmentVariable(_T("Time"), value);

	TCHAR ProcName[] = _T("Notepad1.exe");
	BOOL flag = CreateUnsuspendedProcess(ProcName, &si, &pi, 0);
	WaitForSingleObject(pi.hProcess, INFINITE);

	TCHAR ProcName1[] = _T("Find2.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName1, &si, &pi, 0);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}
