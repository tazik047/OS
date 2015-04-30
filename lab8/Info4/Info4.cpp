#include "stdafx.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hProcSnap, hModSnap;
	PROCESSENTRY32 procEntry;
	MODULEENTRY32 modEntry;
	DWORD procCount = 1, modCount = 1;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //TH32CS_SNAPPROCESS – інформація про процеси
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Error ---> CreateToolhelp32Snapshot"));
		return -1;
	}
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	modEntry.dwSize = sizeof(MODULEENTRY32);

	if (!Process32First(hProcSnap, &procEntry))
	{
		_tprintf(_T("Error ---> Process32First"));
		return -1;
	}
	
	while (Process32Next(hProcSnap, &procEntry))
	{
		_tprintf(_T("%d. PROCESS %d HAS %s THREAD(-S)\n"), procCount++, procEntry.cntThreads, procEntry.szExeFile);

		hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procEntry.th32ProcessID);
		if (hModSnap == INVALID_HANDLE_VALUE)
		{
			printLine();
			continue;
		}
		modEntry.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModSnap, &modEntry))
		{
			_tprintf(_T("Error ---> Module32First"));
			continue;
		}
		while (Module32Next(hModSnap, &modEntry))
		{
			_tprintf(_T("%d   %s\n"), modCount++, modEntry.szModule);
		}
		modCount = 1;
		printLine();
	}
	CloseHandle(hProcSnap);
	system("pause");
	return 0;
}

void printLine()
{
	_tprintf(_T("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"));
}