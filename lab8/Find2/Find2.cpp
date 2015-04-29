// Find2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <locale.h>

FILETIME creationTime;
FILETIME lastAccess;
FILETIME fileWrite;
FILETIME ft;

BOOL IsUnicode(PBYTE file)
{
	return(file[0] == 0xfe && file[1] == 0xff) || (file[0] == 0xff && file[1] == 0xfe);
}

template<typename T>
void countStrAndLength(T* str, int size)
{
	int index = 0;
	int count = 0;
	int length = 0;
	while (index < size)
	{
		if (str[index] == '\r' && index + 1 != size && str[index + 1] == '\n') // новая строка
		{
			_tprintf(_T("\tВ строке № %d %d символов\n"), ++count, length);
			length = 0;
			index+=2;
			continue;
		}
		length++;
		index++;
	}
	if (length != 0){
		_tprintf(_T("\tВ строке № %d %d символов\n"), ++count, length);
	}
}

void getInformation(TCHAR* fileName) {

	HANDLE h = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	GetFileTime(h, &creationTime, &lastAccess, &fileWrite);

	LONG compTime = CompareFileTime(&fileWrite, &ft);
	if (compTime != 1) {
		CloseHandle(h);
		return;
	}
	if (h != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = GetFileSize(h, 0);
		PBYTE buf = new BYTE[dwSize];
		DWORD dwCount;
		ReadFile(h, buf, dwSize, &dwCount, 0);
		CloseHandle(h);
		BOOL isUnicode = IsUnicode(buf);
		_tprintf(_T("Файл: %s\nРазмер файла: %d. Файл сохранен в %s\n"),fileName, dwSize, (isUnicode ? _T("UNICODE") : _T("ANSI")));
		if (isUnicode)
		{
			wchar_t* buf_wc = (wchar_t *)buf;
			buf_wc++; // пропускаем BOM
			countStrAndLength(buf_wc, dwSize / 2 - 1);
			delete[] --buf_wc;
		}
		else
		{
			char* buf_c = (char *)buf;
			countStrAndLength(buf_c, dwSize);
			delete[] buf_c;
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

	LPCTSTR varName = _T("Time");
	TCHAR buff[MAX_PATH];
	GetEnvironmentVariable(varName, buff, MAX_PATH);
	__int64 currentTime = toInt64(buff);

	ft.dwLowDateTime = (DWORD)currentTime;
	ft.dwHighDateTime = (DWORD)(currentTime >> 32);
	WIN32_FIND_DATA fileData;
	HANDLE h = FindFirstFile(_T("*.txt"), &fileData);
	while (h != INVALID_HANDLE_VALUE)
	{
		getInformation(fileData.cFileName);
		if (!FindNextFile(h, &fileData))
			break;
	}
	system("pause");
	return 0;
}

