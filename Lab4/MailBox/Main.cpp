#include "stdafx.h"
#include "MailBox.h"
#include "MyMain.h"


int _tmain(int argc, _TCHAR* argv[])
{
	/*
	MailBox::setFolder(_T("./mailbox"));
	MailBox* mail = new MailBox(_T("myMails"),1000);
	Message* m1 = new Message(_T("Hallo,I'm Ira"));
	Message* m2 = new Message(_T("Today is Monday"));
	Message* m3 = new Message(_T("Heute ist Montag"));

	mail->add(*m1);
	mail->add(*m2);
	mail->add(*m3);*/

	//mail->readWithMap();
	//mail->deleteByIndex(1);
	//mail->deleteAllMessages();//смотреть в дэбаггере
	//int i = mail->contrSum();
	//printf("sum" + i);

	if (!StartMailBox())
		return 0;
	_tprintf(_T("Для работы с почтовым ящиком введите номер предложенной команды:"));
	TCHAR c = '\0';
	while (c != '0'){
		_tprintf(_T("0. Выйти из программы."));
		_tprintf(_T("1. Добавить новое сообщение."));
		_tprintf(_T("2. Просмотреть сообщение."));
		_tprintf(_T("3. Удалить сообщение."));
		_tprintf(_T("4. Очистить почтовый ящик"));
		_tprintf(_T("5. Просмотреть информацию о почтовом ящике."));
		std::cin >> c;
	}
	ExitFromMailBox();
	//system("pause");
	return 0;
}