// Laba10_OS(1)Critical_Section.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <string>
#include <mutex>

#define readerLength 6
#define writerLength 6

HANDLE r, w, mutex1, mutex2;
int countReaders = 0;
int countWriter = 0;
std::vector<std::string> news;
int count = writerLength;
long* temp;

char m1[] = "\tlock %s by %s\n";
char m2[] = "\tunlock %s by %s\n";

DWORD WINAPI writer(LPVOID parametr)//новости записываются в вектор
{
	std::string s((char*)parametr);
	s = "\t" + s;
	for (int i = 0; i < 10; i++)
	{
		WaitForSingleObject(mutex2, INFINITE);
			countWriter++;
			if (countWriter == 1) 
			{
				WaitForSingleObject(r, INFINITE);
			}
		ReleaseMutex(mutex2);

		WaitForSingleObject(w, INFINITE);
			std::string new_i = s + "_new" + std::to_string(i);
			news.push_back(new_i);
			ReleaseSemaphore(w, 1, temp);

		WaitForSingleObject(mutex2, INFINITE);
		countWriter--;
		if (countWriter == 0) {
			ReleaseSemaphore(r, 1, temp);
		}
		ReleaseMutex(mutex2);
	}
	count--;
	return 0;
}

DWORD WINAPI reader(LPVOID parametr)
{
	char* c = (char *)parametr;
	int last_message = 0;
	while (count != 0 || (last_message != news.size()))
	{
		if (news.size() != last_message)
		{
			WaitForSingleObject(r, INFINITE);
				WaitForSingleObject(mutex1, INFINITE);
					countReaders++;
					if (countReaders == 1) {
						WaitForSingleObject(w, INFINITE);
					}
				ReleaseMutex(mutex1);
				ReleaseSemaphore(r, 1, temp);
			///////////////////////////////////////

			printf("%s:\tnew news:\t%s\n", c, (char*)news[last_message++].c_str());

			//////////////////////////////////////
			WaitForSingleObject(mutex1, INFINITE);
				countReaders--;
				if (countReaders == 0) 
				{
					ReleaseSemaphore(w, 1, temp);
				}
			ReleaseMutex(mutex1);
		}
	}
	return 0;
}

LPVOID copy(std::string s){
	int size = s.size();
	char* temp = new CHAR[size + 1];
	memcpy_s(temp, size, (char*)s.c_str(), size);
	temp[size] = '\0';
	return temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	r = CreateSemaphore(0, 1, 1, NULL);
	w = CreateSemaphore(0, 1, 1, NULL);
	mutex1 = CreateMutex(0, FALSE, _T("mutex1"));
	mutex2 = CreateMutex(0, FALSE, _T("mutex2"));

	HANDLE threadHandles[readerLength + writerLength];
	for (int i = 0; i < readerLength; i++)
	{
		std::string readerName = "reader" + std::to_string(i);
		threadHandles[i] = CreateThread(NULL, 0, reader, copy(readerName), 0, NULL);
	}

	for (int i = 0; i < writerLength; i++)
	{
		std::string writerName = "writer" + std::to_string(i);
		threadHandles[i + readerLength] = CreateThread(NULL, 0, writer, copy(writerName), 0, NULL);
	}
	WaitForMultipleObjects(writerLength + readerLength, threadHandles, TRUE, INFINITE);
	for (int i = 0; i < readerLength + writerLength; i++)
		CloseHandle(threadHandles[i]);
	
	ReleaseMutex(mutex1);
	ReleaseMutex(mutex2);
	ReleaseMutex(r);
	ReleaseMutex(w);
	CloseHandle(mutex1);
	CloseHandle(mutex2);
	CloseHandle(r);
	CloseHandle(w);

	system("pause");
	return 0;
}


