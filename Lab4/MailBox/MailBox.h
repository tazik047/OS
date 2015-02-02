#ifndef MailBox_h
#define MailBox_h

#include <windows.h>
#include <vector>
#include "Message.h"

class MailBox
{
	

	int messageCount;
	int messagesSize;
	int boxMaxSize;
	std::vector<Message> messages;
	bool autoFlush;
	bool useMap;
	
	TCHAR fileName[MAX_PATH];
	HANDLE handle;
	

	void error(TCHAR* err);
	void write();
	void read();
	void readWithMap();//6
	
	TCHAR* fullPath();

public:
	static TCHAR ext [];//[MAX_PATH]; //= _T(".mailbox");
	static TCHAR folder[MAX_PATH];// = _T("");


	MailBox(TCHAR* fileName,int size);
public:
	~MailBox();
	void add(Message m);
	void areFlush(bool b);
	void areCompressed(bool c);
	void flush();
	int count();
	int contrSum();
	Message getMessageByIndex(int i);
	void deleteAllMessages();
	static void setFolder(TCHAR* f);
	void deleteByIndex(int i);
	int mailBoxCount();
	Message readWithDeliting(int i);
	void setUseMap(bool c);
};


#endif

