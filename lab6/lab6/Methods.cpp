#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "Methods.h"
#include <locale>

using namespace std;


struct memPages {
	LPVOID address;
	SIZE_T sizePages;
};

vector<memPages> freeMem;
vector<memPages> commitMem;

MEMORY_BASIC_INFORMATION mb;
SIZE_T dwLength = sizeof(mb);
HANDLE h = GetCurrentProcess();

SIZE_T bestSuitedSize, bestSuitedIndex;
LPVOID start, stop;

void getSystemInfo() {
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);

	start = sinf.lpMinimumApplicationAddress;
	stop = sinf.lpMaximumApplicationAddress;
}

void printVirtualMemoryInfo() {
	MEMORYSTATUSEX ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatusEx(&ms);
	printf("MemoryLodad = %d\n", ms.dwMemoryLoad);
	printf("TotalPhys = %08x\n", ms.ullTotalPhys);
	printf("AvailPhys = %08x\n", ms.ullAvailPhys);
	printf("TotalPageFile = %08x\n", ms.ullTotalPageFile);
	printf("AvailPageFile = %08x\n", ms.ullAvailPageFile);
	printf("TotalVirtual = %08x\n", ms.ullTotalVirtual);
	printf("AvailVirtual = %08x\n", ms.ullAvailVirtual);
}
//firstTask
void getMemoryInfo()
{
	printVirtualMemoryInfo();
	getSystemInfo();
	while ((unsigned)start < (unsigned)stop)
	{
		// Технически, можно юзать VirtualQuery
		VirtualQueryEx(h, start, &mb, dwLength);

		_tprintf(_T("AllocationBase: %x\n"), mb.AllocationBase);
		_tprintf(_T("BaseAddress: %x\n"), mb.BaseAddress);
		_tprintf(_T("RegionSize: %x\n"), mb.RegionSize);

		_tprintf(_T("State: "));
		switch (mb.State) {
		case MEM_COMMIT:
			_tprintf(_T("MEM_COMMIT"));
			break;
		case MEM_FREE:
			_tprintf(_T("MEM_FREE"));
			break;
		case MEM_RESERVE:
			_tprintf(_T("MEM_RESERVE"));
			break;
		}
		_tprintf(_T("\n"));

		// Тип не всегда отображается, не знаю почему
		if (mb.Type)
			_tprintf(_T("Type: "));
		switch (mb.Type) {
		case MEM_IMAGE:
			_tprintf(_T("MEM_IMAGE"));
			break;
		case MEM_MAPPED:
			_tprintf(_T("MEM_MAPPED"));
			break;
		case MEM_PRIVATE:
			_tprintf(_T("MEM_PRIVATE"));
			break;
		}

		_tprintf(_T("\n\n"));

		start = (BYTE*)start + mb.RegionSize;
	}
}

void fillVectors()
{
	while ((unsigned)start < (unsigned)stop)
	{
		VirtualQueryEx(h, start, &mb, dwLength);

		switch (mb.State)
		{
		case MEM_FREE:
			memPages freePages;
			freePages.address = start;
			freePages.sizePages = mb.RegionSize;
			freeMem.push_back(freePages);
			break;
		case MEM_COMMIT:
			memPages commitPages;
			commitPages.address = start;
			commitPages.sizePages = mb.RegionSize;
			commitMem.push_back(commitPages);
			break;
		}
		start = (BYTE*)start + mb.RegionSize;
	}
}

////////////////Выделение памяти для added
void theLeastSufficientAdd(SIZE_T added) {
	printCommitMem();
	printFreeMem();
	bestSuitedSize = -1;
	bestSuitedIndex = -1;

	// Ищем куда добавить
	for (int i = 0; i < freeMem.size(); i++)
	{
		if ((freeMem[i].sizePages >= added) && (bestSuitedIndex == -1 || freeMem[i].sizePages < bestSuitedSize))
		{
			bestSuitedSize = freeMem[i].sizePages;
			bestSuitedIndex = i;
		}
	}

	if (bestSuitedIndex == -1)
	{
		_tprintf(_T("Нет подходящей страницы.\n"));
		return;
	}

	_tprintf(_T("\nbestSuitedSize = %d bestSuitedIndex = %d\n\n"), bestSuitedSize, bestSuitedIndex);

	//Добавляем новый элемент в коммитМем
	memPages temp;
	temp.address = freeMem[bestSuitedIndex].address;
	temp.sizePages = added;
	commitMem.push_back(temp);

	// меняем элемент в фриМем
	freeMem[bestSuitedIndex].address = (BYTE*)freeMem[bestSuitedIndex].address + added;
	freeMem[bestSuitedIndex].sizePages -= added;
	if (freeMem[bestSuitedIndex].sizePages == 0)
		freeMem.erase(freeMem.begin() + bestSuitedIndex);
}

void theLeastSufficientDelete(LPVOID del)
{
	for (int i = 0; i < commitMem.size(); i++)
	{
		if (commitMem[i].address == del)
		{
			memPages freePage;
			freePage.address = commitMem[i].address;
			freePage.sizePages = commitMem[i].sizePages;
			freeMem.push_back(freePage);

			commitMem.erase(commitMem.begin() + i);
			break;
		}
	}

	printCommitMem();
	printFreeMem();
}

//Передаем сюда ссылку на вектор, потому что если передавать просто вектор, то все значения будут
//копироваться, а это долого и по памяти напряжно.
void printMem(vector<memPages> &mas)
{
	for (int i = 0; i < mas.size(); i++){
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, mas[i].address, mas[i].sizePages);
	}
	_tprintf(_T("\n"));

}

void printFreeMem() {
	_tprintf(_T("MEM_FREE\n"));
	printMem(freeMem);
}

void printCommitMem() {
	_tprintf(_T("MEM_COMMIT\n"));
	printMem(commitMem);
}


//third
vector<SIZE_T> limitedVector;
void printLimitedVector()
{
	for (int i = 0; i < limitedVector.size(); i++){
		_tprintf(_T("%d. %d\n"), i, limitedVector[i]);
	}
	_tprintf(_T("\n"));
}

void addToPages(SIZE_T elem)
{
	_tprintf(_T("Elem: %d\n"), elem);
	SIZE_T maxLen = 5;

	for (int i = 0; i < limitedVector.size(); i++)
	{
		if (limitedVector[i] == elem)
		{
			limitedVector.push_back(limitedVector[i]);
			limitedVector.erase(limitedVector.begin() + i);
			printLimitedVector();
			return;
		}
	}

	if (limitedVector.size() == maxLen)
	{
		limitedVector.erase(limitedVector.begin());
		limitedVector.push_back(elem);
	}
	else
	{
		limitedVector.push_back(elem);
	}
	printLimitedVector();
}