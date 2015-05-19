// Main.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <locale.h>
#include "..\Consumer\ConcurrentQueue.h"

#define CreateUnsuspendedProcess(path, lpsi, lppi, directory) CreateProcess(0, path, 0, 0, 0, 0, 0, directory, lpsi, lppi);


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	int c = -1;
	TCHAR ProcName51[] = _T("Consumer.exe myMapView");
	TCHAR ProcName52[] = _T("Producer.exe myMapView");
	BOOL b;
	queueStruct* q;
	while (c != 0)
	{
		system("cls");
		_tprintf(_T("0. Выйти из программы.\n"));
		_tprintf(_T("1. -------------\n"));
		_tprintf(_T("5. Запустить производителя и потребителя\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
		case 5:
			HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(queueStruct), _T("myMapView"));
			int i = GetLastError();
			if (h == 0)
				return GetLastError();
			queueStruct* q = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			q->end = FALSE;
			b = CreateUnsuspendedProcess(ProcName52, &si, &pi, 0);
			if (!b)
			{
				_tprintf(_T("Не удалось запустить процесс %s"), ProcName51);
				return -1;
			}
			b = CreateUnsuspendedProcess(ProcName51, &si, &pi, 0);
			if (!b)
			{
				_tprintf(_T("Не удалось запустить процесс %s"), ProcName51);
				return -1;
			}
			q->end = TRUE;
			WaitForSingleObject(pi.hProcess, INFINITE);
			if (q){
				UnmapViewOfFile(q);
				q = 0;
			}
			if (h){
				CloseHandle(h);
				h = 0;
			}
			system("pause");
			break;
		}
	}
}

