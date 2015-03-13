#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "MyMain.h"

TCHAR path[] = _T("./mailbox/messages.dat");
HANDLE h;

void printError(){
	_tprintf(_T("Error, when writting in file"));
	return;
}

BOOL StartMailBox(){
	h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (h == INVALID_HANDLE_VALUE){
		_tprintf(_T("Error when creating/open files"));
		return FALSE;
	}
	if (GetFileSize(h, 0) == 0){
		DWORD written;

		WriteFile(h, &_T("0"), 4, &written, 0);
		if (written != 4){
			printError();
			return FALSE;
		}
		WriteFile(h, &_T("0"), 4, &written, 0);
		if (written != 4){
			printError();
			return FALSE;
		}
		_tprintf(_T("ƒл€ вас будет создан почтовый €щик\n¬ведите макисмальное количество сообщений, которое можно хранить в нем: "));
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

void ExitFromMailBox(){
	CloseHandle(h);
}
