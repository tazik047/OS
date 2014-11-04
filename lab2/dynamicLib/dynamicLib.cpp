// dynamicLib.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#define DYNAMIC
#include "MY_VAR.h"
#include <stdlib.h>


int simple() {
	int r = (rand() % 100 + 3) | 1;
	bool b = true;
	while (b) {
		for (int i = 3; i*i <= r; i = i + 2) {
			if (r%i == 0) {
				b = false;
				break;
			}
		}
		if (!b) {
			r += 2;
			b = true;
		}
		else break;

	}
	return r;
}

int gcd(int a, int b) {
	while (a != 0 || b != 0) {
		if (a > b) {
			a = a%b;
		}
		else {
			b = b%a;
		}
	}
	return a + b;
}
MY_VAR int __stdcall genKey(int &c, int&d) {
	int p = simple();
	int q = p;
	while (q == p) {
		q = simple();
	}
	int n = p*q;
	int u = (p - 1)*(q - 1);
	for (c = 3; gcd(c, u) != 1; c += 2);
	for (d = 3; (d*c) % 4 != 1; d++);
	return n;
}
MY_VAR int __stdcall crypt(int a, int key, int n) {
	int b = a;
	int r = key;
	int count = 0;
	while (r != 0) {
		r = r / 2;
		count++;
	}
	count -= 2;
	for (int i = count; i >= 0; i--) {
		b = b*b%n;
		if (key & (1 << i)) b = b*a%n;
	}
	return b;
}
