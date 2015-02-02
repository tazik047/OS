// Laba10_OS(1)Critical_Section.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <string>
#define readerLength 4
#define writerLength 3


CRITICAL_SECTION criticalSection;
std::vector<std::string> news;
int count = writerLength;

DWORD WINAPI writer(LPVOID parametr)//новости записываются в вектор
{
	for(int i = 0;i < 10;i++)
	{
		EnterCriticalSection (&criticalSection);
		std::string new_i = "new" + std::to_string (i);
		news.insert(news.end(), new_i);
		//char* c = (char *)parametr;
		LeaveCriticalSection (&criticalSection);
		//printf("%s",c);
		
	}
	count--;
	return 0;
}

DWORD WINAPI reader(LPVOID parametr)
{
	char* c = (char *)parametr;
	c[0] = 'r';//what is it??????

	while (count != 0)
	{
		if(news.size() <= 0)
			Sleep(300);
		else
		{
			EnterCriticalSection (&criticalSection);
			printf("%s",c);
			printf(":Wow,have you heard that %s\n",(char*)news[news.size()-1].c_str());
			LeaveCriticalSection (&criticalSection);
		}
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitializeCriticalSection (&criticalSection);
	HANDLE threadHandles[readerLength + writerLength];
	//HANDLE readerThreadHandles[readerLength];
	//HANDLE writerThreadHandles[writerLength];
	for(int i = 0;i < readerLength;i++)
	{
		std::string readerName = "reader" + std::to_string (i);
		threadHandles[i] = CreateThread(NULL, 0, reader, (char*)readerName.c_str(), 0, NULL);
		
	}

	for(int i = 0;i < writerLength;i++)
	{
		std::string writerName = "writer" + std::to_string (i);
		threadHandles[i + readerLength] = CreateThread(NULL, 0, writer, (char*)writerName.c_str(), 0, NULL);
	}
	WaitForMultipleObjects (writerLength, threadHandles, false, INFINITE);
	DeleteCriticalSection (&criticalSection);
	system("pause");
	return 0;
}


