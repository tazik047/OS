#pragma once
/*
#ifdef _STATIC
#define RSA_API
#else
#ifdef _DYNAMIC 
#define RSA_API __declspec(dllexport)
#else 
#define RSA_API __declspec(dllimport)
#endif
#endif
*/ 

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

	MY_VAR int function(int, int);
	MY_VAR int __stdcall genKey(int &c, int&d);
	MY_VAR int __stdcall crypt(int a, int key, int n);
#ifdef __cplusplus
}
#endif

