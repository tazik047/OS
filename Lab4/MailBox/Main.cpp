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
	//mail->deleteAllMessages();//�������� � ���������
	//int i = mail->contrSum();
	//printf("sum" + i);

	_tsetlocale(LC_ALL, _T("Russian"));

	if (!StartMailBox())
		return 0;
	_tprintf(_T("��� ������ � �������� ������ ������� ����� ������������ �������:\n"));
	size_t c = -1;
	while (c != 0){
		_tprintf(_T("0. ����� �� ���������.\n"));
		_tprintf(_T("1. �������� ����� ���������.\n"));
		_tprintf(_T("2. ����������� ���������.\n"));
		_tprintf(_T("3. ������� ���������.\n"));
		_tprintf(_T("4. �������� �������� ����\n"));
		_tprintf(_T("5. ����������� ���������� � �������� �����.\n"));
		_tscanf_s(_T("%d"), &c);
		switch (c)
		{
		case 0:
			return 0;
			break;
		case 1:
			TCHAR message[500];
			_tprintf(_T("������� ����� ���������:\n"));
			_tscanf_s(_T("%qs"), &message);
			addNewMessage(message);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			getMailBoxInformation();
			break;
		}

	}
	ExitFromMailBox();
	_tprintf(_T("����\n"));
	//system("pause");
	return 0;
}