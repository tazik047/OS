#include "stdafx.h"
#include "static.h"
#include <stdlib.h>
#include <vector>
//#include <stdlib.h>
//#include <time.h>

using namespace std;

long long simple(){
	//srand(time(NULL));
	long long n = (rand() % 1000 + 3) | 1; //нечетное число

	for (long i = 3; i * i <= n; i += 2)//проверка на простоту
	if (n % i == 0){//если составное, увеличиваем n и идем сначала
		n += 2;
		i = 1;
	}
	return n;
}

long long GCD(long long a, long long b){
	if (b == 0) return a;
	else return GCD(b, a % b);
}

long long __stdcall genkey(long long &d, long long &e){

	long long p = simple(), q = simple();// два простых неравных числа
	while (q == p)
		q = simple();
	long long n = p * q;// "модуль" n
	long long f = (p - 1) * (q - 1);// функция Эйлера	 
	for (e = 3; GCD(e, f) != 1; e++);// ключ для шифрования (открытый ключ e и n). взаимно простой с f
	for (d = 3; (d * e) % f != 1; d += 2);  // ключ для расшифровки (секретный ключ d и n)
	return n;
}

long long* __stdcall crypt(long long* a, long long key, long long n, int l){
	long long k = key;//ключ
	for (int i = 0; i <= l; i++){//для каждого значения в массиве
		long res = 1;
		while (k > 0){//шифруем
			if (k & 1)
				res = (res * a[i]) % n;
			a[i] = (a[i] * a[i]) % n;
			k >>= 1;
		}
		a[i] = res;
		k = key;
	}
	return a;
}

long long* __stdcall ToLong(char* a, int &l){//представление каждого cимвола в массиве числом 
	long long* result = new long long[strlen(a)];
	l = strlen(a);
	for (int i = 0; i <= l; i++){
		result[i] = (unsigned int)(a[i]);
	}

	return result;
}
char* __stdcall ToChar(long long* a, int l){//представление числа символом
	char* result = new char[l+1];
	for (int i = 0; i <= l; i++){
		result[i] = (char)(a[i]);
	}
	return result;
}
