// Process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Laba9_OS\TheThreadFunc.h"

#define MyCreateThread(threadFunc) CreateThread(0, 0, threadFunc, 0, 0, 0);

int _tmain(int argc, _TCHAR* argv[])
{
	//_tprintf(_T("1Procces stars!\n "));//,argv[1]);
	HANDLE hThread = MyCreateThread(ThreadFunc);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

