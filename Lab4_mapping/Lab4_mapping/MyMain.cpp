#include "stdafx.h"
#include <Windows.h>
#include "MyMain.h"
#include "CRC.h"

BOOL createFileMapping();

DWORD count, bytes, max_message, read;
TCHAR path[] = _T("./mailbox/messages.dat");
HANDLE h;
HANDLE hFileMapping; // объект ядра "проекция файла"
PBYTE pbFile; // массив байт всего файла

void printError(){
	_tprintf(_T("Error, when writting in file1\n"));
	return;
}

BOOL OpenMyFile(BOOL& newFile){
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
		newFile =  TRUE;
	}
}

BOOL StartMailBox(){
	BOOL newFile = FALSE;
	if (!OpenMyFile(newFile))
		return FALSE;
	/*else if (!newFile && !validate(h)){
		_tprintf(_T("Файл испорчен!!!\n"));
		ExitFromMailBox();
		return FALSE;
		}*/
	if (!createFileMapping()){
		_tprintf(_T("Ошибка открытия файла."));
		ExitFromMailBox();
		return FALSE;
	}
	return TRUE;
}

template <typename T>
T byteToT(int start)
{
	BYTE num[4];
	T res = 0;
	for (int i = start + sizeof(T)-1; i >= start; i--){
		res = (res << 8) | pbFile[i];
	}
	return res;
}

template <typename T>
void writeTToByte(int start, T value){
	for (int i = start; i < start + sizeof(TCHAR); i++){
		pbFile[i] = value & MAXBYTE;
		value >>= 8;
	}
}

void print(PBYTE pb){
	printTheInfo();
	for (int i = 12; i < GetFileSize(h, 0);){
		int size = byteToT<int>(i) / sizeof(TCHAR);
		i += 4;//сдвинул на размер числа.
		TCHAR* str = new TCHAR[size + 1];
		str[size] = '\0';
		for (int j = 0; j < size; j++){
			str[j] = byteToT<TCHAR>(i);
			i += sizeof(TCHAR);
		}
		_tprintf(_T("%d - %s\n"), size*sizeof(TCHAR), str);
	}
}

BOOL createFileMapping()
{
	hFileMapping = CreateFileMapping(h, 0, PAGE_READWRITE, 0, 0, 0);
	if (hFileMapping == 0)
		return FALSE;
	// Проецируем в адресное пространство процесса объект "проекция файла"
	pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	return pbFile != 0;
}

void setPosition(int i){
	SetFilePointer(h, i, 0, FILE_END);
	SetEndOfFile(h);
}

void resetPosition(){
	SetFilePointer(h, 0, 0, FILE_BEGIN);
}

void ExitFromMailBox(){
	if (pbFile){
		UnmapViewOfFile(pbFile);
		pbFile = 0;
	}
	if (hFileMapping){
		CloseHandle(hFileMapping);
		hFileMapping = 0;
	}
	//createCRC(h);
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
	count = byteToT<int>(0);
	bytes = byteToT<int>(4);
	max_message = byteToT<int>(8);
}

void addNewMessage(TCHAR* mess){
	getMailBoxInformation();
	if (count == max_message){
		_tprintf(_T("Ящик полон. Необходимо освободить почтовый ящик."));
		return;
	}
	count++;
	int mess_size = sizeof(TCHAR)*_tcslen(mess);
	bytes += mess_size;

	writeTToByte(0, count);
	writeTToByte(4, bytes);
	int end = GetFileSize(h, 0);
	setPosition(mess_size + 4);
	writeTToByte(end, mess_size);
	end += 4;
	for (int i = 0; i < mess_size / sizeof(TCHAR); i++){
		writeTToByte(end, mess[i]);
		end += sizeof(TCHAR);
	}

	_tprintf(_T("Сообщение добавлено\n"));
}

void ReadMessage(int index){
	getMailBoxInformation();
	if (index < 0 || index >= count) {
		_tprintf(_T("You can't read unexisting info\n"));
		return;
	}
	DWORD message_size;
	TCHAR* message;
	int counter = 0;
	int ind = 12;
	while (counter != index){
		ind += byteToT<int>(ind) + 4;
		counter++;
	}
	int size = byteToT<int>(ind);
	ind += 4;
	message = new TCHAR[size / sizeof(TCHAR) + 1];
	message[size / sizeof(TCHAR)] = '\0';
	for (int i = 0; i < size / sizeof(TCHAR); i++){
		message[i] = byteToT<TCHAR>(ind);
		ind += sizeof(TCHAR);
	}
	_tprintf(_T("Сообщение № %d:\n%s\nРазмер сообщения:%d\n"), index, message, size);
}

void deleteTheMessage(int index) {
	getMailBoxInformation();
	if (index<0 || index >= count) {
		_tprintf(_T("You can't delete unexisting info\n"));
		return;
	}
	int message_size;

	int counter = 0;
	int ind = 12;
	while (counter != index){
		ind += byteToT<int>(ind) +4;
		counter++;
	}
	message_size = byteToT<int>(ind);
	bytes -= message_size;
	count--;
	int end = GetFileSize(h,0);
	for (int i = ind + message_size + 4; i < end; i++){
		pbFile[i - message_size - 4] = pbFile[i];
	}
	writeTToByte(0, count);
	writeTToByte(4, bytes);
	
	//Уменьшение размера файла.
	BOOL newFile;
	ExitFromMailBox();
	OpenMyFile(newFile);
	setPosition(-message_size - 4);
	ExitFromMailBox();
	StartMailBox();
	
}

void clearTheMailBox() {
	BOOL newFile;
	ExitFromMailBox();
	OpenMyFile(newFile);
	resetPosition();
	SetEndOfFile(h);
	ExitFromMailBox();
	StartMailBox();
}

void reduceFile(int size){
	BOOL newFile;
	ExitFromMailBox();
	OpenMyFile(newFile);
	setPosition(-size);
	ExitFromMailBox();
	StartMailBox();
}