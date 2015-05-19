#include "stdafx.h"
#include <Windows.h>
#include <string>

struct queueStruct{
	int current;
	TCHAR messages[10][256];
	BOOL end;
};

TCHAR* getMessage(queueStruct&, HANDLE);

BOOL setMessage(queueStruct&, TCHAR*, HANDLE);

BOOL isEmpty(queueStruct&, HANDLE);

HANDLE start();