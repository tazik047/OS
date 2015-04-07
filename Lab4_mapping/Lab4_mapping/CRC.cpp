#include "stdafx.h"
#include "CRC.h"
#include "MyMain.h"

#define MAX_PRIME_NUMBER 81157

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
	int end = GetFileSize(h, 0) - 4;
	DWORD sum, a, b, index = 4;
	sum = byteToT<int>(end);
	a = byteToT<int>(0);
	while (index <= end-4){
		b = byteToT<int>(index);
		a = (a + b) % MAX_PRIME_NUMBER;
		index += 4;
	}
	if (a == sum){
		reduceFile(4);
		return TRUE;
	}
	writeTToByte(end, sum);
	return FALSE;
}

void createCRC(HANDLE h){
	DWORD a, b, index = 4;
	int end = GetFileSize(h, 0);
	a = byteToT<int>(0);
	while (index <= end - 4){
		b = byteToT<int>(index);
		a = (a + b) % MAX_PRIME_NUMBER;
		index += 4;
	}
	setPosition(4);
	writeTToByte(end, a);
}