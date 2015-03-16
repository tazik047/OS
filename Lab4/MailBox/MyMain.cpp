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
		_tprintf(_T("Для вас будет создан почтовый ящик\nВведите макисмальное количество сообщений, которое можно хранить в нем:\n"));
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
	_tprintf(_T("\nИнформация о почтовом ящике:\n"));
	DWORD count, bytes, max_message, read;
	ReadFile(h, &count, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("Количество сообщений в ящике: %d\n"), count);
	ReadFile(h, &bytes, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("Количество байт, занятое сообщениями: %d\n"));
	ReadFile(h, &bytes, 4, &read, 0);
	if (read != 4){
		printError();
	}
	_tprintf(_T("Максимальное количество сообщений: %d\n"));
	_tprintf(_T("\n"));
}
