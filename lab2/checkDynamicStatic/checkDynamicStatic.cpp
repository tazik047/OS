// checkDynamicStatic.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include<Windows.h>
#include<locale.h>
#include "../lab2/static.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int inf;
	setlocale(LC_ALL, "Russian");
	printf("������� ����� ");
	scanf_s("%i", &inf);
	int p, q;
	int n = genKey(p, q);
	int inf_ = crypt(inf, p, n);
	printf("������������� ���������� %d: ", inf_);
	printf("�������������� ���������� %d: ", crypt(inf_, q, n));
	return 0;
}

