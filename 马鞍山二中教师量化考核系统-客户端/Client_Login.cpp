#include "Client_Init.h"

inline void save(std::string telephone, std::string password)
{
	ofstream file(Path_autofill);
	if (file.fail())
	{
		char str[MAX_TIP_LEN] = { 0 };
		sprintf(str, "Failed to open the file - %s\nError code:%d", Path_autofill, GetLastError());
		MessageBoxA(NULL, str, "Error", MB_OK);
		return;
	}
	if (autofill) file << telephone << "\n" << password;
	file.close();
}

void Login()
{
	SDL_Event Event;

Init:
	ULONGLONG Time_Start = GetTickCount64();
	ULONGLONG Time_Cycle = Time_Start;
	ULONGLONG Time_Last;
	int fps = 0;

	int space_line = winW / 30;

	SDL_Rect Rect_BG;
	Rect_BG.h = Surf_Img_BG->h;
	Rect_BG.w = winW * Rect_BG.h / winH;
	Rect_BG.x = (Surf_Img_BG->w - Rect_BG.w) / 2;
	Rect_BG.y = 0;

	SDL_Rect Rect_Icon;
	Rect_Icon.x = space_line;
	Rect_Icon.y = Rect_Icon.x;
	Rect_Icon.h = winH / 10;
	Rect_Icon.w = Rect_Icon.h * Surf_Img_Icon->w / Surf_Img_Icon->h;

	SDL_Rect Rect_TitleBar;
	Rect_TitleBar.x = 0;
	Rect_TitleBar.y = 0;
	Rect_TitleBar.w = winW;
	Rect_TitleBar.h = Rect_Icon.h + space_line * 2;

	SDL_Rect Rect_TitleBar_BG;
	Rect_TitleBar_BG.x = Rect_BG.x;
	Rect_TitleBar_BG.y = Rect_BG.y;
	Rect_TitleBar_BG.w = Rect_BG.w;
	Rect_TitleBar_BG.h = Rect_TitleBar.h * Rect_BG.h / winH;

	TTF_SetFontSize(Font_Hei, space_line * 2);
	SDL_Surface* Surf_Text = TTF_RenderUTF8_Blended(Font_Hei, "ÇëµÇÂ¼ÕË»§", { 0,0,0,0xDD });
	SDL_Texture* Ture_Text = SDL_CreateTextureFromSurface(Rdr_Login, Surf_Text);
	SDL_Rect Rect_Text;
	Rect_Text.x = space_line * 5;
	Rect_Text.y = Rect_TitleBar.x + Rect_TitleBar.h + space_line * 5;
	Rect_Text.h = Surf_Text->h;
	Rect_Text.w = Surf_Text->w;
	SDL_FreeSurface(Surf_Text);

	TTF_SetFontSize(Font_Hei, space_line * 2);
	SDL_Surface* Surf_Text1 = TTF_RenderUTF8_Blended(Font_Hei, "ÊÖ»úºÅ£º", { 0,0,0,0xDD });
	SDL_Texture* Ture_Text1 = SDL_CreateTextureFromSurface(Rdr_Login, Surf_Text1);
	SDL_Rect Rect_Text1;
	Rect_Text1.x = Rect_Text.x;
	Rect_Text1.y = Rect_Text.y + Rect_Text.h + space_line * 2;
	Rect_Text1.h = space_line * 2;
	Rect_Text1.w = Surf_Text1->w;
	SDL_FreeSurface(Surf_Text1);

	SDL_Rect Rect_Form1;
	Rect_Form1.x = Rect_Text1.x + Rect_Text1.w;
	Rect_Form1.y = Rect_Text1.y;
	Rect_Form1.w = winW - Rect_Text1.x * 2 - Rect_Text1.w;
	Rect_Form1.h = Rect_Text1.h;
	SDL_Form Form1(Rdr_Login, Font_Deng, "ÇëÊäÈëÊÖ»úºÅ", &Rect_Form1, max_len_telephone, autofill_telephone);
	Form1.setType(SDL_FORM_TELEPHONE);

	TTF_SetFontSize(Font_Hei, space_line * 2);
	SDL_Surface* Surf_Text2 = TTF_RenderUTF8_Blended(Font_Hei, "ÃÜ   Âë£º", { 0,0,0,0xDD });
	SDL_Texture* Ture_Text2 = SDL_CreateTextureFromSurface(Rdr_Login, Surf_Text2);
	SDL_Rect Rect_Text2;
	Rect_Text2.x = Rect_Text1.x;
	Rect_Text2.y = Rect_Text1.y + Rect_Text1.h + space_line;
	Rect_Text2.h = space_line * 2;
	Rect_Text2.w = Rect_Text1.w;
	SDL_FreeSurface(Surf_Text2);

	SDL_Rect Rect_Form2;
	Rect_Form2.x = Rect_Text2.x + Rect_Text2.w;
	Rect_Form2.y = Rect_Text2.y;
	Rect_Form2.w = Rect_Form1.w;
	Rect_Form2.h = Rect_Text2.h;
	SDL_Form Form2(Rdr_Login, Font_Deng, "ÇëÊäÈëÃÜÂë", &Rect_Form2, max_len_password, autofill_password);
	Form2.setType(SDL_FORM_PASSWORD);

	SDL_Rect Rect_Button_SginIn;
	Rect_Button_SginIn.x = Rect_Text.x;
	Rect_Button_SginIn.y = Rect_Text2.y + Rect_Text2.h + space_line * 2;
	Rect_Button_SginIn.w = Rect_Text1.w + Form1.rect->w;
	Rect_Button_SginIn.h = space_line * 2;
	SDL_Button Button_SignIn(Rdr_Login, Font_Deng, &Rect_Button_SginIn, "µÇ  Â¼");

	bool login_fail = false;
	TTF_SetFontSize(Font_Deng, space_line);
	SDL_Surface* Surf_FailTip = TTF_RenderUTF8_Blended(Font_Deng, "*µÇÂ¼Ê§°Ü£ºÊÖ»úºÅ»òÃÜÂë´íÎó£¡", 0xFF0000FF);
	SDL_Texture* Ture_FailTip = SDL_CreateTextureFromSurface(Rdr_Login, Surf_FailTip);
	SDL_Rect Rect_FailTip;
	Rect_FailTip.x = Rect_Text2.x;
	Rect_FailTip.y = Rect_Form2.y + Rect_Form2.h + space_line / 2;
	Rect_FailTip.h = Surf_FailTip->h;
	Rect_FailTip.w = Surf_FailTip->w;
	SDL_FreeSurface(Surf_FailTip);

	SDL_Rect Rect_CKButton;
	Rect_CKButton.x = Rect_Button_SginIn.x;
	Rect_CKButton.y = Rect_Button_SginIn.y + Rect_Button_SginIn.h + space_line / 2;
	Rect_CKButton.h = space_line;
	Rect_CKButton.w = Rect_CKButton.h;
	SDL_CheckButton CKButton_Save(Rdr_Login, Font_Sun, &Rect_CKButton);
	CKButton_Save.SetActive(autofill);

	TTF_SetFontSize(Font_Sun, Rect_CKButton.h);
	SDL_Surface* Surf_CKText = TTF_RenderUTF8_Blended(Font_Sun, "±£´æÐÅÏ¢£¨ÏÂ´ÎµÇÂ¼×Ô¶¯Ìî³ä£©", 0x000000FF);
	SDL_Texture* Ture_CKText = SDL_CreateTextureFromSurface(Rdr_Login, Surf_CKText);
	SDL_Rect Rect_CKText;
	Rect_CKText.x = Rect_CKButton.x + Rect_CKButton.w + space_line / 2;
	Rect_CKText.y = Rect_CKButton.y;
	Rect_CKText.w = Surf_CKText->w;
	Rect_CKText.h = Rect_CKButton.h;
	SDL_FreeSurface(Surf_CKText);

Again:
	fps++;

	if (Form1.text.size() < 1 || Form2.text.size() < 6)
		Button_SignIn.lock();
	else
		Button_SignIn.unlock();

	SDL_RenderCopy(Rdr_Login, Ture_Img_BG, &Rect_BG, NULL);

	SDL_RenderCopy(Rdr_Login, Ture_Img_Icon, NULL, &Rect_Icon);

	SDL_RenderCopy(Rdr_Login, Ture_Text, NULL, &Rect_Text);
	SDL_RenderCopy(Rdr_Login, Ture_Text1, NULL, &Rect_Text1);
	Form1.SDL_RenderDrawForm();
	SDL_RenderCopy(Rdr_Login, Ture_Text2, NULL, &Rect_Text2);
	Form2.SDL_RenderDrawForm();

	Button_SignIn.SDL_RenderDrawButton();

	if (login_fail) SDL_RenderCopy(Rdr_Login, Ture_FailTip, NULL, &Rect_FailTip);

	CKButton_Save.SDL_RenderDrawCheckButton();
	SDL_RenderCopy(Rdr_Login, Ture_CKText, NULL, &Rect_CKText);

	SDL_RenderPresent(Rdr_Login);

	while (true)
	{
		if (SDL_PollEvent(&Event))
		{
			Form1.SDL_UpdateForm(&Event);
			Form2.SDL_UpdateForm(&Event);
			Button_SignIn.SDL_UpdataButtonState(&Event);
			CKButton_Save.SDL_UpdataCheckButtonState(&Event);

			switch (Event.type)
			{
			case SDL_QUIT:
				Quit();
				break;
			}

			if (Button_SignIn.SDL_GetButtonState() == SDL_ButtonState_LOADING)
			{
				autofill = CKButton_Save.GetActive();
				save(Form1.text, Form2.text);
				string str1 = "<SignIn>";
				char str2[8 + max_len_telephone + 1 + max_len_password] = { 0 };
				strcat(str2, str1.c_str());
				strcat(str2, (Form1.text + "\n").c_str());
				strcat(str2, Form2.text.c_str());
				HP_Client_Send(Client, (const BYTE*)str2, sizeof(str2));
				Sleep(10);
				string str3 = "</SignIn>";
				HP_Client_Send(Client, (const BYTE*)str3.data(), str3.size());

				ULONGLONG time1 = GetTickCount64();
				while (time1 + max_loading_tick > GetTickCount64())
				{
					Sleep(10);
					if (Login_State != NONE)
					{
						if (Login_State == FAILED)
						{
							login_fail = true;
							Login_State = NONE;
						}
						else
						{
							user.telephone = Form1.text;
							user.password = Form2.text;
							SDL_DestroyTexture(Ture_Text);
							SDL_DestroyTexture(Ture_Text1);
							SDL_DestroyTexture(Ture_Text2);
							SDL_DestroyTexture(Ture_FailTip);
							SDL_DestroyTexture(Ture_CKText);
							return;
						}
						Button_SignIn.SDL_ButtonLoadCompleted();
						goto Again;
					}
				}
				char str[MAX_TIP_LEN] = { 0 };
				sprintf(str, "*Connection timed out!\n%s\nError code:%d", HP_Client_GetLastErrorDesc(Client), GetLastError());
				MessageBoxA(NULL, str, "Error", MB_ICONERROR | MB_OK);
				Button_SignIn.SDL_ButtonLoadCompleted();
				goto Again;
			}
		}
		else
		{
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
	}

	goto Again;
}