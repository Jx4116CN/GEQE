#include "Client_Init.h"

En_HP_HandleResult OnConnect(HP_Client pSender, HP_CONNID dwConnID)
{
	printf("%s %llu\n", __FUNCTION__, dwConnID);
	HP_TcpConnect = true;
	return HR_IGNORE;
}

En_HP_HandleResult OnReceive(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
	for (int i = 0; i < iLength; i++)
		printf("%c", pData[i]);
	printf("\n");

	string data((char*)pData);

	if (iLength == 4)
	{
		ConnNUM = *(int*)pData;
		printf("%s NUM:%d ID:%llu\n", __FUNCTION__, ConnNUM, dwConnID);
		return HR_OK;
	}
	else if (data.find("<SignIn:failed>") != std::string::npos)
	{
		Login_State = FAILED;
		return HR_OK;
	}
	else if (data.find("<SignIn:scceeded>") != std::string::npos)
	{
		if (data.find("admin") != std::string::npos)
		{
			Login_State = admin;
			user.permission = "admin";
		}
		else if (data.find("leader") != std::string::npos)
		{
			Login_State = leader;
			user.permission = "leader";
		}
		else
		{
			Login_State = teacher;
			user.permission = "teacher";
		}
		return HR_OK;
	}
	else if (data.find("<BasicData>") != std::string::npos)
	{
		data.erase(0, 11);
		if (data.find("<name>") != std::string::npos)
		{
			int start = data.find("<name>") + 6, end = data.find("</name>");
			user.name = data.substr(start, end - start);
		}
		if (data.find("<year_up>") != std::string::npos)
		{
			int start = data.find("<year_up>") + 9, end = data.find("</year_up>");
			user.year_up = data.substr(start, end - start);
		}
		if (data.find("<month_up>") != std::string::npos)
		{
			int start = data.find("<month_up>") + 10, end = data.find("</month_up>");
			user.month_up = data.substr(start, end - start);
		}
		if (data.find("<day_up>") != std::string::npos)
		{
			int start = data.find("<day_up>") + 8, end = data.find("</day_up>");
			user.day_up = data.substr(start, end - start);
		}
		if (data.find("<hour_up>") != std::string::npos)
		{
			int start = data.find("<hour_up>") + 9, end = data.find("</hour_up>");
			user.hour_up = data.substr(start, end - start);
		}
		if (data.find("<minute_up>") != std::string::npos)
		{
			int start = data.find("<minute_up>") + 11, end = data.find("</minute_up>");
			user.minute_up = data.substr(start, end - start);
		}
		if (data.find("<second_up>") != std::string::npos)
		{
			int start = data.find("<second_up>") + 11, end = data.find("</second_up>");
			user.second_up = data.substr(start, end - start);
		}
		return HR_OK;
	}
	return HR_IGNORE;
}

En_HP_HandleResult OnClose(HP_Client pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
{
	printf("%s %llu\n", __FUNCTION__, dwConnID);
	HP_TcpConnect = false;
	char tip[MAX_TIP_LEN] = { 0 };
	sprintf(
		tip,
		"%s\nError code: %d",
		"The server has been closed!",
		GetLastError());
	MessageBox(
		NULL,
		tip,
		"Error",
		MB_OK | MB_ICONERROR);
	return HR_IGNORE;
}

inline void Connect()
{
Init_Client_Start:
	if (!HP_Client_Start(Client, "103.40.13.6", 20066, false))
	{
		char tip[MAX_TIP_LEN] = { 0 };
		sprintf(
			tip,
			"%s\nError code: %d",
			"Unable to connect to server!",
			GetLastError());
		if (IDCANCEL == MessageBox(
			NULL,
			tip,
			"Error",
			MB_RETRYCANCEL | MB_ICONERROR))
			exit(NULL);
		else
			goto Init_Client_Start;
	}

	HP_Set_FN_Client_OnConnect(Listener, OnConnect);
	HP_Set_FN_Client_OnReceive(Listener, OnReceive);
	HP_Set_FN_Client_OnClose(Listener, OnClose);
}
inline void Unconnect()
{
	HP_Client_Stop(Client);
	Destroy_HP_TcpClient(Client);
	Destroy_HP_TcpClientListener(Listener);
}

inline void Init_Win_Login()
{
Init_Window:
	Win_Login = SDL_CreateWindow(
		"马鞍山二中教师量化考核系统 - 登录",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		winW,
		winH,
		NULL);
	if (!Win_Login)
	{
		if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Window;
	}

Init_rdr:
	Rdr_Login = SDL_CreateRenderer(Win_Login, -1, NULL);
	if (!Rdr_Login)
	{
		if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else goto Init_rdr;
	}
	SDL_SetRenderDrawBlendMode(Rdr_Login, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}
inline void Quit_Win_Login()
{
	SDL_DestroyRenderer(Rdr_Login);
	SDL_DestroyWindow(Win_Login);
}
inline void Init_Win_Main()
{
Init_Window:
	winW = screenW / 2;
	winH = screenH / 2;
	Win_Main = SDL_CreateWindow(
		"马鞍山二中教师量化考核系统 - 客户端",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		winW,
		winH,
		SDL_WINDOW_RESIZABLE);
	if (!Win_Main)
	{
		if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Window;
	}
	SDL_SetWindowMinimumSize(Win_Main, winW, winH);

Init_rdr:
	Rdr_Main = SDL_CreateRenderer(Win_Main, -1, NULL);
	if (!Rdr_Main)
	{
		if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else goto Init_rdr;
	}
	SDL_SetRenderDrawBlendMode(Rdr_Main, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}
inline void Quit_Win_Main()
{
	SDL_DestroyRenderer(Rdr_Main);
	SDL_DestroyWindow(Win_Main);
}

void Init_Image()
{
Init_BG:
	Surf_Img_BG = IMG_Load(Path_Image_BG);
	Surf_Img_BG = SDL_ConvertSurfaceFormat(Surf_Img_BG, SDL_PIXELFORMAT_RGBA8888, NULL);
	if (NULL == Surf_Img_BG)
	{
		if (IDCANCEL == IMG_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_BG;
	}
	Ture_Img_BG = SDL_CreateTextureFromSurface(Rdr_Login, Surf_Img_BG);

Init_Icon:
	Surf_Img_Icon = IMG_Load(Path_Image_Icon);
	Surf_Img_Icon = SDL_ConvertSurfaceFormat(Surf_Img_Icon, SDL_PIXELFORMAT_RGBA8888, NULL);
	if (NULL == Surf_Img_Icon)
	{
		if (IDCANCEL == IMG_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Icon;
	}
	Ture_Img_Icon = SDL_CreateTextureFromSurface(Rdr_Login, Surf_Img_Icon);
}
void Init_Image_Main()
{
	Ture_Img_Icon = SDL_CreateTextureFromSurface(Rdr_Main, Surf_Img_Icon);

Init_BG2:
	Surf_Img_BG2 = IMG_Load(Path_Image_BG2);
	Surf_Img_BG2 = SDL_ConvertSurfaceFormat(Surf_Img_BG2, SDL_PIXELFORMAT_RGBA8888, NULL);
	if (NULL == Surf_Img_BG2)
	{
		if (IDCANCEL == IMG_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_BG2;
	}
	Ture_Img_BG2 = SDL_CreateTextureFromSurface(Rdr_Main, Surf_Img_BG2);
}

void Init_Font()
{
Init_Hei:
	Font_Hei = TTF_OpenFont(Path_Font_Hei, TTF_Font_Initial_Value);
	if (!Font_Hei)
	{
		if (IDCANCEL == TTF_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Hei;
	}
	TTF_SetFontSizeDPI(Font_Hei, TTF_Font_Initial_Value, TTF_Font_Initial_Value, TTF_Font_Initial_Value);
	TTF_SetFontSDF(Font_Hei, SDL_TRUE);

Init_Deng:
	Font_Deng = TTF_OpenFont(Path_Font_Deng, TTF_Font_Initial_Value);
	if (!Font_Deng)
	{
		if (IDCANCEL == TTF_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Deng;
	}
	TTF_SetFontSizeDPI(Font_Deng, TTF_Font_Initial_Value, TTF_Font_Initial_Value, TTF_Font_Initial_Value);

Init_Sun:
	Font_Sun = TTF_OpenFont(Path_Font_Sun, TTF_Font_Initial_Value);
	if (!Font_Sun)
	{
		if (IDCANCEL == TTF_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Sun;
	}
	TTF_SetFontSizeDPI(Font_Sun, TTF_Font_Initial_Value, TTF_Font_Initial_Value, TTF_Font_Initial_Value);
}

void Init_Login()
{
	Connect();

	//Init device
	{
	Init_SDL:
		if (NULL != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		{

			if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
				exit(NULL);
			else
				goto Init_SDL;
		}
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	Init_TTF:
		if (NULL != TTF_Init())
		{

			if (IDCANCEL == TTF_ErrorMessageBox(SDL_MB_RETRY))
				exit(NULL);
			else
				goto Init_TTF;
		}

	Init_IMG:
		if (!IMG_Init(IMG_INIT_PNG))
		{
			if (IDCANCEL == IMG_ErrorMessageBox(SDL_MB_RETRY))
				exit(NULL);
			else
				goto Init_IMG;
		}
	}

	Init_Win_Login();

	Init_Image();

	Init_Font();

Init_autofill:
	ifstream file(Path_autofill);
	if (!file.is_open())
	{
		char str[MAX_TIP_LEN] = { 0 };
		sprintf(str, "Failed to open the file - %s\nError code:%d", Path_autofill, GetLastError());
		if (IDRETRY == MessageBoxA(NULL, str, "Error", SDL_MB_RETRY))
			goto Init_autofill;
	}
	else
	{
		if (getline(file, autofill_telephone)) autofill = true;
		if (getline(file, autofill_password)) autofill = true;
		file.close();
	}
}
void Quit_Login()
{
	Quit_Win_Login();
}
void Init_Main()
{
	Init_Win_Main();

	int font_size = winH / 8;
	TTF_SetFontSDF(Font_Hei, SDL_FALSE);
	TTF_SetFontSize(Font_Hei, font_size);
	SDL_Rect Rect_Text;
	Rect_Text.h = font_size;
	Rect_Text.w = Rect_Text.h * 4;
	Rect_Text.x = (winW - Rect_Text.w) / 2;
	Rect_Text.y = (winH - Rect_Text.h) / 2;

Init_Text_Loading:
	Surf_Text_Loading = TTF_RenderUTF8_Blended(Font_Hei, "加载中…", 0xFFFFFFFF);
	if (NULL == Surf_Text_Loading)
	{
		if (IDCANCEL == TTF_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Text_Loading;
	}
	Ture_Text_Loading = SDL_CreateTextureFromSurface(Rdr_Main, Surf_Text_Loading);
	if (NULL == Ture_Text_Loading)
	{
		if (IDCANCEL == SDL_ErrorMessageBox(SDL_MB_RETRY))
			exit(NULL);
		else
			goto Init_Text_Loading;
	}

	SDL_RenderCopyFadeInLoad(Rdr_Main, Surf_Text_Loading, NULL, &Rect_Text, 1000, 50);

	Init_Image_Main();
}
void Quit_Main()
{
	Quit_Win_Main();
}

void Quit()
{
	SDL_FreeSurface(Surf_Img_BG);
	SDL_DestroyTexture(Ture_Img_BG);

	exit(0);
}