#ifndef Message_h
#define Message_h

#include <tchar.h>

class Message
{
	int length;
	TCHAR* text;

	public:
		//Message();
		Message(TCHAR* data);
		~Message();
		int getLength();
		TCHAR* getText();
		int size();
		int controlSum();
		
};

#endif