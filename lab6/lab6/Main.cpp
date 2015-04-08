#include "stdafx.h"
#include "Methods.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//getMemoryInfo();
	//theLeastSufficientAdd((SIZE_T)4096);
	//theLeastSufficientDelete(100);
	//swapPages(6);
	SIZE_T mas[] = { 100, 541, 6544, 100, 300, 7000, 1333, 1333, 100 };

	for (int i = 0; i < sizeof(mas)/sizeof(SIZE_T); i++)
	{
		addToPages(mas[i]);
	}

	return 0;
}


