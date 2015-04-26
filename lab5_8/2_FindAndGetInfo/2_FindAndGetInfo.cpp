// 2_FindAndGetInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

// a - path
// b - address of STARTUPINFO
// c - address of PROCESS_INFORMATION
void getInformation() {
	HANDLE h = CreateFile(_T("unicode.txt"), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (h != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = GetFileSize(h, 0);
		PBYTE buf = new BYTE[dwSize + 1];
		DWORD dwCount;
		ReadFile(h, buf, dwSize, &dwCount, 0);
		CloseHandle(h);
		int pnz = 0xFFFFFFFF;

		if (IsTextUnicode(buf, dwCount, &pnz))
		{
			char *resAns = new char[dwCount + 1];
			WideCharToMultiByte(CP_ACP, 0, (wchar_t*)buf, dwCount, resAns, (dwCount + 1) / 2, NULL, NULL);
			resAns[(dwCount + 1) / 2] = 0;
			delete[]buf;
			delete[]resAns;
		}
		else {
			buf[dwCount] = 0;
			delete[]buf;
		}
	}
}
void info(TCHAR* folder, TCHAR* fileName, int length)
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	_tprintf(_T("Found file: %s length: %d\n"), fileName, length);
	int stringCount = 0;
	FILE* pFile;
	char mystring[500];
	TCHAR temp[MAX_PATH];
	_tcscpy(temp, folder);
	_tcscat(temp, __TEXT("/"));
	_tcscat(temp, fileName);

	pFile = _tfopen(temp, L"r");
	if (pFile == NULL) perror("Error opening file");
	else {
		while (fgets(mystring, 500, pFile) != NULL)
		{
			stringCount++;
			int l = strlen(mystring);
			printf("String %d : length = %d\n", stringCount, l);

		}
		printf("We have %d string in our file\n ", stringCount);
		fclose(pFile);
	}


}

int _tmain(int argc, _TCHAR* argv[])
{
	/*DWORD count = 0;
	TCHAR temp[MAX_PATH];
	WIN32_FIND_DATA findFile;
	TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	_tcscpy(temp, CurrentPath);
	_tcscat(temp, __TEXT("/*.txt"));
	HANDLE handle = FindFirstFile(temp, &findFile);
	while (handle != INVALID_HANDLE_VALUE)
	{
		info(CurrentPath, findFile.cFileName, findFile.nFileSizeLow);
		if (FindNextFile(handle, &findFile) == FALSE)
			break;
	}
	system("pause");
	return 0;*/
	LPCTSTR varName = _T("NotepadTime");
	// ������, ��-����, ��� ������ ���� �����. ������ ��� � ������ � �������

	PTSTR buff = NULL;
	/* ������� ���������� ���� ���������� ��������, ������������� � �����,
	* ���� 0, ���� �� �� ������� ���������� ���������� ��������� � ����� ������.
	* ������ ������ �������� ���������� ��������� ������� �� ��������, ������� ���
	* �������� � ��������� cchValue �������� 0 ������� GetEnvironmentVariable ���-
	* ������� ������ (����� �������� ���� ����������� NULL-������).*/
	DWORD dwResult = GetEnvironmentVariable(varName, buff, 0);
	if (dwResult != 0)
	{
		// �������� ����� ��� �������� ���������� ���������
		DWORD size = dwResult * sizeof(TCHAR);
		buff = (PTSTR)malloc(size);
		GetEnvironmentVariable(varName, buff, size);
		//_tprintf(TEXT("%s=%s\n"), varName, buff);
		//free(buff);
	}
	else {
		_tprintf(TEXT(",%s,=<unknown value>\n"), varName);
	}
	// end������ 

	WIN32_FIND_DATA wfd;
	HANDLE h = FindFirstFile(varName, &wfd);
	while (FindNextFile(h, &wfd))
	{

	}
	return 0;
}

