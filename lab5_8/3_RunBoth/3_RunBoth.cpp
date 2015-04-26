// 3_RunBoth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME st;
	FILETIME ft;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);

	//run 1_Notepad

	_int64 minTime = 0; // there is should to be smth else not 0
	TCHAR value[20]; // I guess, the value of minTime should be here
	*((_int64*)value) = minTime;
	SetEnvironmentVariable(_T("NotepadTime"), value);

	//run 2_FindAndGetInfo

	return 0;
}

