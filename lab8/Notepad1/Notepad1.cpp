// Notepad1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "..\Main3\RunProc.h"


int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si = { 0 }; //���������� ��� ���� | memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	TCHAR ProcName[] = _T("Notepad.exe");
	BOOL b = CreateUnsuspendedProcess(ProcName, &si, &pi, 0);
	if (!b)
	{
		printf("Error\n");
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}

