#include "stdafx.h"
#include <Windows.h>
#include <string>

struct queueStruct{
	int current;
	std::string messages[10];
};

std::string getMessage(queueStruct &queue);

BOOL setMessage(queueStruct &queue, std::string message);