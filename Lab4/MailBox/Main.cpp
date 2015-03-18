#include "stdafx.h"
#include "MailBox.h"
#include "MyMain.h"
#include "CRC.h"


BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		case CTRL_CLOSE_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			ExitFromMailBox();
			return FALSE;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("Russian"));
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
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
			ExitFromMailBox();
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
	_tprintf(_T("Пока\n"));
	//system("pause");
	return 0;
}