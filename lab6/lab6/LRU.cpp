#include "stdafx.h"
#include "LRU.h"

BOOL contains(int);

BYTE LRU[] = { 0, 0, 0 };

int cache[] = { -1, -1, -1, -1 };

void startLRU()
{
	for (int i = 0; i < 4; i++)
		cache[i] = -1;
	for (int i = 0; i < 3; i++)
		LRU[i] = 0;
}

void put(int key)
{
	if (contains(key)) return;
	if (LRU[0] == 0) // если 0, то нужно писать в первую половину кэша.
	{
		LRU[0] = 1;
		if (LRU[1] == 0)
		{
			cache[0] = key;
			LRU[1] = 1;
		}
		else
		{
			cache[1] = key;
			LRU[1] = 0;
		}
	}
	else // во вторую
	{
		LRU[0] = 0;
		if (LRU[2] == 0)
		{
			cache[2] = key;
			LRU[2] = 1;
		}
		else
		{
			cache[3] = key;
			LRU[2] = 0;
		}
	}
}

BOOL contains(int key)
{
	for (int i = 0; i < 4; i++)
	{
		if (cache[i] == key)
			return TRUE;
	}
	return FALSE;
}

void printCache()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (cache[i] == -1)
		{
			_tprintf(_T("_ "));
			continue;
		}
		_tprintf(_T("%d "), cache[i]);
	}
	_tprintf(_T("\n"));
}