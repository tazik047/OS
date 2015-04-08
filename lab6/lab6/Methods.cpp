#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "Methods.h"
#include "List.h";
#include <locale>

using namespace std;

/*PROBLEMS:
*
* 2. �� ������ �������� %d �� %x (���������� �������� � hex �������. ���� ��� ������� � 10-� ���������)
*/

struct memPages {
	LPVOID address;
	SIZE_T sizePages;
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
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);
	printf("dwMemoryLodad = %d\n", ms.dwMemoryLoad);
	printf("dwTotalPhys = %08x\n", ms.dwTotalPhys);
	printf("dwAvailPhys = %08x\n", ms.dwAvailPhys);
	printf("dwTotalPageFile = %08x\n", ms.dwTotalPageFile);
	printf("dwAvailPageFile = %08x\n", ms.dwAvailPageFile);
	printf("dwTotalVirtual = %08x\n", ms.dwTotalVirtual);
	printf("dwAvailVirtual = %08x\n", ms.dwAvailVirtual);
}
//firstTask
void getMemoryInfo()
{
	printVirtualMemoryInfo();
	getSystemInfo();
	while ((unsigned)start < (unsigned)stop)
	{
		// ����������, ����� ����� VirtualQuery
		VirtualQueryEx(h, start, &mb, dwLength);

		// � �� ����� ����� ������ ���� ��������. ���� ����� ����� ���
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

		// ��� �� ������ ������������, �� ���� ������
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

		start = (SIZE_T*)start + mb.RegionSize;
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
		start = (SIZE_T*)start + mb.RegionSize;
	}

	// ����� ������� �� �������
	printCommitMem();
	printFreeMem();
}

////////////////��������� ������ ��� added
void theLeastSufficientAdd(SIZE_T added) {
	getSystemInfo();
	fillVectors();

	bestSuitedSize = freeMem[0].sizePages;
	bestSuitedIndex = 0;

	// ���� ���� ��������
	for (int i = 1; i < freeMem.size(); i++)
	{
		if ((freeMem[i].sizePages >= added) && (freeMem[i].sizePages < bestSuitedSize))
		{
			bestSuitedSize = freeMem[i].sizePages;
			bestSuitedIndex = i;
		}
	}

	_tprintf(_T("\nbestSuitedSize = %d bestSuitedIndex = %d\n\n"), bestSuitedSize, bestSuitedIndex);

	//��������� ����� ������� � ���������
	memPages temp;
	temp.address = freeMem[bestSuitedIndex].address;
	temp.sizePages = added;
	commitMem.push_back(temp);

	// ������ ������� � ������
	freeMem[bestSuitedIndex].address = (SIZE_T*)freeMem[bestSuitedIndex].address + added;
	freeMem[bestSuitedIndex].sizePages -= added;
	if (freeMem[bestSuitedIndex].sizePages == 0)
		freeMem.erase(freeMem.begin() + bestSuitedIndex);

	// ����� �� ������� freeMem ����� ��������� ������
	printFreeMem();
}

// ������� ����� �� ������, �� ��� ������ �������, �.�. �� �������� ���������
void theLeastSufficientDelete(SIZE_T elem)
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
/*
���� � ��� ������.������ ������ ���� ������������ �� �������.��������, 
������ ������ 4. ��� ����� �������� �������� �� ��������� 1000 - ���������.����� ��������� 500. 
� ������ ���� ������ �� ������� ������ 4. ����� ����, ����� ��� ���� ����� ��� ���� ������� ��������, 
�� "�����������" ����� ������ �������, ������� � ��� � ������, � � ����� ��������� �����
����, ��������, �������� ������ �������� ��������� � ��� ������������ - 
�� ������ ������������ ������(�� ���� ������ ���� ������� ����� ��������� �� ���������, � ���, 
��� ���� ������ ���, �� ��������� ������ ����������� �� ��������� ����� ����� ��������).
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
	_tprintf(_T("��� ������� ���������:\n"));
	size_t c = -1;
	int count = 0;
	while (c != 0){
		_tprintf(_T("0. ����� �� ���������.\n"));
		_tprintf(_T("1. �������� ��������.\n"));
		_tprintf(_T("2. ������� ���������� � ���������\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
		case 0:
			break;
		case 1:
			_tprintf(_T("������� ����� ��������:\n"));
			LPVOID address;
			fflush(stdin);
			scanf("%d", &address);
			_tprintf(_T("������� ������ ��������:\n"));
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
		_tprintf(_T("����\n"));
	}
}