#include<iostream>
#include<Windows.h>


void printError();

BOOL StartMailBox();

void ExitFromMailBox();

void getMailBoxInformation();

void addNewMessage(TCHAR* mess);

void deleteTheMessage(int index);

void ReadMessage(int index);

void printTheInfo();

void clearTheMailBox();