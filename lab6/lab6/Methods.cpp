#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "Methods.h"

using namespace std;

/*PROBLEMS:
*
* 2. не забыть поменять %d на %x (Мельникова говорила в hex системе. Хотя мне удобнее в 10-й проверять)
*/

struct memPages {
	LPVOID address;
	BYTE sizePages;
};

vector<memPages> freeMem;
vector<memPages> commitMem;

MEMORY_BASIC_INFORMATION mb;
BYTE dwLength = sizeof(mb);
HANDLE h = GetCurrentProcess();

BYTE bestSuitedSize, bestSuitedIndex;
LPVOID start, stop;

void getSystemInfo() {
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);

	start = sinf.lpMinimumApplicationAddress;
	stop = sinf.lpMaximumApplicationAddress;
}

void printVirtualMemoryInfo() {
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);
	printf("dwMemoryLodad = %d/n", ms.dwMemoryLoad);
	printf("dwTotalPhys = %08x/n", ms.dwTotalPhys);
	printf("dwAvailPhys = %08x/n", ms.dwAvailPhys);
	printf("dwTotalPageFile = %08x/n", ms.dwTotalPageFile);
	printf("dwAvailPageFile = %08x/n", ms.dwAvailPageFile);
	printf("dwTotalVirtual = %08x/n", ms.dwTotalVirtual);
	printf("dwAvailVirtual = %08x/n", ms.dwAvailVirtual);
}

void getMemoryInfo()
{
	getSystemInfo();
	while ((unsigned)start < (unsigned)stop)
	{
		// Технически, можно юзать VirtualQuery
		VirtualQueryEx(h, start, &mb, dwLength);

		// Я не помню какую именно инфу выводить. Пока пусть будет так
		_tprintf(_T("AllocationBase: %x\n"), mb.AllocationBase);
		_tprintf(_T("BaseAddress: %x\n"), mb.BaseAddress);
		_tprintf(_T("RegionSize: %x\n"), mb.RegionSize);

		_tprintf(_T("RegionSize: "));
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

	// Вывод списков на консоль
	printCommitMem();
	printFreeMem();
}

////////////////Выделение памяти для added
void theLeastSufficientAdd(BYTE added) {
	getSystemInfo();
	fillVectors();

	bestSuitedSize = freeMem[0].sizePages;
	bestSuitedIndex = 0;

	// Ищем куда добавить
	for (int i = 1; i < freeMem.size(); i++)
	{
		if ((freeMem[i].sizePages >= added) && (freeMem[i].sizePages < bestSuitedSize))
		{
			bestSuitedSize = freeMem[i].sizePages;
			bestSuitedIndex = i;
		}
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

	// Вывод на консоль freeMem после выделения памяти
	printFreeMem();
}

// Удалять нужно по адресу, но его трудно угадать, т.к. он меняется постоянно
void theLeastSufficientDelete(BYTE elem)
{
	getSystemInfo();
	fillVectors();

	_tprintf(_T("DELETING. Address: %d Size: %d\n\n"), commitMem[elem].address, commitMem[elem].sizePages);
	LPVOID del = commitMem[elem].address;


	for (int i = 1; i < commitMem.size(); i++)
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

void printFreeMem() {
	_tprintf(_T("MEM_FREE\n"));
	for (int i = 0; i < freeMem.size(); i++){
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, freeMem[i].address, freeMem[i].sizePages);
	}
	_tprintf(_T("\n"));
}

void printCommitMem() {
	_tprintf(_T("MEM_COMMIT\n"));
	for (int i = 0; i < commitMem.size(); i++){
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, commitMem[i].address, commitMem[i].sizePages);
	}
	_tprintf(_T("\n"));
}