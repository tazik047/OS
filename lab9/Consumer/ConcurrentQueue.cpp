#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"

void copy(TCHAR*, TCHAR*);

TCHAR* getMessage(queueStruct &queue)
{
	if (isEmpty(queue)) return 0;
	TCHAR* res = queue.messages[0];
	for (int i = 0; i < queue.current; i++)
		copy(queue.messages[i], queue.messages[i + 1]);
	queue.current--;
	return res;
}

BOOL setMessage(queueStruct &queue, TCHAR* message)
{
	if (queue.current == 10) return FALSE;
	//queue.messages[queue.current]
	//for (int i = 0; i < message.length(); i++)
	copy(queue.messages[queue.current++], message);
	//queue.current++;
	return TRUE;
}

BOOL isEmpty(queueStruct &queue)
{
	return queue.current == 0;
}

void copy(TCHAR* str1, TCHAR* str2)
{
	memset(str1, 0, _tcslen(str1)*sizeof(TCHAR));
	for (int i = 0; i < _tcslen(str2); i++)
		str1[i] = str2[i];
}