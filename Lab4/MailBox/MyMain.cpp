#include "stdafx.h"
#include <Windows.h>
#include "MyMain.h"

TCHAR path[] = _T("./mailbox/messages.dat");
HANDLE h;

void printError(){
	_tprintf(_T("Error, when writting in file\n"));
	return;
}

BOOL StartMailBox(){
	h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	if (h == INVALID_HANDLE_VALUE){
		_tprintf(_T("Error when creating/open files\n"));
		return FALSE;
	}
	DWORD  max;
	size_t t = GetFileSize(h, &max);
	if (t == 0){
		DWORD written;
		DWORD zero = 0;
		WriteFile(h, &zero, 4, &written, 0);
		if (written != 4){
			printError();
			return FALSE;
		}
		WriteFile(h, &zero, 4, &written, 0);
		if (written != 4){
			printError();
			return FALSE;
		}
		_tprintf(_T("��� ��� ����� ������ �������� ����\n������� ������������ ���������� ���������, ������� ����� ������� � ���:\n"));
		size_t maxCount;
		std::cin >> maxCount;
		WriteFile(h, &maxCount, 4, &written, 0);
		if (written != 4){
			printError();
			return FALSE;
		}
	}
	return TRUE;
}

// svn://jtrain.ddns.net:9003

void resetPosition(){
	SetFilePointer(h, 0, 0, FILE_BEGIN);
}

void ExitFromMailBox(){
	CloseHandle(h);
}

void getMailBoxInformation(){
	resetPosition();
	_tprintf(_T("\n���������� � �������� �����:\n"));
	DWORD count, bytes, max_message, read;
	ReadFile(h, &count, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("���������� ��������� � �����: %d\n"), count);
	ReadFile(h, &bytes, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("���������� ����, ������� �����������: %d\n"), bytes);
	ReadFile(h, &max_message, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("������������ ���������� ���������: %d\n"), max_message);
	_tprintf(_T("\n"));
}

void doError(HANDLE h, int mas[],DWORD &read, int mess_size, TCHAR* mess, int mode){
	switch (mode) {
	case 0:
		WriteFile(h, &mas[0], 4, &read, 0);
		if (read != 4){
			printError();
			return;
		}
		break;
	case 1:
		WriteFile(h, &mess_size, 4, &read, 0);
		if (read != 4){
			printError();
			return;
		}
		break;
	case 2:
		WriteFile(h, &mess, mess_size, &read, 0);
		if (read != mess_size){
			printError();
			return;
		}
		break;
	}
}

void addNewMessage(TCHAR* mess){
	int mas[3];
	DWORD read;
	resetPosition();
	ReadFile(h, &mas, 4 * 3, &read, 0);
	if (mas[0] == mas[2]){
		_tprintf(_T("���� �����. ���������� ���������� �������� ����."));
		return;
	}
	mas[0]++;
	int mess_size = sizeof(TCHAR)*_tcslen(mess);
	mas[1] += mess_size;
	resetPosition();
	doError(h, &mas[1], read, mess_size, mess,0);
	doError(h, &mas[1], read, mess_size,mess,0);
	SetFilePointer(h, 0, 0, FILE_END);
	doError(h, &mas[1], read, mess_size, mess, 1);
	doError(h, &mas[1], read, mess_size, mess, 2);
	_tprintf(_T("��������� ���������\n"));
}

