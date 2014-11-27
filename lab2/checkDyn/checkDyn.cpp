// checkDyn.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include<stdlib.h>

typedef int(__stdcall *GENKEY) (int&,int& );
typedef int(__stdcall *CRYPT) (int, int,int);

void doLib(int language, HMODULE languageLib) {
	HMODULE h = LoadLibrary(_T("dynamicLib.dll"));
	if (h != 0)
	{
		if (language == 1)
			setlocale(LC_ALL, "Russian");
		GENKEY genKey = (GENKEY)GetProcAddress(h, "genKey");
		CRYPT crypt = (CRYPT)GetProcAddress(h, "crypt");
		if (genKey  && crypt)
		{
			int inf;
			TCHAR Buf[MAX_PATH];
			LoadString(languageLib, 102, Buf, sizeof(Buf));
			_tprintf(Buf);
			scanf_s("%i", &inf);
			int p, q;
			int n = genKey(p, q);
			int inf_ = crypt(inf, p, n);
			LoadString(languageLib, 103, Buf, sizeof(Buf));
			_tprintf(Buf, inf_);
			LoadString(languageLib, 104, Buf, sizeof(Buf));
			_tprintf(Buf, crypt(inf_, q, n));
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
	scanf_s("%i", &information);
	if (information == 1) {
		lib = LoadLibrary(_T("resourceDll.dll"));
	}
	else if (information == 2) {
		lib = LoadLibrary(_T("resourceDllEnglish.dll"));
		
	}
	else {
		printf("—корее всего, вы ошиблись");
		return -1;
	}

	if (lib != 0) {
			TCHAR Buf[MAX_PATH];
			LoadString(lib, 101, Buf, sizeof(Buf));
			MessageBox(0, Buf, _T("About"), 0);
			doLib(information, lib);
			FreeLibrary(lib);
		}
	return 0;
}


