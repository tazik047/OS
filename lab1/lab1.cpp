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

int compare(const void* e1, const void* e2) {
	TCHAR* s1 = *(TCHAR**)e1;
	TCHAR* s2 = *(TCHAR**)e2;
	return _tcscmp(s1, s2);
}

template<typename T>
void reverse(T* text){
	int len;
	if (sizeof(T) == sizeof(char))
		len = strlen((char*)text) - 1;
	else
		len = wcslen((wchar_t*)text) - 1;
	while (text[len] == '\n' || text[len] == '\r'){
		len--;
	}
	if (len <= 0)
		return;
	for (int i = 0; i < len / 2 +1; i++){
		T temp = text[i];
		text[i] = text[len - i];
		text[len - i] = temp;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian")); // русский в консоли.
	
	_tprintf(_T("Первое задание:\n"));
	_tprintf(_T("\tРазмер tchar: %d\n"), sizeof(TCHAR));
	
	
	
	char buf[LEN_MAS][256];
	int i = 0;
	while (i < LEN_MAS){
		//scanf("%s", buf[i++]);
		cin >> buf[i++];
	}
	TCHAR** _tbuf = new TCHAR*[LEN_MAS];
	_tprintf(_T("\nВторое задание:\n"));
	while (i > 0){
		OemToCharA(buf[--i], buf[i]);
		_tbuf[i] = new wchar_t[sizeof(buf[i])];
		MultiByteToWideChar(CP_ACP, 0, buf[i], sizeof(buf[i]), _tbuf[i], sizeof(buf[i]));
		_tprintf(_T("\t%s\n"), _tbuf[i]);
		MessageBox(0, _tbuf[i], _T("Введенное слово"), MB_OK);
	}

	_tprintf(_T("\nТретье задание:\n"));
	int temp = sizeof(*_tbuf[0]);
	int temp1 = sizeof(int);
	qsort(_tbuf, LEN_MAS, sizeof(_tbuf[0]), compare);
	while (i < LEN_MAS){
		WideCharToMultiByte(CP_ACP, 0, _tbuf[i], 256, buf[i], 256, NULL, NULL);
		printf("\t%s\n", buf[i++]);
	}
	delete _tbuf;
	

	readAndReverse();


	return 0;
}


void readAndReverse(){

	char path[] = "file_unicode.txt";
	_tprintf(_T("\n11 задание:\n"));
	FILE *file = nullptr;
	FILE *w = nullptr;
	int bom[2];
	ifstream f(path, ios_base::binary);
	bom[0] = f.get();
	bom[1] = f.get();
	f.close();
	if ((bom[0] == 0xfe && bom[1] == 0xff) || (bom[0] == 0xff && bom[1] == 0xfe)){
		TCHAR _tPath[100];
		MultiByteToWideChar(CP_ACP, 0, path, sizeof(path), _tPath, sizeof(_tPath));
		TCHAR textWchar_t[256];
		file = _wfopen(_tPath, _T("r,ccs=UTF-16LE"));
		w = _wfopen(_T("resFile.txt"), _T("w,ccs=UTF-16LE"));
		while (!feof(file)){
			wchar_t t[256];
			fgetws(textWchar_t, 255, file);
			reverse(textWchar_t);
			fputws(textWchar_t, w);
		}
	}
	else{
		char textChar[256];
		file = fopen(path, "r");
		w = fopen("resFile.txt", "w");
		while (!feof(file)){
			fgets(textChar, 255, file);
			reverse(textChar);
			fputs(textChar, w);
		}
	}
	fclose(w);
	fclose(file);
}


// -17 -69 -65   --utf8

// -1 -2 unicode