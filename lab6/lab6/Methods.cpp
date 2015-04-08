#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "Methods.h"
#include "List.h";
#include <locale>

using namespace std;

/*PROBLEMS:
*
* 2. не забыть поменять %d на %x (Мельникова говорила в hex системе. Хотя мне удобнее в 10-й проверять)
*/

struct memPages {
	LPVOID address;
	SIZE_T sizePages;
};

struct limited {
	SIZE_T data;
	SIZE_T length;
};

List *listOfPages = new List();
int pageLimit;
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

		// Я не помню какую именно инфу выводить. Пока пусть будет так
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
		default:
			_tprintf(_T("MEM_FREE"));
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
void theLeastSufficientAdd(SIZE_T added) {
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
void theLeastSufficientDelete(SIZE_T elem)
{
	getSystemInfo();
	fillVectors();

	if (elem >= commitMem.size())
	{
		_tprintf(_T("Index is too big\n\n"));
		return;
	}

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
	/*for (int i = 0; i < freeMem.size(); i++){
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, freeMem[i].address, freeMem[i].sizePages);
	}
	_tprintf(_T("\n"));*/
	printMem(freeMem);
}

void printCommitMem() {
	_tprintf(_T("MEM_COMMIT\n"));
	/*for (int i = 0; i < commitMem.size(); i++){
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, commitMem[i].address, commitMem[i].sizePages);
	}
	_tprintf(_T("\n"));*/
	printMem(commitMem);
}

/*
есть у нас список.список должен быть ограниченным по размеру.например, 
размер списка 4. нам нужно добавить страницу со значением 1000 - добавляем.потом добавляем 500. 
и блабла пока список не ставнет равным 4. после чего, когда нам надо будет еще один элемент добавить, 
мы "выталкиваем" самый старый элемент, который у нас в начале, а в конец добавляем новый
если, например, значение нового элемента совпадает с уже существующим - 
мы просто перекидываем ссылки(то есть теперь этот элемент будет ссылаться на последний, а там, 
где этот элмент был, мы перекинем ссылку предыдущего на следующий после этого элемента).
*/
void insertPage(SIZE_T size, LPVOID address,int count) {
	List::memPages commitPages;
	commitPages.address = address;
	commitPages.sizePages = size;
	List::node* nd = listOfPages->initNode(commitPages, count);
	if (listOfPages->getLength() < pageLimit && !listOfPages->contains(nd)) {
		listOfPages->addNode(nd);
	}
	else if (listOfPages->getLength() < pageLimit && listOfPages->contains(nd)) {
		listOfPages->changePages(nd);
	}
	else {
		if (listOfPages->getLength() == pageLimit) {
			listOfPages->changePages(nd);
			return;
		}
		listOfPages->deleteOddNode(nd);
	}
}
void swapPages(int limitationOfPages) {
	pageLimit = limitationOfPages;
	_tsetlocale(LC_ALL, _T("Russian"));
	_tprintf(_T("Что желаете выполнить:\n"));
	size_t c = -1;
	int count = 0;
	while (c != 0){
		_tprintf(_T("0. Выйти из программы.\n"));
		_tprintf(_T("1. Добавить страницу.\n"));
		_tprintf(_T("2. Вывести информацию о страницах\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{ 
		case 0:
			break;
		case 1:
			_tprintf(_T("Введите адрес страницы:\n"));
			LPVOID address;
			fflush(stdin);
			scanf("%d", &address);
			_tprintf(_T("Введите размер страницы:\n"));
			SIZE_T size;
			fflush(stdin);
			scanf("%d", &size);
			count++;
			insertPage(size, address, count);
			break;
		case 2:
			listOfPages->printList();
			break;
		}
		_tprintf(_T("Пока\n"));
	}
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