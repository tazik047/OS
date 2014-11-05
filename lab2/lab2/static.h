#pragma once

#ifdef _STATIC 
#define RCA_crypt
#else
#ifdef _USRDLL
#define RCA_crypt __declspec(dllexport)
#else
#define RCA_crypt __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

	RCA_crypt int __stdcall genKey(int &c, int&d);
	RCA_crypt int __stdcall crypt(int a, int key, int n);
#ifdef __cplusplus
}
#endif