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
			//_tprintf(_T("Error ---> CreateToolhelp32Snapshot"));
			printLine();
			continue;
		}
		modEntry.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModSnap, &modEntry))
		{
			_tprintf(_T("Error"));
			CloseHandle(hModSnap); // clean the snapshot object
			continue;
		}
		while (Module32Next(hModSnap, &modEntry))
		{
			_tprintf(TEXT("%d   %s\n"), modCount++, modEntry.szModule);
		}
		modCount = 1;
		printLine();
	}
	CloseHandle(hProcSnap);
	system("pause");
	return 0;
}
/*
int getModuleInfo(DWORD id)
{


	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Error ---> CreateToolhelp32Snapshot"));
		return -1;
	}

	// Set the size of the structure before using it.
	me.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me))
	{
		_tprintf(_T("Error"));
		CloseHandle(hModuleSnap); // clean the snapshot object
		return -1;
	}
	while (Module32Next(hModuleSnap, &me))
	{
		_tprintf(TEXT("   MODULE NAME:     %s\n"), me.szModule);
	}
	return 0;
}*/

void printLine()
{
	_tprintf(_T("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"));
}