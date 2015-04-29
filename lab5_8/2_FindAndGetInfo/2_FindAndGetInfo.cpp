// 2_FindAndGetInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <locale>

FILETIME creationTime;
FILETIME lastAccess;
FILETIME fileWrite;
SYSTEMTIME st;
SYSTEMTIME checkST;
FILETIME ft;
FILETIME tmp;

void getInformation(TCHAR* fileName, __int64 currentTime) {

	HANDLE h = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	GetFileTime(h, &creationTime, &lastAccess, &fileWrite);
	__int64 fileCreationTimeI64 = ((__int64(fileWrite.dwHighDateTime) << 32) | __int64(fileWrite.dwLowDateTime));//выскакивает разница в 3 часа

	tmp.dwLowDateTime = (DWORD)fileCreationTimeI64;
	tmp.dwHighDateTime = (DWORD)(fileCreationTimeI64 >> 32);
	FileTimeToSystemTime(&tmp, &st);//разница в 3 часа с настоящим временем (отстает)
	st.wHour = st.wHour + 3;
	SystemTimeToFileTime(&st, &tmp);//костыль но пусть хоть так пока

	ft.dwLowDateTime = (DWORD)currentTime;
	ft.dwHighDateTime = (DWORD)(currentTime >> 32);
	FileTimeToSystemTime(&ft, &checkST);//переводит норм


	LONG compTime = CompareFileTime(&tmp, &ft);
	if (compTime != 1) {
		return;
	}
	if (h != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = GetFileSize(h, 0);
		PBYTE buf = new BYTE[dwSize + 1];
		DWORD dwCount;
		ReadFile(h, buf, dwSize, &dwCount, 0);
		CloseHandle(h);
		int size = 0;
		int countOfLines = 0;
		int lengthOfLine = 0;
		int pnz = 0xFFFFFFFF;
		if (IsTextUnicode(buf, dwCount, &pnz))
		{
			_tprintf(_T("Файл с именем %s записан в Юникоде\n"), fileName);
			char *result = (char*)buf;
			char *resAns = new char[dwCount + 1];
			WideCharToMultiByte(CP_ACP, 0,(wchar_t*)buf, dwCount, resAns, (dwCount + 1) / 2, NULL, NULL);
			resAns[(dwCount + 1) / 2] = '\0';
			size = dwSize;
			printf("The file size is %d\n", size);
			for (int i = 1; i <= (dwCount + 1) / 2; i++){
				if (resAns[i] == '\n') {
					continue;
				}
				if (resAns[i] == '\r' || i == (dwCount + 1) / 2) {
					countOfLines++;
					printf("The length of %d line is %d\n", countOfLines, lengthOfLine);
					lengthOfLine = 0;
					continue;
				}
				lengthOfLine++;
			}
			
		}
		else {
			_tprintf(_T("Файл с именем %s записан в ANSI\n"), fileName);
			char *result = (char*)buf;
			result[dwCount] = '\0';
			size = dwSize;
			printf("The file size is %d\n", size);
			for (int i = 0; i <= dwCount; i++){
				if (result[i] == '\n') {
					continue;
				}
				if (result[i] == '\r' || i == dwCount) {
					countOfLines++;
					printf("The length of %d line is %d\n", countOfLines, lengthOfLine);
					lengthOfLine = 0;
					continue;
				}
				lengthOfLine++;
			}
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	//SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	//GetLocalTime(&st);
	//SystemTimeToFileTime(&st, &ft);
	//__int64 minTime = (__int64(ft.dwHighDateTime) << 32) | __int64(ft.dwLowDateTime);
	//char value[MAX_PATH];
	////*((__int64*)value) = minTime;
	//_i64toa(minTime, value, 10);
	//wchar_t* wString = new wchar_t[4096];
	//MultiByteToWideChar(CP_ACP, 0, value, -1, wString, 4096);
	//SetEnvironmentVariable(_T("NotepadTime"), wString);
	
	_tsetlocale(LC_ALL, _T("Russian"));
	LPCTSTR varName = _T("NotepadTime");
	TCHAR* buff = NULL;
	DWORD dwResult = GetEnvironmentVariable(varName, buff, 0);
	if (dwResult != 0)
	{
		DWORD size = dwResult * sizeof(TCHAR);
		buff = (PTSTR)malloc(size);
		GetEnvironmentVariable(varName, buff, size);
		//_tprintf(TEXT("%s=%s\n"), varName, buff);
		//free(buff);
	}
	else {
		_tprintf(TEXT("%s,=<unknown value>\n"), varName);
		exit(1);
	}
	__int64 currentTime = _wcstoui64(buff,NULL,10);
	tmp.dwLowDateTime = (DWORD)currentTime;
	tmp.dwHighDateTime = (DWORD)(currentTime >> 32);
	// endРихтер 
	WIN32_FIND_DATA findFile;
	TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	_tcscat(CurrentPath, __TEXT("\\*.txt"));
	HANDLE handle = FindFirstFile(CurrentPath, &findFile);
	while (handle != INVALID_HANDLE_VALUE)
	{
		getInformation(findFile.cFileName, currentTime);
		if (FindNextFile(handle, &findFile) == FALSE)
			break;
	}
	system("pause");
	return 0;
}

