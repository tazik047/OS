#include "stdafx.h"
#include <Windows.h>
#include "MyMain.h"
#include "CRC.h"

DWORD count, bytes, max_message, read;
TCHAR path[] = _T("./mailbox/messages.dat");
HANDLE h;

void printError(){
	_tprintf(_T("Error, when writting in file1\n"));
	return;
}

BOOL StartMailBox(){
	h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	if (h == INVALID_HANDLE_VALUE){
		_tprintf(_T("Error when creating/open files\n"));
		return FALSE;
	}
	DWORD  max, t = GetFileSize(h, &max);
	if (t == 0 && max == 0){
		DWORD zero = 0;
		WriteFile(h, &zero, 4, &read, 0);
		if (read != 4){
			printError();
			return FALSE;
		}
		WriteFile(h, &zero, 4, &read, 0);
		if (read != 4){
			printError();
			return FALSE;
		}
		_tprintf(_T("Для вас будет создан почтовый ящик\nВведите макисмальное количество сообщений, которое можно хранить в нем:\n"));
		size_t maxCount;
		std::cin >> maxCount;
		WriteFile(h, &maxCount, 4, &read, 0);
		if (read != 4){
			printError();
			return FALSE;
		}
	}
	else if (!validate(h)){
		_tprintf(_T("Файл испорчен!!!\n"));
		return FALSE;
	}
	return TRUE;
}

// svn://jtrain.ddns.net:9003

void resetPosition(){
	SetFilePointer(h, 0, 0, FILE_BEGIN);
}

void ExitFromMailBox(){
	createCRC(h);
	CloseHandle(h);
}

void printTheInfo() {
	getMailBoxInformation();
	_tprintf(_T("\nИнформация о почтовом ящике:\n"));
	_tprintf(_T("Количество сообщений в ящике: %d\n"), count);
	_tprintf(_T("Количество байт, занятое сообщениями: %d\n"), bytes);
	_tprintf(_T("Максимальное количество сообщений: %d\n"), max_message);
	_tprintf(_T("\n"));
}

void getMailBoxInformation(){
	resetPosition();
	ReadFile(h, &count, 4, &read, 0);
	if (read != 4){
		printError();
	}
	ReadFile(h, &bytes, 4, &read, 0);
	if (read != 4){
		printError();
	}
	ReadFile(h, &max_message, 4, &read, 0);
	if (read != 4){
		printError();
	}
}

void addNewMessage(TCHAR* mess){
	resetPosition();
	int mas[3];
	ReadFile(h, &mas, 4 * 3, &read, 0);
	if (mas[0] == mas[2]){
		_tprintf(_T("Ящик полон. Необходимо освободить почтовый ящик."));
		return;
	}
	mas[0]++;
	int mess_size = sizeof(TCHAR)*_tcslen(mess);
	mas[1] += mess_size;
	resetPosition();
	WriteFile(h, &mas[0], 4, &read, 0);
	if (read != 4){
		printError();
		return;
	}

	WriteFile(h, &mas[1], 4, &read, 0);
	if (read != 4){
		printError();
		return;
	}

	SetFilePointer(h, 0, 0, FILE_END);
	WriteFile(h, &mess_size, 4, &read, 0);
	if (read != 4){
		printError();
		return;
	}
	WriteFile(h, mess, mess_size, &read, 0);
	if (read != mess_size){
		printError();
		return;
	}

	_tprintf(_T("Сообщение добавлено\n"));
}

void ReadMessage(int index){
	getMailBoxInformation();
	if (index >= count) {
		_tprintf(_T("You can't read unexisting info\n"));
		return;
	}
	DWORD message_size;
	TCHAR* message;
	int counter = 0;
	while (ReadFile(h, &message_size, 4, &read, 0)) {
		message = new TCHAR[message_size / sizeof(TCHAR)+1];
		ReadFile(h, message, message_size, &read, 0);
		message[message_size / sizeof(TCHAR)] = '\0';
		if (counter == index) {
			_tprintf(_T("Сообщение № %d:\n%s\nРазмер сообщения:%d\n"), index, message, message_size);
			return;
		}
		counter++;
		delete message;
	}
}

void deleteTheMessage(int index) {
	getMailBoxInformation();
	if (index >= count) {
		_tprintf(_T("You can't read unexisting info\n"));
		return;
	}
	int message_size;
	TCHAR* message;
	int counter = 0;
	while (ReadFile(h, &message_size, 4, &read, 0)) {
		message = new TCHAR[message_size / sizeof(TCHAR) + 1];
		ReadFile(h, message, message_size, &read, 0);
		message[message_size / sizeof(TCHAR)] = '\0';
		if (counter == index) {
			bytes -= message_size;
			count--;
			break;
		}
		counter++;
		delete message;
	}
	TCHAR* buffer;
	int prev_size_message = message_size;
	while (TRUE){
		message = new TCHAR[message_size / sizeof(TCHAR) + 1];
		ReadFile(h, message, message_size, &read, 0);
		if (read != 4)
			break;
		message[message_size / sizeof(TCHAR)] = '\0';
		SetFilePointer(h, -(message_size + prev_size_message + 8), 0, FILE_CURRENT);
		WriteFile(h, &message_size, 4, &read, 0);
		WriteFile(h, &message, message_size, &read, 0);
		SetFilePointer(h, prev_size_message + 4, 0, FILE_CURRENT);
		
	}

	SetEndOfFile(h);
	resetPosition();
	WriteFile(h, &count, 4, &read, 0);
	if (read != 4){
		printError();
		return;
	}

	WriteFile(h, &bytes, 4, &read, 0);
	if (read != 4){
		printError();
		return;
	}
}

void clearTheMailBox() {
	DeleteFile(path);
	StartMailBox();
}