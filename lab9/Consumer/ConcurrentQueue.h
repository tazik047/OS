#include "stdafx.h"
#include <Windows.h>
#include <string>

#define MAX_MESSAGE 256


struct queueStruct{
	int current;
	TCHAR messages[10][MAX_MESSAGE];
	BOOL end;
};

TCHAR* getMessage(queueStruct&, HANDLE);

BOOL setMessage(queueStruct&, TCHAR*, HANDLE);

BOOL isEmpty(queueStruct&, HANDLE);

HANDLE start();

void end(HANDLE map, LPVOID data, HANDLE mutex);