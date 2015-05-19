#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"

void copy(TCHAR*, TCHAR*);

TCHAR* getMessage(queueStruct &queue, HANDLE mutex)
{
	if (isEmpty(queue, mutex)) return 0;
	WaitForSingleObject(mutex, INFINITE);
	TCHAR* res = queue.messages[0];
	for (int i = 0; i < queue.current; i++)
		copy(queue.messages[i], queue.messages[i + 1]);
	queue.current--;
	ReleaseMutex(mutex);	
	return res;
}

BOOL setMessage(queueStruct &queue, TCHAR* message, HANDLE mutex)
{
	WaitForSingleObject(mutex, INFINITE);
	if (queue.current == 10) return FALSE;
	copy(queue.messages[queue.current++], message);
	ReleaseMutex(mutex);	
	return TRUE;
}

BOOL isEmpty(queueStruct &queue, HANDLE mutex)
{
	int count;
	WaitForSingleObject(mutex, INFINITE);
	count = queue.current;
	ReleaseMutex(mutex);	
	return count == 0;
}

void copy(TCHAR* str1, TCHAR* str2)
{
	memset(str1, 0, _tcslen(str1)*sizeof(TCHAR));
	for (int i = 0; i < _tcslen(str2); i++)
		str1[i] = str2[i];
}

HANDLE start()
{
	TCHAR name[] = _T("mutex_lab");
	return CreateMutex(0, FALSE, name);
}