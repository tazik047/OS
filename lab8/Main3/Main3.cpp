// Main3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "RunProc.h"
#include <locale.h>


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	int c = -1;
	TCHAR ProcName[] = _T("Notepad1.exe");
	TCHAR ProcName1[] = _T("Find2.exe");
	TCHAR ProcName3[] = _T("Info4.exe");
	BOOL b;
	__int64 time;
	while (c != 0)
	{
		system("cls");
		_tprintf(_T("0. Выйти из программы.\n"));
		_tprintf(_T("1. Запустить проверку файлов\n"));
		_tprintf(_T("2. Просмотреть информацию о процессах\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
			case 1:
				SYSTEMTIME st;
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				time = (__int64(ft.dwHighDateTime) << 32) | __int64(ft.dwLowDateTime);
				TCHAR value[MAX_PATH];
				_stprintf_s(value, _T("%I64d"), time);
				SetEnvironmentVariable(_T("Time"), value);

				b = CreateUnsuspendedProcess(ProcName, &si, &pi, 0);
				if (!b) 
				{
					_tprintf(_T("Не удалось запустить процесс %s"), ProcName);
					return 0;
				}
				WaitForSingleObject(pi.hProcess, INFINITE);


				b = CreateUnsuspendedProcess(ProcName1, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("Не удалось запустить процесс %s"), ProcName1);
					return 0;
				}
				WaitForSingleObject(pi.hProcess, INFINITE);				
				break;
			case 2:
				b = CreateUnsuspendedProcess(ProcName3, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("Не удалось запустить процесс %s"), ProcName3);
					return 0;
				}
				WaitForSingleObject(pi.hProcess, INFINITE);
				break;
		}
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}
