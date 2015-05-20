#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"

void copy(TCHAR*, TCHAR*);

TCHAR* getMessage(queueStruct &queue, HANDLE mutex)
{
	if (isEmpty(queue, mutex)) return 0;
	WaitForSingleObject(mutex, INFINITE);
	TCHAR* res = new TCHAR[MAX_MESSAGE];
	/*_tprintf(_T("sleeeeep\n"));
	Sleep(1000);
	_tprintf(_T("wake up\n"));
	Sleep(1000);*/
	copy(res, queue.messages[0]);
	for (int i = 0; i < queue.current; i++)
		copy(queue.messages[i], queue.messages[i + 1]);
	queue.current--;
	ReleaseMutex(mutex);	
	return res;
}

BOOL setMessage(queueStruct &queue, TCHAR* message, HANDLE mutex)
{
	WaitForSingleObject(mutex, INFINITE);
	if (queue.current == 10) {
		ReleaseMutex(mutex);
		return FALSE;
	}
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
	memset(str1, 0, MAX_MESSAGE);
	for (int i = 0; i < MAX_MESSAGE; i++){
		str1[i] = str2[i];
		if (str2[i] == '\0') break;
	}
}

HANDLE start()
{
	TCHAR name[] = _T("mutex_lab");
	HANDLE h = OpenMutex(0, FALSE, name);
	if (h==0)
		return CreateMutex(0, FALSE, name);
	return h;
}

void end(HANDLE map, LPVOID data, HANDLE mutex){
	if (data)
		UnmapViewOfFile(data);
	if (mutex)
		CloseHandle(mutex);
	if (map)
		CloseHandle(map);
}