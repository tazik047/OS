// OS_lab3.cpp: определяет точку входа для консольного приложения.
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
	int count = 0;
	int mask = 0;
	int len = _tcslen(password);
	if (len < 8) return GP_STRONG;
	for (int i = 0; i < len; i++) {
		if (password[i] == _T(' '))
			count++;
		else {
			Classes cl = getClass(password[i]);
			if (cl == BADC) return GP_PASS;
			mask |= 1 << cl;
		}
	}
	return mask == 15 && count < len / 4 ? GP_SUCCESS : GP_STRONG;
}

BOOL writePromt(HANDLE h, TCHAR t[]) {
	DWORD d;
	TCHAR endl[] = _T("\r\n");
	BOOL b = WriteConsole(h, t, _tcslen(t), &d, 0);
	DWORD er = GetLastError();
	if (b)
		b = WriteConsole(h, endl, 2, &d, 0);
	return b;
}

BOOL readPass(HANDLE h1, HANDLE h2, TCHAR pas[], int ch, BOOL writeStar) {
	TCHAR star = _T('*');
	TCHAR endl[] = _T("\r\n");
	DWORD d; TCHAR t;
	BOOL b;
	for (int i = 0; i < ch - 1; i++) {
		b = ReadConsole(h1, &t, 1, &d, 0);
		if (t == '\b') {
			CONSOLE_SCREEN_BUFFER_INFO pointer;
			GetConsoleScreenBufferInfo(h2, &pointer);
			pointer.dwCursorPosition.X -= 1;
			SetConsoleCursorPosition(h2, pointer.dwCursorPosition);
			WriteConsole(h2, _T(" "), 1, 0, 0);
			SetConsoleCursorPosition(h2, pointer.dwCursorPosition);
			i-=2;
			if (i == 0)
				i = -1;
			pas[i+1] = '\0';
			continue;
		}
		if (t == _T('\r\n')){
			pas[i] = '\0';
			break;
		}
		pas[i] = t;
		if (writeStar)
			b = WriteConsole(h2, &star, 1, &d, 0);
	}
	b = WriteConsole(h2, endl, 2, &d, 0);
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

void tcharToInt(TCHAR t[], int& i) {
	i = 0;
	int count = 0;
	for (int k = 0; k < _tcslen(t); k++) {
		if (t[k] == '\r') {
			break;
		}
		if (t[k] - '0'<0 || t[k] - '0'>9) {
			printf("Не число!");
			return;
		}
		count++;
	}
	for (int j = 0; j < count; j++) {
		i = i * 10 + (t[j] - '0');
	}
}

void outputDigit(int digit) {

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL b;
	DWORD d;
	TCHAR buf[256];
	_stprintf_s(buf, _T("%d"), digit);
	b = WriteConsole(h, buf, _tcslen(buf), &d, 0);
	printf("\n");
}

int inputDigit() {
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
	bool b;
	DWORD d;
	TCHAR buf[256];
	b = ReadConsole(h, buf, _tcslen(buf), &d, 0);
	int digit=0;
	tcharToInt(buf, digit);
	return digit;
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
	//inputDigit();
	/*GetConsoleMode(hStdin, &consoleMode);
	_tprintf(_T("%d"),consoleMode);*/
}

void sevenTask(BOOL withEcho){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
	TCHAR pass1[258];
	if (out == INVALID_HANDLE_VALUE || in == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("error - %d\n"),GetLastError());
		return;
	}
	writePromt(out, _T("Введите свой пароль:"));
	if (withEcho)
	{
		readPass(in, out, pass1, 256, FALSE);
	}
	else{
		DWORD mode; TCHAR pass2[258];
		GetConsoleMode(in, &mode);
		BOOL an = SetConsoleMode(in, (mode & (~ENABLE_ECHO_INPUT) &(~ENABLE_LINE_INPUT)) | ENABLE_INSERT_MODE);
		readPass(in, out, pass1, 256, TRUE);
		writePromt(out, _T("Повторите ваш пароль:"));
		readPass(in, out, pass2, 256, TRUE);
		if (_tcslen(pass1) != _tcslen(pass2))
		{
			writePromt(out, _T("Пароли не совпадают"));
			sevenTask(withEcho);
			return;
		}
		for (int i = 0; i < _tcslen(pass1); i++)
		{
			if (pass1[i] != pass2[i]){
				writePromt(out, _T("Пароли не совпадают"));
				sevenTask(withEcho);
				return;
			}
		}
	}
	switch (checkPass(pass1)){
		case GP_SUCCESS:
			writePromt(out, _T("Пароль строгий"));
			break;
		case GP_STRONG:
			writePromt(out, _T("Пароль не строгий"));
			break;

		}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	//checkMessageBox();
	//checkErrors();
	//workingWithConsole();
	sevenTask(0);
	//int a = inputDigit();
	//outputDigit(a);
	system("pause");
}