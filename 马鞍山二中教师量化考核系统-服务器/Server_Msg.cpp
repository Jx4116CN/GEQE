#include "Server_Init.h"

void DealMsg_SignIn()
{
	while (true)
	{
		if (Msg_SignIn->next != NULL)
		{
			mtx_Msg_SignIn.lock();
			MSG_SignIn cMsg_SignIn;
			cMsg_SignIn.total = Msg_SignIn->total;
			cMsg_SignIn.pSender = Msg_SignIn->pSender;
			cMsg_SignIn.dwConnID = Msg_SignIn->dwConnID;
			cMsg_SignIn.pData = Msg_SignIn->pData;
			cMsg_SignIn.iLength = Msg_SignIn->iLength;

			MSG_SignIn* copy = Msg_SignIn;
			Msg_SignIn = Msg_SignIn->next;
			delete(copy);
			mtx_Msg_SignIn.unlock();

			std::string telephone = cMsg_SignIn.pData, password = cMsg_SignIn.pData;
			int p = telephone.find('\n');
			telephone.erase(p, cMsg_SignIn.iLength);
			password.erase(0, p + 1);
			char path[64] = { 0 };
			sprintf(path, "%s%s\\%s.password", Path_UserData, telephone.c_str(), password.c_str());
			std::ifstream file(path);
			if (!file)
			{
				std::string str = "<SignIn:failed>";
				HP_Server_Send(cMsg_SignIn.pSender, cMsg_SignIn.dwConnID, (const BYTE*)str.c_str(), str.size() + 1);
			}
			else
			{
				std::string str;
				std::getline(file, str);
				str = "<SignIn:scceeded>" + str;
				HP_Server_Send(cMsg_SignIn.pSender, cMsg_SignIn.dwConnID, (const BYTE*)str.c_str(), str.size() + 1);

				char path_data[64] = { 0 };
				sprintf(path_data, "%s%s\\basic.data", Path_UserData, telephone.c_str());
				std::ifstream file_data(path_data);
				if (file_data.is_open())
				{
					std::string data = "<BasicData>";
					std::string data_each;
					while (getline(file_data, data_each))
						data += "\n" + data_each;
					HP_Server_Send(cMsg_SignIn.pSender, cMsg_SignIn.dwConnID, (const BYTE*)data.c_str(), data.size() + 1);
				}
			}
			file.close();
		}
		else
			Sleep(10);
	}
}