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
	TCHAR temp[256];
	BOOL b;
	queueStruct* q;
	HANDLE hProducers[10];
	HANDLE hConsumer[5];
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
			HANDLE mutex = start();
			q->end = FALSE;
			for (int i = 0; i < 10; i++){
				_stprintf_s(temp, _T("%s %d"), ProcName52, i);
				b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("Не удалось запустить процесс %s"), temp);
					return -1;
				}
				hProducers[i] = pi.hProcess;
			}
			for (int i = 0; i < 5; i++){
				_stprintf_s(temp, _T("%s %d"), ProcName51, i);
				b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("Не удалось запустить процесс %s"), temp);
					return -1;
				}
				hConsumer[i] = pi.hProcess;
			}
			WaitForMultipleObjects(10, hProducers, TRUE, INFINITE);
			q->end = TRUE;
			WaitForMultipleObjects(5, hConsumer, TRUE, INFINITE);
			end(h, q, mutex);
			system("pause");
			break;
		}
	}
}

