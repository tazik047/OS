#pragma once

#ifdef _STATIC 
#define MY_VAR
#else
#ifdef _USRDLL
#define MY_VAR __declspec(dllexport)
#else
#define MY_VAR __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

	MY_VAR int __stdcall genKey(int &c, int&d);
	MY_VAR int __stdcall crypt(int a, int key, int n);
#ifdef __cplusplus
}
#endif

