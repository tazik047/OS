// checkDyn.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include"..\dynamicLib\MY_VAR.h"
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
			unsigned inf;
			setlocale(LC_ALL, "Russian");
			printf("Введите число ");
			scanf_s("%i", &inf);
			unsigned p, q;
			/*unsigned e = genKey(int,int);
			unsigned inf_ = crypt(e, n, inf);
			printf("Зашифрованная информация %d: ", inf_);*/
		}
		FreeLibrary(h);
	}
	else
	{
		return 1;
	}
	return 0;
}
