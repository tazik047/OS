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

template <typename T>
T byteToT(int);

template <typename T>
void writeTToByte(int, T);

void reduceFile(int);

void setPosition(int);