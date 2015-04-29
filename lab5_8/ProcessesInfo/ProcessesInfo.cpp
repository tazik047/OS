// ProcessesInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>

using namespace std;
vector<HANDLE> names;

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Error"));
		return -1;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	BOOL b = Process32First(hSnapshot, &pe);
	if (!b)
	{
		_tprintf(_T("Something went wrong..."));
		return -1;
	}

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded, i, count = 0;
	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);
	HANDLE hModSnap;
	while (Process32Next(hSnapshot, &pe))
	{
		_tprintf(_T("%d. cntThreads: %d, szExeFile: %s\n"), count++, pe.cntThreads, pe.szExeFile);
		getModuleInfo(pe.th32ProcessID);
		_tprintf(_T("\n"));
	}
	CloseHandle(hSnapshot);
	system("pause");
	return 0;
}
int getModuleInfo(DWORD id)
{
	HANDLE hModuleSnap;
	MODULEENTRY32 me;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("\n"));
		return -1;
	}

	// Set the size of the structure before using it.
	me.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me))
	{
		_tprintf(_T("Error"));
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return -1;
	}
	while (Module32Next(hModuleSnap, &me))
	{
		_tprintf(TEXT("   MODULE NAME:     %s\n"), me.szModule);
	} 
	return 0;
}

