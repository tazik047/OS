#include "stdafx.h"
#include "Methods.h"
#include <Windows.h>
#include "LRU.h"
#include <locale>

void genSwapPages() {
	_tprintf(_T("�������� �� ���������. 100, 541, 6544, 100, 300, 7000, 1333, 1333, 100\n ������������ ������ - 5"));
	SIZE_T mas[] = { 100, 541, 6544, 100, 300, 7000, 1333, 1333, 100 };
	for (int i = 0; i < sizeof(mas) / sizeof(SIZE_T); i++)
	{
		addToPages(mas[i]);
	}
}
void genLRU() {
	_tprintf(_T("�������� � �������� 0, 1, 0, 2, 3, 0, 3, 1\n"));
	int myM[] = { 0, 1, 0, 2, 3, 0, 3, 1 };
	int size = sizeof(myM) / sizeof(int);
	for (int i = 0; i < size; i++)
	{
		put(myM[i]);
		printCache();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	_tprintf(_T("��� ������� ���������:\n"));
	size_t c = -1;
	int count = 0;
	while (c != 0){
		_tprintf(_T("0. ����� �� ���������.\n"));
		_tprintf(_T("1. ������� ��������� ����������\n"));
		_tprintf(_T("2. ������ ��������� ������ ������ �� ��������� ���������� �����������\n"));
		_tprintf(_T("3. �������� ��������� �������\n"));
		_tprintf(_T("4. LRU\n"));
		_tscanf_s(_T("%d"), &c);
		size_t j = -1;
		switch (c)
		{
		case 0:
			_tprintf(_T("����\n"));
			break;
		case 1:
			getMemoryInfo();
			break;
		case 2:
			_tprintf(_T("������� ������ ����������� ��������\n"));
			_tscanf_s(_T("%d"), &j);
			theLeastSufficientAdd((SIZE_T)j);
			printFreeMem();
			printCommitMem();
			_tprintf(_T("������� ��������� ��������\n"));
			_tscanf_s(_T("%d"), &j);
			theLeastSufficientDelete(j);
			break; 
		case 3:
			genSwapPages();
			break;
		case 4:
			genLRU();
			break;
		}
	}
	return 0;
}


