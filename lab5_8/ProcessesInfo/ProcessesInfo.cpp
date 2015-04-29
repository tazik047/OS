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
		getModuleInfo(pe.th32ProcessID);
		//====================UNWORKING
		/*
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
			_tprintf(_T("%s \n", me.szExePath));
		}
		*/

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

// doesn't work
// попытка сделать как говорила Мельникова. не знаю, где тут инфу о dll взять. надо погуглить...


int getModuleInfo(DWORD id)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Error"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me32))
	{
		_tprintf(_T("Error"));
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return(FALSE);
	}

	// Now walk the module list of the process,
	// and display information about each module
	do
	{
		_tprintf(TEXT("\n\   MODULE NAME:     %s"), me32.szModule);

	} while (Module32Next(hModuleSnap, &me32));


	/*MODULEENTRY32 me;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
	HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (hModSnap == INVALID_HANDLE_VALUE)
	{
	_tprintf(_T("Error"));
	return -1;
	}
	me.dwSize = sizeof(MODULEENTRY32);

	BOOL bMod = Module32First(hModSnap, &me);
	if (!bMod)
	{
	_tprintf(_T("Error"));
	return -1;
	}

	while (Module32Next(hModSnap, &me))
	{
	//тут явно должно быть не это
	_tprintf(_T("%s \n", me.szModule));
	}

	CloseHandle(hModSnap);*/
	return 0;
}

