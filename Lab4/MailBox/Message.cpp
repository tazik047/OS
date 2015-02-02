#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include "Message.h"

Message::Message(TCHAR* data)//constuctor
{
	length = _tcslen(data) + 1;

		text = new TCHAR[length ];
		_tcscpy(text, data);
}
Message::~Message()//destructor
{
	if(length > 0)
	{
		//delete(text);!!!!!!??
	}
}

int Message::getLength()
{
	return length;
}

TCHAR* Message::getText()
{
	return text;
}
int Message::size()
{
	return sizeof(int)+sizeof(TCHAR)*length;//размер в байтах
}
int Message::controlSum()
{
	int sum = length;
	int flag = -1;
	if(IsTextUnicode( text, sizeof (text), &flag))
	{
		wchar_t c;
		for(int i = 0;i < length;i++)
		{
			c = text[i];
			sum += c;
		}
	}
	else
	{
		char c;
		for(int i = 0;i < length;i++)
		{
			c = text[i];
			sum += c;
		}
	}
	return sum;
}