#include "stdafx.h"
#include "LRU.h"

int LRU[3]; // {0,0,0}

int cache[4];

void put(int key)
{
	//���������� ���� ������ ������ � ����, ��������� ���� ������ ���, ����� ������������
	//�� ���������� ���� ����. ���.
}

void printCache()
{
	for (size_t i = 0; i < 4; i++)
	{
		_tprintf(_T("%d "), cache[i]);
	}
	_tprintf(_T("\n"));
}