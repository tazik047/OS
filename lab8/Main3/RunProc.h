#include<Windows.h>

#define CreateUnsuspendedProcess(path, lpsi, lppi, directory) CreateProcess(0, path, 0, 0, 0, 0, 0, directory, lpsi, lppi);
#define CreateSuspendedProcess(path, lpsi, lppi, directory) CreateProcess(0, path, 0, 0, 0, CREATE_SUSPENDED, 0, directory, lpsi, lppi);