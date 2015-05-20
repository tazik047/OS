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
	if (h == 0)
		return -1;
	queueStruct* q = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	HANDLE mutex = start();
	while (!q->end){
		while (!isEmpty(*q, mutex)) {
			//_tprintf(_T("\twait\n"));
			//continue;
			TCHAR* text = getMessage(*q, mutex);
			if (text!=NULL)
				_tprintf(_T("\tread by %s - %s\n"), argv[2], text);
			//Sleep(5000);
		}		
	}
	end(h, q, mutex);
	_tprintf(_T("end\n"));
	return 0;
}

