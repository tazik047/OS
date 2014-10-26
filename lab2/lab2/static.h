#pragma once

#ifdef _STATIC 
	#define MY_VAR
#else
	#ifdef _USERDLL
#define MY_VAR __declspec(dllimport)
	#else
#define MY_VAR __declspec(dllexport)
	#endif
#endif

MY_VAR int function(int, int);