// 2_FindAndGetInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

// a - path
// b - address of STARTUPINFO
// c - address of PROCESS_INFORMATION
#define CreateSuspendedProcess(a, b, c) CreateProcess(0, a, 0, 0, 0, CREATE_SUSPENDED, 0, 0, b, c);

int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR varName = _T("NotepadTime");

	// вообще, по-идее, все должно быть проще. дальше код и цитата с Рихтера

	PTSTR buff = NULL;
	/* Функция возвращает либо количество символов, скопированных в буфер,
	* либо 0, если ей не удалось обнаружить переменную окружения с таким именем.
	* Однако размер значения переменной окружения заранее не известен, поэтому при
	* передаче в параметре cchValue значения 0 функция GetEnvironmentVariable воз-
	* вращает размер (число символов плюс завершающий NULL-символ).*/
	DWORD dwResult = GetEnvironmentVariable(varName, buff, 0);
	if (dwResult != 0)
	{
		// выделяем буфер для значения переменной окружения
		DWORD size = dwResult * sizeof(TCHAR);
		buff = (PTSTR)malloc(size);
		GetEnvironmentVariable(varName, buff, size);
		//_tprintf(TEXT("%s=%s\n"), varName, buff);
		//free(buff);
	}
	else {
		_tprintf(TEXT(",%s,=<unknown value>\n"), varName);
	}
	// endРихтер 

	WIN32_FIND_DATA wfd;
	HANDLE h = FindFirstFile(varName, &wfd);
	while (FindNextFile(h, &wfd))
	{

	}

	return 0;
}

