// Producer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Consumer\ConcurrentQueue.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(queueStruct), argv[1]);
	_tprintf(_T("start %s\n"), argv[1]);
	int i = GetLastError();
	if (h == 0)
		return GetLastError();
	queueStruct* q1 = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE h1 = start();
	setMessage(*q1, _T("stringlfdkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk1"), h1);
	setMessage(*q1, _T("string2"), h1);
	/*if (q1){
	UnmapViewOfFile(q1);
	q1 = 0;
	}*/
	setMessage(*q1, _T("string3333333333333333333"), h1);
	//ReleaseMutex(h1);
	//system("pause");
	if (q1){
		UnmapViewOfFile(q1);
		q1 = 0;
	}
	if (h){
		CloseHandle(h);
		h = 0;
	}
	//system("pause");
	_tprintf(_T("all written\n"));
	return 0;
}

