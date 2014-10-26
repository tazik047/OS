// dynamicLib.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "..\lab2\static.h"

MY_VAR int function(int s, int f){ return s + f; }
