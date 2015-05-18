// Consumer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"



int _tmain(int argc, _TCHAR* argv[])
{
	argv[0] = _T("myMapView");
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE,  0, 1024*1024, argv[0]);
	if (h == 0)
		return -1;
	queueStruct* q = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	UnmapViewOfFile(q);
	CloseHandle(h);
	return 0;
}

