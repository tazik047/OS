// checkDynamicStatic.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include<iostream>
#include "../lab2/static.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	char text[256];
	printf("Введите текст для шифрования ");
	std::cin.getline(text, 256);
	int len;
	long long p, q, n;
	n = genkey(p, q);
	long long * inf_ = ToLong(text, len);
	inf_ = crypt(inf_, p, n, len);

	printf("Зашифрованная информация:\n");
	for (int i = 0; i < len; i++)
		printf("%d", inf_[i]);
	printf("\n");
	printf("Расшифрованная информация:\n%s\n", ToChar(crypt(inf_, q, n, len), len));
	system("pause");
	return 0;
}

