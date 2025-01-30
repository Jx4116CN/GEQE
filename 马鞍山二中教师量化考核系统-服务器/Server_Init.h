#pragma once
#include <iostream>
#include <string>
#include <HPSocket/HPSocket4C.h>
#include <thread>
#include <mutex>
#include <fstream>
#pragma execution_character_set("utf-8")

#define Max_Users 1024

enum user_state
{
	FREE = 0,
	SIGNIN = 1,
	SIGNUP = 2
};

struct User
{
	bool binding = false;
	HP_CONNID dwConnID;
	user_state state = FREE;
	char name[32] = { 0 };
	char password[16] = { 0 };
};

__declspec(selectany) User user[Max_Users];

__declspec(selectany) int* IDtoUser = new int[1];
__declspec(selectany) size_t IDtoUser_size = 1;

__declspec(selectany) const char* Path_UserData = "UserData\\";

struct MSG_SignIn
{
	int total = 1;
	HP_Server pSender;
	HP_CONNID dwConnID;
	std::string pData;
	int iLength;
	struct MSG_SignIn* next = NULL;
};
__declspec(selectany) MSG_SignIn* Msg_SignIn = new MSG_SignIn;
__declspec(selectany) std::mutex mtx_Msg_SignIn;

void DealMsg_SignIn();

__declspec(selectany) HP_TcpServerListener listener;
__declspec(selectany) HP_TcpServer server;

En_HP_HandleResult OnAccept(HP_Server pSender, HP_CONNID dwConnID, UINT_PTR pClient);
En_HP_HandleResult OnReceive(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
En_HP_HandleResult OnClose(HP_Server pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);

void init();
void quit();