#include "stdafx.h"
#include "MyMain.h"
#include "CRC.h"
#include <string>


using namespace std;

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
	_tprintf_s(_T("Для работы с почтовым ящиком введите номер предложенной команды:\n"));
	size_t c = -1;
	while (c != 0){
		_tprintf_s(_T("0. Выйти из программы.\n"));
		_tprintf_s(_T("1. Добавить новое сообщение.\n"));
		_tprintf_s(_T("2. Просмотреть сообщение.\n"));
		_tprintf_s(_T("3. Удалить сообщение.\n"));
		_tprintf_s(_T("4. Очистить почтовый ящик\n"));
		_tprintf_s(_T("5. Просмотреть информацию о почтовом ящике.\n"));
		_tscanf_s(_T("%d"), &c);
		string tempBuf;
		TCHAR* buf;
		switch (c)
		{
		case 0:
			ExitFromMailBox();
			break;
		case 1:
			_tprintf_s(_T("Введите текст сообщения:\n"));
			fflush(stdin);
			getline(std::cin, tempBuf);
			buf = new TCHAR[tempBuf.size() + 1];
			OemToChar(tempBuf.c_str(), buf);
			addNewMessage(buf);
			delete[] buf;
			break;
		case 2:
			_tprintf_s(_T("Введите индекс сообщения:\n"));
			int index1;
			fflush(stdin);
			scanf("%d", &index1);
			ReadMessage(index1);
			break;
		case 3:
			_tprintf_s(_T("Введите индекс для удаления:\n"));
			int index;
			fflush(stdin);
			scanf("%d", &index);
			deleteTheMessage(index);
			break;
		case 4:
			clearTheMailBox();
			break;
		case 5:
			printTheInfo();
			break;
		}

	}
	_tprintf_s(_T("Пока\n"));
	//system("pause");
	return 0;
}