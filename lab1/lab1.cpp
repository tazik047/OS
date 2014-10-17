// lab1.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	_tsetlocale(LC_ALL, _T("Russian")); // ������� � �������.
	/*
	_tprintf(_T("������ �������:\n"));
	_tprintf(_T("\t������ tchar: %d\n"), sizeof(TCHAR));
	
	
	
	char buf[LEN_MAS][256];
	int i = 0;
	while (i < LEN_MAS){
		//scanf("%s", buf[i++]);
		cin >> buf[i++];
	}
	char buf1[256];
	TCHAR** _tbuf = new TCHAR*[LEN_MAS];
	_tprintf(_T("\n������ �������:\n"));
	while (i > 0){
		OemToCharA(buf[--i], buf1);
		_tbuf[i] = new wchar_t[sizeof(buf1)];
		MultiByteToWideChar(CP_ACP, 0, buf1, sizeof(buf1), _tbuf[i], sizeof(buf1));
		_tprintf(_T("\t%s\n"), _tbuf[i]);
		MessageBox(0, _tbuf[i], _T("��������� �����"), MB_OK);
	}

	_tprintf(_T("\n������ �������:\n"));
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

	char path[] = "file_unicode.txt";
	_tprintf(_T("\n11 �������:\n"));
	FILE *file = nullptr;
	FILE *w = nullptr;
	int bom[2];
	ifstream f(path, ios_base::binary);
	bom[0] = f.get();
	bom[1] = f.get();
	f.close();
	if ((bom[0] == 0xfe && bom[1] == 0xff) || (bom[0] == 0xff && bom[1] == 0xfe)){
		TCHAR _tPath[100];
		MultiByteToWideChar(CP_ACP, 0, path, strlen(path), _tPath, _tclen(_tPath));
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


// -17 -69 -65   --utf8

// -1 -2 unicode