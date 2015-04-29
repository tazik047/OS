// 3_RunBoth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <locale>
#define CreateUnsuspendedProcess(a, b, c) CreateProcess(0, a, 0, 0, 0, 0, 0, 0, b, c);
//BOOL WINAPI CreateProcess(
//	LPCTSTR lpApplicationName, // Ім'я додатка
//	LPTSTR lpCommandLine,// Командний рядок
//	LPSECURITY_ATTRIBUTES lpProcessAttributes, // Атрибути
//	LPSECURITY_ATTRIBUTES lpThreadAttributes,  // безпеки
//	BOOL bInheritHandles, // Спадкування дескрипторів
//	DWORD dwCreationFlags, // Прапорці створення
//	LPVOID lpEnvironment, // Середа
//	LPCTSTR lpCurrentDirectory,// поточний каталог
//	LPSTARTUPINFO lpStartupInfo, // Структура з вхідними даними
//	LPPROCESS_INFORMATION lpProcessInformation
//	// Структура з дескрипторами створеного процесу та потоку
//	);

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	_tprintf(_T("Сколько файлов желаете создать???:\n"));
	size_t c = -1;
	int count = 0;
	_tscanf_s(_T("%d"), &c);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	SYSTEMTIME st;
	FILETIME ft;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	__int64 minTime = (__int64(ft.dwHighDateTime) << 32) | __int64(ft.dwLowDateTime);
	char value[MAX_PATH];
	//*((__int64*)value) = minTime;
	_i64toa(minTime, value, 10);
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, value, -1, wString, 4096);
	SetEnvironmentVariable(_T("NotepadTime"), wString);
	while (count < c) {
		TCHAR ProcName[] = _T("1_Notepad.exe");
		BOOL flag = CreateUnsuspendedProcess(ProcName, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		count++;
	}
	TCHAR ProcName1[] = _T("2_FindAndGetInfo.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName1, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}

