// checkDyn.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include<stdlib.h>

typedef int(__stdcall *GENKEY) (int&,int& );
typedef int(__stdcall *CRYPT) (int, int,int);

void doRussian(int language) {
	HMODULE h = LoadLibrary(_T("dynamicLib.dll"));
	if (h != 0)
	{

		GENKEY genKey = (GENKEY)GetProcAddress(h, "genKey");
		CRYPT crypt = (CRYPT)GetProcAddress(h, "crypt");
		if (genKey  && crypt)
		{
			int inf;
			if (language == 1) {
				setlocale(LC_ALL, "Russian");

				printf("������� ����� ");
				scanf_s("%i", &inf);
				int p, q;
				int n = genKey(p, q);
				int inf_ = crypt(inf, p, n);
				printf("������������� ���������� %d: ", inf_);
				printf("�������������� ���������� %d: ", crypt(inf_, q, n));
				system("pause");
			}
			else {
				printf("Enter the digit ");
				scanf_s("%i", &inf);
				int p, q;
				int n = genKey(p, q);
				int inf_ = crypt(inf, p, n);
				printf("Crypt information %d: ", inf_);
				printf("Decrypt information %d: ", crypt(inf_, q, n));
				system("pause");
			}
		}
		FreeLibrary(h);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	printf("�� ����� ����� ������� �������� ����������? (1-�������, 2 - ����������)    ");
	int information;
	scanf_s("%i", &information);
	if (information == 1) {
		HMODULE lib = LoadLibrary(_T("resourceDll.dll"));
		if (lib != 0) {
			TCHAR Buf[MAX_PATH];
			LoadString(lib, 101, Buf, sizeof(Buf));
			MessageBox(0, Buf, _T("About"), 0);
			doRussian(information);
			FreeLibrary(lib);
		}
	}
	else if (information == 2) {
		HMODULE lib = LoadLibrary(_T("resourceDllEnglish.dll"));
		if (lib != 0) {
			TCHAR Buf[MAX_PATH];
			LoadString(lib, 101, Buf, sizeof(Buf));
			MessageBox(0, Buf, _T("About"), 0);
			doRussian(information);
			FreeLibrary(lib);
		}
	}
	else {
		printf("������ �����, �� ��������");
	}
	/*HMODULE h = LoadLibrary(_T("dynamicLib.dll"));
	if (h != 0)
	{

		GENKEY genKey = (GENKEY)GetProcAddress(h, "genKey");
		CRYPT crypt = (CRYPT)GetProcAddress(h, "crypt");
		if (genKey  && crypt)
		{
			int inf;
			setlocale(LC_ALL, "Russian");
			printf("������� ����� ");
			scanf_s("%i", &inf);
			int p, q;
			int n = genKey(p,q);
			int inf_ = crypt(inf, p, n);
			printf("������������� ���������� %d: ", inf_);
			printf("�������������� ���������� %d: ", crypt(inf_, q,n ));
			system("pause");
		}
		FreeLibrary(h);
	}
	else
	{
		return 1;
	}*/

	return 0;
}


