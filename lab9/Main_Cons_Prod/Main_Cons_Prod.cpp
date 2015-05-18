// Main_Cons_Prod.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\Consumer\ConcurrentQueue.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR name[] = _T("myMapView");
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 1024*1024, name );
	int i = GetLastError();
	if (h == 0)
		return -1;
	queueStruct* q1 = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	setMessage(*q1, "stringlfdkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk1");
	setMessage(*q1, "string2");
	if (q1){
		UnmapViewOfFile(q1);
		q1 = 0;
	}
	system("pause");
	if (q1){
		UnmapViewOfFile(q1);
		q1 = 0;
	}
	if (h){
	CloseHandle(h);
	h = 0;
	}
	return 0;
}

