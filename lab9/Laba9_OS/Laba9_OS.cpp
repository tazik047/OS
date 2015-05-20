// Laba9_OS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "TheThreadFunc.h"

#define MyCreateThread(threadFunc, id) CreateThread(0, 0, threadFunc, 0, 0, id);

HANDLE hThread[10];
DWORD id[10];

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_tprintf(_T("debug\n"));
#else
	_tprintf(_T("not debug\n"));
#endif

	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), _T("CountCall"));
	long* i = (long *)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	*i = 0;
	for (int j = 0; j < 10; j++) {
		hThread[j] = MyCreateThread(ThreadFunc, id + j);
	}

	WaitForMultipleObjects(10, hThread, true, INFINITE);

	_tprintf(_T("i = %d\n"), i);
	for (int j = 0; j < 10; j++) {
		CloseHandle(hThread[j]);
	}

	return 0;
}







/*
int count = 0;
FILE*  fileHandle;
DWORD WINAPI tenThreads(LPVOID parametr)
	{
		count++;
#ifdef DEBUG
		fprintf(fileHandle,"Begin thread: %s\n",(char*) parametr);
#endif
		for(long i = 0;i < 10000000;i++)
		{
			
		}
#ifdef DEBUG
		fprintf(fileHandle,"End thread: %s\n",(char*)parametr);
#endif
		return 0;
	}

int _tmain(int argc, _TCHAR* argv[])
{
	char *threads[] = {"1", "2","3", "4", "5","6","7","8","9", "10"};
	HANDLE threadHandles[10];
	fileHandle = fopen("file.txt","w");
	if(fileHandle == NULL)
		return 1;
	for(int i = 0;i < 10;i++)
	{
	threadHandles[i] = CreateThread(NULL, 0, tenThreads, threads[i], 0, NULL);
	if (!threadHandles[i])
		{
			printf ("CreateThread Error\n");
			continue;
		}
	}
	WaitForMultipleObjects (10, threadHandles, false, INFINITE);
	for (int i = 0; i < 10; i++)
		CloseHandle (threadHandles[i]);
	fprintf(fileHandle,"Threads: %d\n",count);
	fclose(fileHandle);
	system("pause");
	return 0;
}*/

