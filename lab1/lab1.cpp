// lab1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <locale.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>

#define LEN_MAS 3


using namespace std;

void readAndReverse();
void myReverse(char*);
void myReverse(wchar_t*);

int compare(const void* e1, const void* e2) {
	TCHAR* s1 = *(TCHAR**)e1;
	TCHAR* s2 = *(TCHAR**)e2;
	return _tcscmp(s1, s2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian")); // русский в консоли.
	/*
	_tprintf(_T("Первое задание:\n"));
	_tprintf(_T("\tРазмер tchar: %d\n"), sizeof(TCHAR));
	
	
	
	char buf[LEN_MAS][256];
	int i = 0;
	while (i < LEN_MAS){
		//scanf("%s", buf[i++]);
		cin >> buf[i++];
	}
	char buf1[256];
	TCHAR** _tbuf = new TCHAR*[LEN_MAS];
	_tprintf(_T("\nВторое задание:\n"));
	while (i > 0){
		OemToCharA(buf[--i], buf1);
		_tbuf[i] = new wchar_t[sizeof(buf1)];
		MultiByteToWideChar(CP_ACP, 0, buf1, sizeof(buf1), _tbuf[i], sizeof(buf1));
		_tprintf(_T("\t%s\n"), _tbuf[i]);
		MessageBox(0, _tbuf[i], _T("Введенное слово"), MB_OK);
	}

	_tprintf(_T("\nТретье задание:\n"));
	qsort(_tbuf, LEN_MAS, sizeof(_tbuf[0]), compare);
	while (i < LEN_MAS){
		WideCharToMultiByte(CP_ACP, 0, _tbuf[i], 256, buf[i], 256, NULL, NULL);
		printf("\t%s\n", buf[i++]);
	}
	delete _tbuf;
	*/

	readAndReverse();


	return 0;
}


void readAndReverse(){
	char path[] = "file.txt";
	_tprintf(_T("\n11 задание:\n"));
	char textChar[256];
	wchar_t textWchar_t[256];
	FILE *fChar = fopen(path, "r");// открытие файла
	FILE *w = fopen("resFile.txt", "w");
	fgets(textChar, 255, fChar);
	int t = IS_TEXT_UNICODE_UNICODE_MASK;
	if (!IsTextUnicode(textChar, sizeof(textChar), &t)){
		
		while (!feof(fChar)){
			printf("%s", textChar);
			//MultiByteToWideChar(CP_ACP, 0, textChar, sizeof(textChar), textWchar_t, sizeof(textWchar_t));
			myReverse(textChar);
			fputs(textChar, w);
		}
	}
	else{
		FILE *fWchar_t = fopen(path, "r");
		while (!feof(fWchar_t)){
			fgetws(textWchar_t, 255, fWchar_t);
			wprintf(L"%s", textWchar_t);
			myReverse(textWchar_t);
			fputws(textWchar_t, w);
		}
		fclose(fWchar_t);
		//fgets(text, 256, my_file); //считываем строку разимером максимум
		// 100 символов из файла
		//puts(text); // вывод (необязательно)
		fclose(fChar);
		fclose(w);
	}
}

void myReverse(char* text){
	wchar_t newText[sizeof(text)];
	MultiByteToWideChar(CP_ACP, 0, text, sizeof(text), newText, sizeof(text));
	myReverse(newText);
}

void myReverse(wchar_t* text){
	int len = _tcslen(text);
	if (text[len - 1] == '\n')
		len--;
	wprintf(L" %d\n", len);
}
