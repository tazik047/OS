#include "stdafx.h"
#include "MailBox.h"


int _tmain(int argc, _TCHAR* argv[])
{
	MailBox::setFolder(_T("./mailbox"));
	MailBox* mail = new MailBox(_T("myMails"),1000);
	Message* m1 = new Message(_T("Hallo,I'm Ira"));
	Message* m2 = new Message(_T("Today is Monday"));
	Message* m3 = new Message(_T("Heute ist Montag"));

	mail->add(*m1);
	mail->add(*m2);
	mail->add(*m3);

	//mail->readWithMap();
	//mail->deleteByIndex(1);
	//mail->deleteAllMessages();//סלמענועü ג ה‎באדדונו
	//int i = mail->contrSum();
	//printf("sum" + i);
	
	system("pause");
	return 0;
}