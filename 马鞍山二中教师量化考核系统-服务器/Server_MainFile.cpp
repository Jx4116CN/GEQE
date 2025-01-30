#include "Server_Init.h"
using namespace std;

int main()
{
	init();

	std::thread thread_DealMsg_SignIn(DealMsg_SignIn);

	string str;
	while (getline(cin, str))
	{
		if (str == "user list")
		{
			bool found = false;
			for (int i = 0; i < Max_Users; i++)
			{
				if (user[i].binding)
				{
					printf("[num:%d][dwConnID:%lld]\n", i, user[i].dwConnID);
					found = true;
				}
			}
			if (!found) printf("None!\n");
		}
		else if (str == "id list")
		{
			for (ULONGLONG i = 1; i < IDtoUser_size; i++)
				printf("[dwConnID:%lld][num:%d]\n", i, IDtoUser[i]);
		}
		else if (str == "SignIn message")
		{
			MSG_SignIn* cMsg_SignIn = Msg_SignIn;
			mtx_Msg_SignIn.lock();
			int i = 0;
			while (cMsg_SignIn->next != NULL)
			{
				i++;
				printf("[%d][total:%d][dwConnID:%lld][message:%s]\n", i, cMsg_SignIn->total, cMsg_SignIn->dwConnID, cMsg_SignIn->pData.c_str());
				cMsg_SignIn = cMsg_SignIn->next;
			}
			mtx_Msg_SignIn.unlock();
		}
		else if (str == "stop")
			break;
	}

	quit();

	return 0;
}