#include "stdafx.h"
#include <Windows.h>
#include "MyMain.h"
#include "CRC.h"

BOOL createFileMapping();

DWORD count, bytes, max_message, read;
TCHAR path[] = _T("./mailbox/messages.dat");
HANDLE h;
HANDLE hFileMapping; // ������ ���� "�������� �����"
PBYTE pbFile; // ������ ���� ����� �����

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
		_tprintf(_T("��� ��� ����� ������ �������� ����\n������� ������������ ���������� ���������, ������� ����� ������� � ���:\n"));
		size_t maxCount;
		std::cin >> maxCount;
		WriteFile(h, &maxCount, 4, &read, 0);
		if (read != 4){
			printError();
			return FALSE;
		}
	}
	/*else if (!validate(h)){
		_tprintf(_T("���� ��������!!!\n"));
		ExitFromMailBox();
		return FALSE;
		}*/
	if (!createFileMapping()){
		_tprintf(_T("������ �������� �����."));
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
		i += 4;//������� �� ������ �����.
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
	// ���������� � �������� ������������ �������� ������ "�������� �����"
	pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	//print(pbFile);
	return pbFile != 0;
}

void setPosition(int i){
	SetFilePointer(h, i, 0, FILE_END);
	SetEndOfFile(h);
}

void resetPosition(){
	SetFilePointer(h, 0, 0, FILE_END);
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
	_tprintf(_T("\n���������� � �������� �����:\n"));
	_tprintf(_T("���������� ��������� � �����: %d\n"), count);
	_tprintf(_T("���������� ����, ������� �����������: %d\n"), bytes);
	_tprintf(_T("������������ ���������� ���������: %d\n"), max_message);
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
		_tprintf(_T("���� �����. ���������� ���������� �������� ����."));
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

	_tprintf(_T("��������� ���������\n"));
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
	_tprintf(_T("��������� � %d:\n%s\n������ ���������:%d\n"), index, message, size);
}

void deleteTheMessage(int index) {
	getMailBoxInformation();
	if (index<0 || index >= count) {
		_tprintf(_T("You can't delete unexisting info\n"));
		return;
	}
	int message_size;
	/*
	int counter = 0;
	while (ReadFile(h, &message_size, 4, &read, 0)) {
		SetFilePointer(h, message_size, 0, FILE_CURRENT);
		if (counter == index) {
			bytes -= message_size;
			count--;
			break;
		}
		counter++;
	}
	TCHAR* message;
	int prev_size_message = message_size;
	while (TRUE){
		ReadFile(h, &message_size, 4, &read, 0);
		if (read != 4)
			break;
		message = new TCHAR[message_size / sizeof(TCHAR)];
		ReadFile(h, message, message_size, &read, 0);
		message[message_size / sizeof(TCHAR)] = '\0';
		SetFilePointer(h, -(message_size + prev_size_message + 8), 0, FILE_CURRENT);
		WriteFile(h, &message_size, 4, &read, 0);
		WriteFile(h, message, message_size, &read, 0);
		SetFilePointer(h, prev_size_message + 4, 0, FILE_CURRENT);
	}
	SetFilePointer(h, -(prev_size_message + 4), 0, FILE_END);
	SetEndOfFile(h);
	ExitFromMailBox();
	StartMailBox();
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
	}*/

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
		pbFile[i] = 0;
	}
	//TODO: ���� �� ���������, � ���� ����� ���-�� ������.
	setPosition(-message_size - 4);
	ExitFromMailBox();
	StartMailBox();
	writeTToByte(0, count);
	writeTToByte(4, bytes);
}

void clearTheMailBox() {
	resetPosition();
	SetEndOfFile(h);
	CloseHandle(h);
	StartMailBox();
}