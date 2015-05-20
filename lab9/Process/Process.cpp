// Process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Laba9_OS\TheThreadFunc.h"

#define MyCreateThread(threadFunc, value) CreateThread(0, 0, threadFunc, value, 0, 0);

int toInt(TCHAR* str){
	__int64 res = 0;
	int i = 0;
	while (str[i] != '\0'){
		res = res * 10 + (str[i++] - '0');
	}
	return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf(_T("Procces%s starts!\n "),argv[1]);
	HANDLE hThread = MyCreateThread(ThreadFunc, (LPVOID)toInt(argv[1]));
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

