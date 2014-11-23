﻿// OS_lab3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <winerror.h>
#include <locale.h>
#include <windows.h>
#include <iostream>

typedef enum {
	GP_SUCCESS, GP_IO, GP_REPEAT, GP_PASS, GP_STRONG
} ErrorCode;
typedef enum {
	DIGIT, SL, BL, DLIM, BADC
} Classes;

Classes getClass(TCHAR c) {
	if (c >= _T('0') && c <= _T('9'))
		return DIGIT;
	if (c >= _T('a') && c <= _T('z'))
		return SL;
	if (c >= _T('A') && c <= _T('Z'))
		return BL;
	TCHAR d[] = _T(",_.!?()");
	if (_tcschr(d, c) != 0)
		return DLIM;
	return BADC;
}

ErrorCode checkPass(TCHAR password[]) {
	ErrorCode code;
	int count = 0;
	int mask = 0;
	int len = _tcslen(password);
	if (len < 8) return GP_STRONG;
	for (int i = 0; i < len; i++) {
		if (password[i] == _T('_'))
			count++;
		else {
			Classes cl = getClass(password[i]);
			if (cl == BADC) return GP_PASS;
			mask |= 1 << cl;
		}
	}
	return mask == 15 && count < len / 4 ? GP_SUCCESS : GP_STRONG;
}

bool writePromt(HANDLE h, TCHAR t[]) {
	DWORD d;
	TCHAR endl[] = _T("\r\n");
	bool b = WriteConsole(h, t, _tcslen(t), &d, 0);
	DWORD er = GetLastError();
	if (b)
		b = WriteConsole(h, endl, 2, &d, 0);
	return b;
}

bool readPass(HANDLE h1, HANDLE h2, TCHAR pas[], int ch) {
	TCHAR star = _T('*');
	TCHAR endl[] = _T("\r\n");
	DWORD d; TCHAR t;
	int i; bool b;
	for (int i = 0; i < ch - 1; i++) {
		b = ReadConsole(h1, &t, 1, &d, 0);
		if (t == _T('\r\n')) break;
		b = WriteConsole(h2, &star, 1, &d, 0);
		b = WriteConsole(h2, endl, 1, &d, 0);
	}
	return b;

}
//SecondTask
int getFields(unsigned int extCode, int& code, int& subSystem, int& owner, int& error) {
	code = extCode & 0xFFFF;
	subSystem = (extCode >> 16) & 0xFFFF;
	owner = (extCode >> 20) & 1;
	error = (extCode >> 30) & 3;
	return code;
}
//ThirdTask
unsigned int getExtError(int code, int subSystem, int owner, int error) {
	unsigned int result = error << 30;
	result |= owner << 20;
	result |= subSystem << 16;
	result |= code;
	return result;
}
bool checkErrors(){
	unsigned int extError = (rand() << 17) | (rand() << 2) | rand() & 3;
	int code, subSystem, owner, error;
	getFields(extError, code, subSystem, owner, error);
	unsigned int newError = getExtError(code, subSystem, owner, error);
	return extError == newError;
}
//4TH TASK
void checkMessageBox() {
	int msgboxID = MessageBox(
		0,
		_T("Привіт"),
		_T("Ще не вмерла Україна"),
		MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);
	msgboxID = MessageBox(
		0,
		_T("Ukraine is still alive"),
		_T("HI"),
		MB_ICONINFORMATION | MB_YESNO
		);
	msgboxID = MessageBox(
		0,
		_T("Еще не умерла Украина"),
		_T("Привет"),
		MB_ICONQUESTION | MB_OKCANCEL
		);
	msgboxID = MessageBox(
		0,
		_T("Еще разок"),
		_T("Украина"),
		MB_ICONERROR | MB_OK
		);
	msgboxID = MessageBox(
		0,
		_T("もし-код Энигма  Ю-571 ♀ ♦"),
		_T("もし"),
		MB_ICONQUESTION | MB_OKCANCEL
		);
}

void printConsoleMode(HANDLE h){
	DWORD text;
	GetConsoleMode(h, &text);
	//printf("%d", (text&ENABLE_ECHO_INPUT)==ENABLE_ECHO_INPUT);
	if ((text&ENABLE_QUICK_EDIT_MODE) == ENABLE_QUICK_EDIT_MODE)
		_tprintf(_T("Работа с мышью присутствует\n"));
	if ((text&ENABLE_LINE_INPUT) == ENABLE_LINE_INPUT) {
		_tprintf(_T("Ввод в буфер по кнопке Энтер\n"));
	}
	if ((text&ENABLE_ECHO_INPUT) == ENABLE_ECHO_INPUT)
		_tprintf(_T("Показ символов включен\n"));
	if ((text&ENABLE_INSERT_MODE) == ENABLE_INSERT_MODE)
		_tprintf(_T("Символы записываются начиная с текущего положения курсора\n"));
	if ((text&ENABLE_PROCESSED_INPUT) == ENABLE_PROCESSED_INPUT)
		_tprintf(_T("Сигнал CTRL+C обрабатывается системой и не помещается в буфере ввода\n"));
	if ((text&ENABLE_WINDOW_INPUT) == ENABLE_WINDOW_INPUT)
			_tprintf(_T("Пользователь воздействует, чтобы изменить размер экранного буфера консоли записанный в буфере ввода консоли\n"));
	if ((text&ENABLE_MOUSE_INPUT) == ENABLE_MOUSE_INPUT)
			_tprintf(_T("Если курсор мыши - в пределах границ консольного окна, и окно имеет фокус клавиатуры, события от нажатия мыши, созданные перемещением мыши и нажатием кнопки помещаются в буфере ввода\n"));

}


void workingWithConsole() {
	HANDLE hStdin;
	DWORD fdwSaveOldMode; 
	DWORD consoleMode;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		printf("GetStdHandle");
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		printf("GetConsoleMode");
	printf("SaveDonbassPeople");
	consoleMode = ENABLE_QUICK_EDIT_MODE | ENABLE_LINE_INPUT |
		ENABLE_ECHO_INPUT |
		ENABLE_INSERT_MODE;
	if (!SetConsoleMode(hStdin, consoleMode)) 
		printf("SetConsoleMode");
	printConsoleMode(hStdin);
	/*GetConsoleMode(hStdin, &consoleMode);
	_tprintf(_T("%d"),consoleMode);*/
}

void sevenTask(bool withEcho){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
	if (out == INVALID_HANDLE_VALUE || in == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("error - %d\n"),GetLastError());
		return;
	}
	writePromt(out, _T("Введите свой пароль:"));
	if (!withEcho){
		DWORD mode;
		GetConsoleMode(in, &mode);
		SetConsoleMode(in, mode & (~ENABLE_ECHO_INPUT));
		GetConsoleMode(out, &mode);
		SetConsoleMode(out, mode | ENABLE_LINE_INPUT);
		printConsoleMode(out);
		TCHAR t[258];
		readPass(in, out, t, 256);		
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	//checkMessageBox();
	//checkErrors();
	//workingWithConsole();
	sevenTask(FALSE);
	system("pause");
}
