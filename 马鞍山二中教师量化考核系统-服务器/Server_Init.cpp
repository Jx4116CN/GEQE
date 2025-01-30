#include "Server_Init.h"

En_HP_HandleResult OnAccept(HP_Server pSender, HP_CONNID dwConnID, UINT_PTR pClient)
{
	for (int i = 0; i < Max_Users; i++)
	{
		if (!user[i].binding)
		{
			user[i].binding = true;
			user[i].dwConnID = dwConnID;
			memset(user[i].name, 0, sizeof(user[i].name));
			memset(user[i].password, 0, sizeof(user[i].password));

			while (IDtoUser_size <= dwConnID)
			{
				IDtoUser = (int*)realloc(IDtoUser, sizeof(int) * (IDtoUser_size + 1));
				IDtoUser_size++;
			}
			IDtoUser[dwConnID] = i;

			HP_Server_Send(pSender, dwConnID, (BYTE*)&i, sizeof(int));
			break;
		}
	}
	printf("%s %llu\n", __FUNCTION__, dwConnID);
	return HR_IGNORE;
}

En_HP_HandleResult OnReceive(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
	for (int i = 0; i < iLength; i++)
		printf("%c", pData[i]);
	printf("\n");

	std::string data((char*)pData);

	if (NULL != strstr((const char*)pData, "<SignIn>"))
	{
		user[IDtoUser[dwConnID]].state = SIGNIN;
		data.erase(0, 8);
	}
	else if (NULL != strcmp((const char*)pData, "</SignIn>"))
		user[IDtoUser[dwConnID]].state = FREE;

	switch (user[IDtoUser[dwConnID]].state)
	{
	case SIGNIN:
		MSG_SignIn* cMsg_SignIn = Msg_SignIn;
		while (cMsg_SignIn->next != NULL)
			cMsg_SignIn = cMsg_SignIn->next;
		mtx_Msg_SignIn.lock();
		cMsg_SignIn->next = new MSG_SignIn;
		cMsg_SignIn->next->total = cMsg_SignIn->total + 1;
		cMsg_SignIn->pSender = pSender;
		cMsg_SignIn->dwConnID = dwConnID;
		cMsg_SignIn->pData = data;
		cMsg_SignIn->iLength = iLength;
		mtx_Msg_SignIn.unlock();
	}

	return HR_OK;
}

En_HP_HandleResult OnClose(HP_Server pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
{
	for (int i = 0; i < Max_Users; i++)
	{
		if (user[i].dwConnID == dwConnID)
		{
			user[i].binding = false;
			break;
		}
	}
	printf("%s %llu\n", __FUNCTION__, dwConnID);
	return HR_IGNORE;
}

void init()
{
	listener = Create_HP_TcpServerListener();
	server = Create_HP_TcpServer(listener);

	HP_Set_FN_Server_OnAccept(listener, OnAccept);
	HP_Set_FN_Server_OnReceive(listener, OnReceive);
	HP_Set_FN_Server_OnClose(listener, OnClose);

	HP_Server_Start(server, "0.0.0.0", 21611);
}

void quit()
{
	HP_Server_Stop(server);
	Destroy_HP_TcpServer(server);
	Destroy_HP_TcpServerListener(listener);
}