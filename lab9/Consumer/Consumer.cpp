// Consumer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//argv[1] = _T("proc1");
	//argv[1] = _T("myMapView");
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE,  0, sizeof(queueStruct), argv[1]);
	_tprintf(_T("startC %s\n"), argv[1]);
	system("pause");
	if (h == 0)
		return -1;
	queueStruct* q = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE mutex = start();
	while (!q->end){
		if (isEmpty(*q, mutex)) {
			//_tprintf(_T("\twait\n"));
			continue;
		}
		TCHAR* text = getMessage(*q, mutex);
		_tprintf(_T("read - %s\n"), text);
	}
	UnmapViewOfFile(q);
	CloseHandle(h);
	_tprintf(_T("end"));
	return 0;
}

