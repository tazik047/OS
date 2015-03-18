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

	_tsetlocale(LC_ALL, _T("Russian"));

	if (!StartMailBox())
		return 0;
	_tprintf(_T("Для работы с почтовым ящиком введите номер предложенной команды:\n"));
	size_t c = -1;
	while (c != 0){
		_tprintf(_T("0. Выйти из программы.\n"));
		_tprintf(_T("1. Добавить новое сообщение.\n"));
		_tprintf(_T("2. Просмотреть сообщение.\n"));
		_tprintf(_T("3. Удалить сообщение.\n"));
		_tprintf(_T("4. Очистить почтовый ящик\n"));
		_tprintf(_T("5. Просмотреть информацию о почтовом ящике.\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
		case 0:
			return 0;
			break;
		case 1:			
			_tprintf(_T("Введите текст сообщения:\n"));
			char temp[501];
			fflush(stdin);
			scanf("%[^\r\n]", &temp);
			TCHAR message[501];
			OemToChar(temp, message);
			addNewMessage(message);
			break;
		case 2:
			_tprintf(_T("Введите индекс сообщения:\n"));
			int index1;
			fflush(stdin);
			scanf("%d", &index1);
			ReadMessage(index1);
			break;
		case 3:
			_tprintf(_T("Введите индекс для удаления:\n"));
			int index;
			fflush(stdin);
			scanf("%d", &index);
			deleteTheMessage(index);
			break;
		case 4:
			break;
		case 5:
			printTheInfo();
			break;
		}

	}
	ExitFromMailBox();
	_tprintf(_T("Пока\n"));
	//system("pause");
	return 0;
}