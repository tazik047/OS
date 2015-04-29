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
	//almost solved (without dll) solution for the task lies here:
	//msdn.microsoft.com/en-us/library/windows/desktop/ms686701(v=vs.85).aspx

	// TH32CS_SNAPPROCESS - includes all processes in the system in the snapshot
	// 0 - в любой непонятной ситуации... то есть, ноль - это current process
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
		//getModuleInfo(pe.th32ProcessID);
		//====================UNWORKING

		_tprintf(_T("%d\n"), pe.th32ProcessID);
		//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);// pe.th32ProcessID);

		if (hModSnap == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Error\n"));
			return -1;
		}

		BOOL bMod = Module32First(hModSnap, &me);
		if (!bMod)
		{
			_tprintf(_T("Error"));
			return -1;
		}

		while (Module32Next(hModSnap, &me))
		{
			//тут явно должно быть не это
			_tprintf(TEXT("\n\t %s"), me.szModule);
		}


		//====================WORKS
		/*
		// следующие взято с той лабы. вроде работает.
		// хотя Мельникова говорила делать через Module32First/Module32Next
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
		TCHAR szModName[MAX_PATH];

		// Get the full path to the module's file.
		if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
		{
		// Print the module name and handle value.
		_tprintf(_T("\t%s\n"), szModName);
		}
		}
		}*/
		_tprintf(_T("\n"));
	}
	CloseHandle(hSnapshot);
	system("pause");
	return 0;
}