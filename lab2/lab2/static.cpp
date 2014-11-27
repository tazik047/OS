#include "stdafx.h"
#include "static.h"
#include <stdlib.h>
#include <vector>
//#include <stdlib.h>
//#include <time.h>

using namespace std;

long long simple(){
	//srand(time(NULL));
	long long n = (rand() % 1000 + 3) | 1; //�������� �����

	for (long i = 3; i * i <= n; i += 2)//�������� �� ��������
	if (n % i == 0){//���� ���������, ����������� n � ���� �������
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

	long long p = simple(), q = simple();// ��� ������� �������� �����
	while (q == p)
		q = simple();
	long long n = p * q;// "������" n
	long long f = (p - 1) * (q - 1);// ������� ������	 
	for (e = 3; GCD(e, f) != 1; e++);// ���� ��� ���������� (�������� ���� e � n). ������� ������� � f
	for (d = 3; (d * e) % f != 1; d += 2);  // ���� ��� ����������� (��������� ���� d � n)
	return n;
}

long long* __stdcall crypt(long long* a, long long key, long long n, int l){
	long long k = key;//����
	for (int i = 0; i <= l; i++){//��� ������� �������� � �������
		long res = 1;
		while (k > 0){//�������
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

long long* __stdcall ToLong(char* a, int &l){//������������� ������� c������ � ������� ������ 
	long long* result = new long long[strlen(a)];
	l = strlen(a);
	for (int i = 0; i <= l; i++){
		result[i] = (unsigned int)(a[i]);
	}

	return result;
}
char* __stdcall ToChar(long long* a, int l){//������������� ����� ��������
	char* result = new char[l+1];
	for (int i = 0; i <= l; i++){
		result[i] = (char)(a[i]);
	}
	return result;
}
