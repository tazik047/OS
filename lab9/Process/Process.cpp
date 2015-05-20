// Process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Laba9_OS\TheThreadFunc.h"

#define MyCreateThread(threadFunc, value) CreateThread(0, 0, threadFunc, value, 0, 0);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread = MyCreateThread(ThreadFunc, (LPVOID)toInt(argv[1]));
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

