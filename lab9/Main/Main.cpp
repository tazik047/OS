// Main.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	int c = 5;
	TCHAR ProcName3[] = _T("Laba9_OS.exe");
	TCHAR ProcName4[]  = _T("TheFourthTask");
	TCHAR ProcName51[] = _T("Consumer.exe myMapView");
	TCHAR ProcName52[] = _T("Producer.exe myMapView");
	TCHAR temp[256];
	BOOL b;
	queueStruct* q;
	const int maxProducer = 10;
	const int maxConsumer = 5;
	HANDLE hProducers[maxProducer];
	HANDLE hConsumer[maxConsumer];
	while (c != 0)
	{
		system("cls");
		_tprintf(_T("0. ����� �� ���������.\n"));
		_tprintf(_T("2. ��������� 10 ����� � ������� � ����.\n"));
		_tprintf(_T("3. ��������� 10 ��������� � ������� � ����.\n"));
		_tprintf(_T("4. ��������� ������������ ���������� �������\n"));
		_tprintf(_T("5. ��������� ������������� � �����������\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
		case 2:
			_stprintf_s(temp, _T("%s %d"), ProcName3, 1);
			b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
			if (!b)
			{
				_tprintf(_T("�� ������� ��������� ������� %s"), temp);
				return -1;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			break;
		case 3:
			_stprintf_s(temp, _T("%s %d"), ProcName3, 2);
			b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
			if (!b)
			{
				_tprintf(_T("�� ������� ��������� ������� %s"), temp);
				return -1;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			break;
		case 4:
			b = CreateUnsuspendedProcess(ProcName4, &si, &pi, 0);
			if (!b)
			{
				_tprintf(_T("�� ������� ��������� ������� %s"), ProcName4);
				return -1;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			break;
		case 5:
			HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(queueStruct), _T("myMapView"));
			int i = GetLastError();
			if (h == 0)
				return GetLastError();
			queueStruct* q = (queueStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			HANDLE mutex = start();
			q->end = FALSE;
			for (int i = 0; i < maxProducer; i++){
				_stprintf_s(temp, _T("%s %d"), ProcName52, i);
				b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("�� ������� ��������� ������� %s"), temp);
					return -1;
				}
				hProducers[i] = pi.hProcess;
			}
			for (int i = 0; i < maxConsumer; i++){
				_stprintf_s(temp, _T("%s %d"), ProcName51, i);
				b = CreateUnsuspendedProcess(temp, &si, &pi, 0);
				if (!b)
				{
					_tprintf(_T("�� ������� ��������� ������� %s"), temp);
					return -1;
				}
				hConsumer[i] = pi.hProcess;
			}
			WaitForMultipleObjects(maxProducer, hProducers, TRUE, INFINITE);
			q->end = TRUE;
			for (int i = 0; i < maxProducer; i++)
				CloseHandle(hProducers[i]);
			WaitForMultipleObjects(maxConsumer, hConsumer, TRUE, INFINITE);
			for (int i = 0; i < maxConsumer; i++)
				CloseHandle(hConsumer[i]);
			end(h, q, mutex);
			system("pause");
			break;
		}
	}
}

