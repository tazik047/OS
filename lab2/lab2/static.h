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

	RCA_crypt long long __stdcall genkey(long long &d, long long &e);
	RCA_crypt long long* __stdcall crypt(long long* a, long long key, long long n, int l);
	RCA_crypt long long* __stdcall ToLong(char* a, int &l);
	RCA_crypt char* __stdcall ToChar(long long* a, int l);

#ifdef __cplusplus
}
#endif