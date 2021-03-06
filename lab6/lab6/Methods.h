#include "stdafx.h"
#include <Windows.h>

void getSystemInfo();

void getMemoryInfo();

void fillVectors();

void theLeastSufficientAdd(SIZE_T added);

void theLeastSufficientDelete(LPVOID del);

void printCommitMem();

void printFreeMem();

void swapPages(int limitationOfPages);

void printLimitedVector();

void addToPages(SIZE_T elem);

void printVirtualMemoryInfo();