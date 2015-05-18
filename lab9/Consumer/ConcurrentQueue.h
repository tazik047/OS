#include "stdafx.h"
#include <Windows.h>
#include <string>

struct queueStruct{
	int current;
	TCHAR messages[10][256];
	BOOL end;
};

TCHAR* getMessage(queueStruct&);

BOOL setMessage(queueStruct&, TCHAR*);

BOOL isEmpty(queueStruct&);