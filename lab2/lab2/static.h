#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
	int __stdcall genKey(int &c, int&d);
	int __stdcall crypt(int a, int key, int n);
#ifdef __cplusplus
}
#endif