// Find2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <locale.h>

FILETIME creationTime;
FILETIME lastAccess;
FILETIME fileWrite;
//SYSTEMTIME st;
//SYSTEMTIME checkST;
FILETIME ft;
FILETIME tmp;


void getInformation(TCHAR* fileName, __int64 currentTime) {

	HANDLE h = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	GetFileTime(h, &creationTime, &lastAccess, &fileWrite);
	/*__int64 fileCreationTimeI64 = fileWrite.dwHighDateTime;
	fileCreationTimeI64 = (fileCreationTimeI64 << 32) | fileWrite.dwLowDateTime;*/
	/*
	tmp.dwLowDateTime = (DWORD)fileCreationTimeI64;
	tmp.dwHighDateTime = (DWORD)(fileCreationTimeI64 >> 32);
	FileTimeToSystemTime(&tmp, &st);//разница в 3 часа с настоящим временем (отстает)
	st.wHour = st.wHour + 3;
	SystemTimeToFileTime(&st, &tmp);//костыль но пусть хоть так пока*/

	ft.dwLowDateTime = (DWORD)currentTime;
	ft.dwHighDateTime = (DWORD)(currentTime >> 32);
	//FileTimeToSystemTime(&ft, &checkST);//переводит норм


	LONG compTime = CompareFileTime(&fileWrite, &ft);
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
			char *result = (char*)buf;
			char *resAns = new char[dwCount + 1];
			WideCharToMultiByte(CP_ACP, 0, (wchar_t*)buf, dwCount, resAns, (dwCount + 1) / 2, NULL, NULL);
			resAns[(dwCount + 1) / 2] = '\0';
			size = dwSize;
			printf("The file size is %d\n", size);
			for (int i = 1; i < (dwCount + 1) / 2; i++){
				if (resAns[i] == '\n') {
					continue;
				}
				if (resAns[i] == '\r' || i == (dwCount + 1) / 2 - 1) {
					countOfLines++;
					printf("The length of %d line is %d\n", countOfLines, lengthOfLine);
					lengthOfLine = 0;
					continue;
				}
				lengthOfLine++;
			}

		}
		else {
			char *result = (char*)buf;
			result[dwCount] = '\0';
			size = dwSize;
			printf("The file size is %d\n", size);
			for (int i = 0; i < dwCount; i++){
				if (result[i] == '\n') {
					continue;
				}
				if (result[i] == '\r' || i == dwCount - 1) {
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


__int64 toInt64(TCHAR* str){
	__int64 res = 0;
	int i = 0;
	while (str[i] != '\0'){
		res = res * 10 + (str[i++] - '0');
	}
	return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));

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

	

	LPCTSTR varName = _T("Time");
	TCHAR buff[MAX_PATH];
	GetEnvironmentVariable(varName, buff, MAX_PATH);
	__int64 currentTime = toInt64(buff);

	tmp.dwLowDateTime = (DWORD)currentTime;
	tmp.dwHighDateTime = (DWORD)(currentTime >> 32);
	WIN32_FIND_DATA findFile;
	/*TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	_tcscat_s(CurrentPath, _T("\\*.txt"));*/
	HANDLE handle = FindFirstFile(_T("*.txt"), &findFile);
	while (handle != INVALID_HANDLE_VALUE)
	{
		getInformation(findFile.cFileName, currentTime);
		if (!FindNextFile(handle, &findFile))
			break;
	}
	system("pause");
	return 0;
}

