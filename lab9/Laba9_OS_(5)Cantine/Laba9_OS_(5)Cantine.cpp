// Laba9_OS_(5)Cantine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <string>


//volatile 
int files = 1000;
//volatile 
bool the_end = false;
std::vector<std::string> strings;

DWORD WINAPI producer(LPVOID parametr)
{
	
	for(int i = 0;i < files;i++)
	{
		std::string fileName = "f" + std::to_string (i);
		fileName += ".txt";
		std::string text = "text" + std::to_string (i);
		FILE* f = fopen(((char*)fileName.c_str()),"w");
		if(f == NULL)
		{
			printf("error producer");
			continue;
		}
		fputs((char*)text.c_str(),f);
		fclose(f);
		strings.insert(strings.end(), fileName);
	}
	the_end = true;
	return 0;
}
bool one()
{
	if(strings.size() > 0)
	{
		std::string file = strings[0];
		char mystring [500];
		FILE* f = fopen(((char*)file.c_str()),"r");
		if(f == NULL)
		{
			return false;
		}
		char* c = fgets(mystring,500,f);
		printf("%s = %s\n",(char*)file.c_str(),c);
		fclose(f);
		strings.erase(strings.begin() + 0);
		return true;
	}
	else
		return false;
}
DWORD WINAPI customer(LPVOID parametr)
{
	while(!the_end)
	{
		if(!one())
			Sleep(500);

	}
	while(one())
	{

	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char *threads[] = {"1", "2"};
	HANDLE threadHandles[2];
	threadHandles[0] = CreateThread(NULL, 0, producer, threads[0], 0, NULL);
	threadHandles[1] = CreateThread(NULL, 0, customer, threads[1], 0, NULL);
	WaitForMultipleObjects (2, threadHandles, true, INFINITE);
	system("pause");
	return 0;
}

