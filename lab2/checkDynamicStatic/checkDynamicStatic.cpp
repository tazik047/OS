// checkDynamicStatic.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include "../lab2/static.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int inf;
	setlocale(LC_ALL, "Russian");
	printf("Введите число ");
	scanf_s("%i", &inf);
	int p, q;
	int n = genKey(p, q);
	int inf_ = crypt(inf, p, n);
	printf("Зашифрованная информация %d: ", inf_);
	printf("Расшифрованная информация %d: ", crypt(inf_, q, n));
	return 0;
}

