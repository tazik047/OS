// checkDyn.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include<stdlib.h>

typedef int(__stdcall *GENKEY) (int&,int& );
typedef int(__stdcall *CRYPT) (int, int,int);



int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE h = LoadLibrary(_T("dynamicLib.dll"));
	if (h != 0)
	{

		GENKEY genKey = (GENKEY)GetProcAddress(h, "genKey");
		CRYPT crypt = (CRYPT)GetProcAddress(h, "crypt");
		if (genKey  && crypt)
		{
			int inf;
			setlocale(LC_ALL, "Russian");
			printf("Введите число ");
			scanf_s("%i", &inf);
			int p, q;
			int n = genKey(p,q);
			int inf_ = crypt(inf, p, n);
			printf("Зашифрованная информация %d: ", inf_);
			printf("Расшифрованная информация %d: ", crypt(inf_, q,n ));
		}
		FreeLibrary(h);
	}
	else
	{
		return 1;
	}
	return 0;
}


