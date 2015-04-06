#include "stdafx.h"
#include "CRC.h"

int MAX_PRIME_NUMBER = 81157;

BOOL isPrime(int num)
{
	BOOL isprime = TRUE;
	for (int i = 2; i <= std::sqrt(num); i++)
	{
		if ((num % i) == 0)
			return FALSE;
	}
	return isprime;
}


BOOL validate(HANDLE h){
	SetFilePointer(h, -4, 0, FILE_END);
	DWORD sum, a, b, read;
	ReadFile(h, &sum, 4, &read, 0);
	SetFilePointer(h, -4, 0, FILE_END);
	SetEndOfFile(h);
	SetFilePointer(h, 0, 0, FILE_BEGIN);
	ReadFile(h, &a, 4, &read, 0);
	while (TRUE){
		ReadFile(h, &b, 4, &read, 0);
		if (read != 4)
			break;
		a = (a + b) % MAX_PRIME_NUMBER;
	}
	if (a == sum){
		return TRUE;
	}
	WriteFile(h, &sum, 4, &read, 0);
	return FALSE;
}

void createCRC(HANDLE h){
	DWORD a, b, read;
	SetFilePointer(h, 0, 0, FILE_BEGIN);
	ReadFile(h, &a, 4, &read, 0);
	while (TRUE){
		ReadFile(h, &b, 4, &read, 0);
		if (read != 4)
			break;
		a = (a + b) % MAX_PRIME_NUMBER;
	}
	SetFilePointer(h, 0, 0, FILE_END);
	WriteFile(h, &a, 4, &read, 0);
}