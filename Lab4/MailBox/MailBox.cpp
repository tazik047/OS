#include "stdafx.h"
#include "MailBox.h"

TCHAR MailBox::ext [] = _T(".mailbox");
TCHAR MailBox::folder [MAX_PATH]= _T(".");
MailBox::MailBox(TCHAR* fName,int size)
{
	autoFlush = true;
	useMap = true;
	 WIN32_FIND_DATA findFileData;//ищем файл
	 _tcscpy(fileName, fName);
	 handle = FindFirstFile(fullPath(), &findFileData);
	 
		if (handle == INVALID_HANDLE_VALUE)//если его нет 
		{
			messageCount = 0;
			messagesSize = 0;

			boxMaxSize = size;
			if(autoFlush)
				write();
			//error(_T("Invalid File Handle"));
		} 

		else
		{

			FindClose(handle);
			if(useMap) //6
				readWithMap();
			else
				read();
		}
	 
}
void MailBox::error(TCHAR* err)
{
	printf ("%s . GetLastError reports %d\n",err,
             GetLastError ());
}

void MailBox::write()
{
	handle = CreateFile (fullPath(), GENERIC_WRITE,  0, 0,
                OPEN_ALWAYS,  0,  0 );
        	if (handle == INVALID_HANDLE_VALUE)
			{
				error(_T("W"));
				return;
			}
			LONG lPart = 0, hPart = 0;// указатель на начало файла
			SetFilePointer (handle, lPart, &hPart, FILE_BEGIN);
			DWORD Count=0;
			
			WriteFile (handle, &messageCount, sizeof (int), &Count, 0);//пишем messageCount
			WriteFile (handle, &messagesSize, sizeof (int), &Count, 0);
			WriteFile (handle, &boxMaxSize, sizeof (int), &Count, 0);
			
			for(int i = 0;i < messageCount;i++)//пишем сообщения из вектора
			{
				Message m = messages[i];
				int l = m.getLength();
				TCHAR* t = m.getText();
				WriteFile (handle, &l, sizeof (int), &Count, 0);
				WriteFile (handle, t, sizeof (TCHAR)*l, &Count, 0);
			}
			CloseHandle (handle);
}

void MailBox::read()
{
	handle = CreateFile (fullPath(), GENERIC_READ,  0, 0,
                OPEN_ALWAYS,  0,  0 );
        	if (handle == INVALID_HANDLE_VALUE)
			{
				error(_T("R"));
				return;
			}
			LONG lPart = 0, hPart = 0;// указатель на начало файла
			SetFilePointer (handle, lPart, &hPart, FILE_BEGIN);
			DWORD Count=0;
			ReadFile (handle, &messageCount, sizeof (int), &Count,0);
			if (Count == 0) 
			{
				error(_T("1"));
				return;
			}
			ReadFile (handle, &messagesSize, sizeof (int), &Count, 0);
			if (Count == 0) 
			{
				error(_T("2"));
				return;
			}

			ReadFile (handle, &boxMaxSize, sizeof (int), &Count, 0);
			if (Count == 0) 
			{
				error(_T("3"));
				return;
			}
			for(int i = 0;i < messageCount;i++)
			{
				int l;
				ReadFile (handle, &l, sizeof (int), &Count, 0);
				if (Count == 0) 
				{
					error(_T("length"));
					return;
				}
				TCHAR* t = new TCHAR[l];
				ReadFile (handle, t, sizeof (TCHAR)*l, &Count, 0);
				if (Count == 0) 
				{
					error(_T("text"));
					return;
				}

				Message* m = new Message(t);
				messages.insert(messages.end(), *m);
			}
			CloseHandle (handle);
}
void MailBox::add(Message m)
{
	messages.insert(messages.end(), m);
	messageCount++;
	messagesSize += m.size();
	if(autoFlush)
		write();
}
MailBox::~MailBox()
{
		for(int i = 0;i < messageCount;i++)
		{
			delete(&messages[i]);
		}
		//delete(&messages);???
}
void MailBox::areFlush(bool b)
{
	autoFlush = b;
}
void MailBox::flush()
{
	if(!autoFlush)
		write();
}
int MailBox::count()
{
	return messageCount;
}
int MailBox::contrSum()
{
	int sum = messageCount + messagesSize + boxMaxSize;
	for(int i = 0;i > messageCount;i++)
	{
		sum += messages[i].controlSum();
	}
	return sum;
}
void MailBox::deleteByIndex(int i)
{
	if((i >= 0)&&(i < messageCount))
	{
		messageCount--;
		messagesSize -= messages[i].size();
		messages.erase(messages.begin() + i);
	}
	if(autoFlush)
			write();
}
Message MailBox::getMessageByIndex(int i)
{
	if((i < messageCount)&&(i >= 0))
		return messages[i];
	else
		return NULL;
}
void MailBox::deleteAllMessages()//получается слишком большой файл,спросить!!
{
	messages.clear();
	messageCount = 0;
	messagesSize = 0;
	if(autoFlush)
		write();
}
void MailBox::setFolder(TCHAR* f)
{
	_tcscpy(folder, f);//куда-откуда
	CreateDirectory(folder,NULL);
}
TCHAR* MailBox::fullPath()
{
	TCHAR* path = new TCHAR[_tcslen(folder) + _tcslen(ext) + _tcslen(fileName) + 2];
	_tcscpy(path, folder);
	_tcscat(path,__TEXT("/"));
	_tcscat (path, fileName);
	_tcscat (path, ext);
	return path;

}
int MailBox::mailBoxCount()
{
	DWORD boxesCount = 0;
	TCHAR temp[MAX_PATH];
	WIN32_FIND_DATA findFile;
	_tcscpy(temp, folder);
	_tcscat(temp,__TEXT("/*"));
	_tcscat (temp, ext);
	handle = FindFirstFile(temp, &findFile);
	while(handle != INVALID_HANDLE_VALUE)
	{
	      boxesCount++;
	      if(FindNextFile(handle, &findFile) == FALSE)
	        break;
	}
	return boxesCount;
}
Message MailBox::readWithDeliting(int i)
{
	Message m = getMessageByIndex(i);
	deleteByIndex(i);
	return m;
}



//6 th laba

void MailBox::setUseMap(bool c)
{
	useMap = c;
}
void MailBox::readWithMap()
{
	 handle = CreateFile (fullPath(), //file name
		GENERIC_READ|GENERIC_WRITE, //read /write
		0, // don't share
		NULL, // default security
		OPEN_EXISTING, // existing
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template
	if (handle == INVALID_HANDLE_VALUE) 
		return;//error
	unsigned int iFileSize = GetFileSize(handle,NULL);
	HANDLE hFileMapping = CreateFileMapping(handle, NULL,PAGE_READWRITE,0,iFileSize,NULL);
	if (!hFileMapping)
	{
		CloseHandle (handle); 
		return;//error
	}
	PBYTE memoryPointer = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, iFileSize );
	if (!memoryPointer)
	{
		CloseHandle (hFileMapping); CloseHandle (handle);
		return;
	}
	messageCount = *((int*)memoryPointer);
	memoryPointer += sizeof(int);
	messagesSize = *((int*)memoryPointer);
	memoryPointer += sizeof(int);
	boxMaxSize = *((int*)memoryPointer);
	memoryPointer += sizeof(int);
	for(int i = 0;i < messageCount;i++)
	{
		memoryPointer += sizeof(int);
		Message* m = new Message((TCHAR*)memoryPointer);
		messages.insert(messages.end(), *m);
		memoryPointer += (m->getLength()) * sizeof(TCHAR);
	}
	UnmapViewOfFile(memoryPointer);
	CloseHandle(hFileMapping);
	CloseHandle (handle);
}





