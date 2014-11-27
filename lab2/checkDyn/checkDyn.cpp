// checkDyn.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include<iostream>
#include<stdlib.h>

typedef long long (__stdcall *GENKEY) (long long&,long long& );
typedef long long* (__stdcall *CRYPT) (long long*, long long, long long, int);
typedef long long* (__stdcall *TOLONG) (char*, int&);
typedef char* (__stdcall *TOCHAR) (long long*, int);

void doLib(int language, HMODULE languageLib) {
	HMODULE h = LoadLibrary(_T("dynamicLib.dll"));
	if (h != 0)
	{
		if (language == 1)
			setlocale(LC_ALL, "Russian");
		GENKEY genkey = (GENKEY)GetProcAddress(h, "genkey");
		CRYPT crypt = (CRYPT)GetProcAddress(h, "crypt");
		TOLONG ToLong = (TOLONG)GetProcAddress(h, "ToLong");
		TOCHAR ToChar = (TOCHAR)GetProcAddress(h, "ToChar");

		if (genkey  && crypt && ToChar && ToLong)
		{
			CHAR Buf[MAX_PATH];
			char* text = new char[256];
			LoadStringA(languageLib, 102, Buf, sizeof(Buf));
			printf(Buf, 256);
			scanf_s("%s", text, 256);
			int len;
			long long p, q, n;
			n = genkey(p, q);
			long long * inf_ = ToLong(text, len);
			inf_ = crypt(inf_, p, n, len);
			LoadStringA(languageLib, 103, Buf, sizeof(Buf));
			printf(Buf);
			for (int i = 0; i < len; i++)
				printf("%d", inf_[i]);
			printf("\n");
			LoadStringA(languageLib, 104, Buf, sizeof(Buf));
			printf(Buf, ToChar(crypt(inf_, q, n, len), len));
			delete[] text;
			system("pause");

		}
		FreeLibrary(h);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	printf("Ќа каком €зыке желаете получать информацию? (1-русский, 2 - английский)    ");
	int information;
	HMODULE lib;
	std::cin >> information;
	if (information == 1)
		lib = LoadLibrary(_T("resourceDll.dll"));
	else if (information == 2)
		lib = LoadLibrary(_T("resourceDllEnglish.dll"));
	else {
		printf("—корее всего, вы ошиблись");
		return -1;
	}

	if (lib != 0) {
			CHAR Buf[MAX_PATH];
			LoadStringA(lib, 101, Buf, sizeof(Buf));
			printf(Buf);
			printf("\n");
			doLib(information, lib);
			FreeLibrary(lib);
		}
	return 0;
}


