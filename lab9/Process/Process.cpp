// Process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "TheThreadFunc.h"

#define MyCreateThread(threadFunc, id) CreateThread(0, 0, threadFunc, 0, 0, id);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread = MyCreateThread(ThreadFunc, id);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

