#include "stdafx.h"
#include "Methods.h"
#include <Windows.h>
#include "LRU.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//getMemoryInfo();
	//theLeastSufficientAdd((SIZE_T)4096);
	/*theLeastSufficientDelete(5);
	//swapPages(6);
	SIZE_T mas[] = { 100, 541, 6544, 100, 300, 7000, 1333, 1333, 100 };

	for (int i = 0; i < sizeof(mas)/sizeof(SIZE_T); i++)
	{
		addToPages(mas[i]);
	}*/

	int myM[] = { 0, 1, 0, 2, 3, 0, 3, 1 };

	for (int i = 0; i < sizeof(myM) / sizeof(int); i++)
	{
		put(i);
		printCache();
	}

	return 0;
}


