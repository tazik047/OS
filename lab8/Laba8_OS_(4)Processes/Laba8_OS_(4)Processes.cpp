// Laba8_OS_(4)Processes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <psapi.h>

int PrintModules( DWORD processID )
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Get a handle to the process.

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, processID );
    if (NULL == hProcess)
        return 1;

   // Get a list of all the modules in this process.

    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];

            // Get the full path to the module's file.

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {
                // Print the module name and handle value.

                _tprintf( TEXT("\t%s\n"), szModName );
            }
        }
    }
    
    // Release the handle to the process.

    CloseHandle( hProcess );
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	HANDLE WINAPI hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	 if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		printf("CreateToolhelp32Snapshot (of processes)" );
		return( FALSE );
	}
	  pe32.dwSize = sizeof( PROCESSENTRY32 );
	  BOOL b;
	  b = Process32First( hProcessSnap, &pe32 );
		while(b){
			PrintModules(pe32.th32ProcessID);
			printf ("cntThreads = %d, ", pe32.cntThreads);//потоки
			_tprintf (_T("szExeFile = %s\n"), pe32.szExeFile);//имя процесса
			
		//Для всех остальных процессов
		b = Process32Next( hProcessSnap, &pe32);
		if (!b) 	break;

	}
		system("pause");
	return 0;
}

