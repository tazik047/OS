// 4_ProcInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//solved solution for the task lies here:
	//msdn.microsoft.com/en-us/library/windows/desktop/ms686701(v=vs.85).aspx

	// TH32CS_SNAPPROCESS - includes all processes in the system in the snapshot
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
		// clean the snapshot object - FROM MSDN so idk if we need this
		//CloseHandle(hSnapshot);
		return -1;
	}

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	DWORD count = 0;
	unsigned int i;
	while (Process32Next(hSnapshot, &pe))
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		_tprintf(_T("%d. cntThreads: %d, szExeFile: %s\n"), count++, pe.cntThreads, pe.szExeFile);
		
		// следующие взято с той лабы. вроде работает. 
		// хотя мельникова говорила делать через Module32First/Module32Next
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
		}
		_tprintf(_T("\n"));
	}
	CloseHandle(hSnapshot);
	system("pause");
	return 0;
}

// doesn't work
// попытка сделать как говорила Мельникова. не знаю, где библиотеки тут лежат. надо погуглить...
int getModuleInfo(DWORD id)
{
	MODULEENTRY32 me;
	HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
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
		_tprintf(_T("%s \n", me.szModule));
	}
	CloseHandle(hModSnap);
	return 0;
}