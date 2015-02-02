// Laba8_OS(2)Find.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

void info(TCHAR* folder,TCHAR* fileName,int length)
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	_tprintf(_T("Found file: %s length: %d\n"),fileName,length);
	int stringCount = 0;
	FILE* pFile;
    char mystring [500];
	TCHAR temp[MAX_PATH];
	_tcscpy(temp,folder);
	_tcscat(temp,__TEXT("/"));
	_tcscat(temp,fileName);

	pFile = _tfopen (temp , L"r");
	if (pFile == NULL) perror ("Error opening file");
	else {
		while ( fgets (mystring , 500 , pFile) != NULL )
		{
			stringCount ++;
			int l = strlen(mystring);
			printf("String %d : length = %d\n",stringCount,l);
			
		}
		printf("We have %d string in our file\n ",stringCount);
     fclose (pFile);
   }


}
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD count = 0;
	TCHAR temp[MAX_PATH];
	WIN32_FIND_DATA findFile;
	_tcscpy(temp, _T("D:\Laba8"));
	_tcscat(temp,__TEXT("/*.txt"));
	HANDLE handle = FindFirstFile(temp, &findFile);
	while(handle != INVALID_HANDLE_VALUE)
	{
	      info(_T("D:\Laba8"),findFile.cFileName,findFile.nFileSizeLow);
	      if(FindNextFile(handle, &findFile) == FALSE)
	        break;
	}
	system("pause");
	return 0;
	
}

