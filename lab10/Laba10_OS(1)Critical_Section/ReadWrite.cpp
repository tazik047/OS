// Laba10_OS(1)Critical_Section.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <string>
#include <mutex>

#define readerLength 2
#define writerLength 2

HANDLE r, w, mutex1, mutex2, counter;
int countReaders = 0;
int countWriter = 0;
std::vector<std::string> news;
int count = writerLength;
long* temp;

#define ManyProc

#define MAX_SIZE 10000

char m1[] = "\tlock %s by %s\n";
char m2[] = "\tunlock %s by %s\n";

struct threadStruct{
	int Count;
	char messages[MAX_SIZE][256];
	int length;
};

threadStruct* struct1;


void copy(char* str1, char* str2)
{
	memset(str1, 0, 256);
	for (int i = 0; i < 256; i++){
		str1[i] = str2[i];
		if (str2[i] == '\0') break;
	}
}


DWORD WINAPI writer(LPVOID parametr)
{
	std::string s((char*)parametr);
	for (int i = 0; i < 2; i++)
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
#ifdef ManyProc
			copy(struct1->messages[struct1->length++], (char*)new_i.c_str());
#else
			news.push_back(new_i);
#endif
			
		ReleaseSemaphore(w, 1, temp);

		WaitForSingleObject(mutex2, INFINITE);
		countWriter--;
		if (countWriter == 0) {
			ReleaseSemaphore(r, 1, temp);
		}
		ReleaseMutex(mutex2);
	}
	WaitForSingleObject(counter, INFINITE);
#ifdef ManyProc
	struct1->Count--;
#else
	count--;
#endif
	ReleaseMutex(counter);
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

DWORD WINAPI readerProc(LPVOID parametr)
{
	char* c = (char *)parametr;
	int last_message = 0;
	while (struct1->Count != 0 || (last_message != struct1->length))
	{
		if (last_message != struct1->length)
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

			printf("%s:\tnew news:\t%s\n", c, (char*)struct1->messages[last_message++]);

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
	counter = CreateMutex(0, FALSE, _T("counterLock"));
	r = CreateSemaphore(0, 1, 1, _T("reader"));
	w = CreateSemaphore(0, 1, 1, _T("writer"));
	mutex1 = CreateMutex(0, FALSE, _T("mutex1"));
	mutex2 = CreateMutex(0, FALSE, _T("mutex2"));

#ifdef ManyProc
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(threadStruct), _T("myMapView"));
	int i = GetLastError();
	if (h == 0)
		return GetLastError();
	struct1 = (threadStruct*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif


	HANDLE threadHandles[readerLength + writerLength];
	for (int i = 0; i < readerLength; i++)
	{
		std::string readerName = "reader" + std::to_string(i);
#ifdef ManyProc
		threadHandles[i] = CreateThread(NULL, 0, readerProc, copy(readerName), 0, NULL);
#else
		threadHandles[i] = CreateThread(NULL, 0, reader, copy(readerName), 0, NULL);
#endif
	}

	for (int i = 0; i < writerLength; i++)
	{
		std::string writerName = "writer" + std::to_string(i);
		threadHandles[i + readerLength] = CreateThread(NULL, 0, writer, copy(writerName), 0, NULL);
	}
	WaitForMultipleObjects(writerLength + readerLength, threadHandles, TRUE, INFINITE);
	for (int i = 0; i < readerLength + writerLength; i++)
		CloseHandle(threadHandles[i]);
	CloseHandle(mutex1);
	CloseHandle(mutex2);
	CloseHandle(r);
	CloseHandle(w);

	system("pause");
	return 0;
}


