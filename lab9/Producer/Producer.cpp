// Producer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Consumer\ConcurrentQueue.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(queueStruct), argv[1]);
	//_tprintf(_T("start %s\n"), argv[1]);
	int i = GetLastError();
	if (h == 0)
		return GetLastError();
	queueStruct* q1 = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE h1 = start();
	TCHAR temp[50];
	for (int i = 0; i < 5; i++){
		_stprintf_s(temp, _T("process %s\tmessage %d"), argv[2], i);
		if (!setMessage(*q1, temp, h1))
			i--;
		//_tprintf(_T("write str\n"));
		//Sleep(10000);
	}
	end(h, q1, h1);
	_tprintf(_T("all written in proc %s\n"),argv[2]);
	return 0;
}

