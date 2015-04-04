#include "stdafx.h"
#include <vector>
#include <Windows.h>

using namespace std;

struct memPage {
	LPVOID address;
	BYTE size;
};

void getMemoryInfo()
{
	//�� ������ ����
	vector<memPage> freeMem; // case mbi.State & MEM_FREE
	vector<memPage> commitMem; // case mbi.State & MEM_COMMIT

	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);

	LPVOID start = sinf.lpMinimumApplicationAddress;
	LPVOID stop = sinf.lpMaximumApplicationAddress;

	MEMORY_BASIC_INFORMATION mb;
	SIZE_T dwLength = sizeof(mb);
	HANDLE h = GetCurrentProcess();

	while ((unsigned)start < (unsigned)stop)
	{
		// ����������, ����� ����� VirtualQueryEx
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

		// ��� �� ������ ������������. ������, ����� ���� ����, ��� �����-�� ��� ����
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
