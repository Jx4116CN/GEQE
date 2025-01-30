#include "Client_Init.h"

void MainWin()
{
	SDL_Event Event;

	SDL_Buffer* buffer = NULL;
	buffer = SDL_CreateBufferFromSurface(SDL_GetWindowSurface(Win_Main), GetTickCount64(), 1000);

	ULONGLONG Time_Start = GetTickCount64();
	ULONGLONG Time_Cycle = Time_Start;
	ULONGLONG Time_Last;
	int fps = 0;

	int space_line;

	SDL_Rect srcRect_BG2;

	SDL_Rect Rect_Icon;
	SDL_Rect Rect_TopBar;
	SDL_Rect Rect_LeftBar;

	const char* Text_DataTitle[4] = { "姓名:","权限:","手机号:","注册日期:" };
	SDL_Rect Rect_DataTitle[4];
	SDL_Surface* Surf_DataTitle[4];
	SDL_Texture* Ture_DataTitle[4];
	char Text_Data[4][64] = { " \0", "\0", "\0" };
	sprintf(Text_Data[0], "%s", user.name.c_str());
	sprintf(Text_Data[1], "%s", user.permission.c_str());
	sprintf(Text_Data[2], "%s", user.telephone.c_str());
	sprintf(Text_Data[3], "%s/%s/%s", user.year_up.c_str(), user.month_up.c_str(), user.day_up.c_str());
	SDL_Rect Rect_Data[4];
	SDL_Surface* Surf_Data[4];
	SDL_Texture* Ture_Data[4];

	const char* Text_Button[4] = { "参与评测","管理评测","修改用户信息","修改学校信息" };
	SDL_Rect Rect_Button[4];
	SDL_Button* Button[4] = { NULL,NULL,NULL,NULL };

	const char* Text_TButton = "修改密码";
	SDL_Point Point_TButton;
	SDL_TextButton* TButton = NULL;

Init:
	space_line = screenH / 55 - (screenH * 5 / winH);

	if (winW * Surf_Img_BG2->h / Surf_Img_BG2->w < winH)
	{
		srcRect_BG2.h = Surf_Img_BG2->h;
		srcRect_BG2.w = srcRect_BG2.h * winW / winH;
	}
	else
	{
		srcRect_BG2.w = Surf_Img_BG2->w;
		srcRect_BG2.h = srcRect_BG2.w * winH / winW;
	}
	srcRect_BG2.x = (Surf_Img_BG2->w - srcRect_BG2.w) / 2;
	srcRect_BG2.y = (Surf_Img_BG2->h - srcRect_BG2.h) / 2;

	Rect_Icon.x = space_line;
	Rect_Icon.y = space_line;
	Rect_Icon.h = space_line * 7;
	Rect_Icon.w = Rect_Icon.h * Surf_Img_Icon->w / Surf_Img_Icon->h;

	Rect_TopBar.x = 0;
	Rect_TopBar.y = 0;
	Rect_TopBar.w = winW;
	Rect_TopBar.h = Rect_Icon.h + Rect_Icon.x * 2;

	Rect_LeftBar.x = 0;
	Rect_LeftBar.y = Rect_TopBar.y + Rect_TopBar.h;
	Rect_LeftBar.w = space_line * 15;
	Rect_LeftBar.h = winH - Rect_LeftBar.y;

	TTF_SetFontSize(Font_Hei, space_line * 2);
	TTF_SetFontSize(Font_Deng, space_line * 5 / 3);
	for (int i = 0; i < 4; i++)
	{
		Surf_DataTitle[i] = TTF_RenderUTF8_Blended(Font_Hei, Text_DataTitle[i], black);
		Ture_DataTitle[i] = SDL_CreateTextureFromSurface(Rdr_Main, Surf_DataTitle[i]);
		Rect_DataTitle[i].w = Surf_DataTitle[i]->w;
		Rect_DataTitle[i].h = Surf_DataTitle[i]->h;
		Rect_DataTitle[i].x = space_line;
		Rect_DataTitle[i].y = Rect_TopBar.y + Rect_TopBar.h + Rect_DataTitle[0].x + i * (Rect_DataTitle[i].h + space_line * 2 + TTF_FontHeight(Font_Deng));
		SDL_FreeSurface(Surf_DataTitle[i]);

		Surf_Data[i] = TTF_RenderUTF8_Blended(Font_Deng, Text_Data[i], black);
		Ture_Data[i] = SDL_CreateTextureFromSurface(Rdr_Main, Surf_Data[i]);
		Rect_Data[i].w = Surf_Data[i]->w;
		Rect_Data[i].h = Surf_Data[i]->h;
		Rect_Data[i].x = Rect_DataTitle[i].x + space_line;
		Rect_Data[i].y = Rect_DataTitle[i].y + Rect_DataTitle[i].h + space_line;
		SDL_FreeSurface(Surf_Data[i]);
	}

	int line = space_line * 3;
	for (int i = 0; i < 4; i++)
	{
		Rect_Button[i].w = (winW - Rect_LeftBar.x - Rect_LeftBar.w) / 2;
		Rect_Button[i].y = (winH - Rect_TopBar.x - Rect_TopBar.h - line) * i / 4 + Rect_TopBar.x + Rect_TopBar.h + line;
		Rect_Button[i].x = Rect_LeftBar.x + Rect_LeftBar.w + Rect_Button[i].w / 2;
		Rect_Button[i].h = (winH - Rect_TopBar.x - Rect_TopBar.h - line * 5) / 4;
		delete(Button[i]);
		Button[i] = new SDL_Button(Rdr_Main, Font_Sun, &Rect_Button[i], Text_Button[i], space_line * 3, 0x88888888, 0xEEEEEEAA, 0x333333CC);
	}
	switch (Login_State)
	{
	case admin:
		Button[0]->lock();
		Button[1]->lock();
		Button[2]->lock();
		break;
	case leader:
		Button[0]->lock();
		Button[3]->lock();
		break;
	case teacher:
		Button[1]->lock();
		Button[2]->lock();
		Button[3]->lock();
		break;
	}

	Point_TButton.x = (Rect_LeftBar.w - Rect_LeftBar.x - space_line * 2 * strlenUTF8(Text_TButton)) / 2;
	Point_TButton.y = Rect_LeftBar.y + Rect_LeftBar.h - space_line * 4;
	delete(TButton);
	TButton = new SDL_TextButton(Rdr_Main, Font_Sun, space_line * 2, Point_TButton, Text_TButton, 0x0000FFFF, 0x0000FFFF);

Again:
	fps++;

	SDL_RenderCopy(Rdr_Main, Ture_Img_BG2, &srcRect_BG2, NULL);
	SDL_SetRenderDrawColor(Rdr_Main, 0xFFFFFF88);
	SDL_RenderFillRect(Rdr_Main, NULL);
	SDL_SetRenderDrawColor(Rdr_Main, 0xFFFFFFD8);
	SDL_RenderFillRect(Rdr_Main, &Rect_TopBar);
	SDL_SetRenderDrawColor(Rdr_Main, 0x88A4C8CC);
	SDL_RenderFillRect(Rdr_Main, &Rect_LeftBar);

	SDL_RenderCopy(Rdr_Main, Ture_Img_Icon, NULL, &Rect_Icon);

	for (int i = 0; i < 4; i++) {
		SDL_RenderCopy(Rdr_Main, Ture_DataTitle[i], NULL, &Rect_DataTitle[i]);
		SDL_RenderCopy(Rdr_Main, Ture_Data[i], NULL, &Rect_Data[i]);
	}

	for (int i = 0; i < 4; i++)
		if ((Login_State == teacher && Button[i]->SDL_GetButtonState() != SDL_ButtonState_DISABLE) || Login_State != teacher)
			Button[i]->SDL_RenderDrawButton();

	TButton->SDL_RenderDrawTextButton();

	SDL_RenderDrawBuffer(Rdr_Main, buffer, NULL, NULL);

	SDL_RenderPresent(Rdr_Main);

	while (true)
	{
		if (SDL_PollEvent(&Event))
		{
			for (int i = 0; i < 4; i++)
				Button[i]->SDL_UpdataButtonState(&Event);
			TButton->SDL_UpdataTextButtonState(&Event);

			switch (Event.type)
			{
			case SDL_QUIT:
				Quit();
				break;
			case SDL_WINDOWEVENT:
				if (Event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					winW = Event.window.data1;
					winH = Event.window.data2;
					goto Init;
				}
				break;
			}
		}

		ULONGLONG Time_Now = GetTickCount64();
		if (Time_Now - Time_Cycle >= FPS_TICK)
		{
			Time_Cycle += FPS_TICK;
			printf("FPS: %d\n", fps);
			fps = 0;
		}
		if (1000 * fps / FPS > Time_Now - Time_Cycle)
			Sleep(1);
		else
			break;
	}

	goto Again;
}