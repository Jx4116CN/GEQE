//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma execution_character_set("utf-8")
#pragma once

#include <HPSocket/HPSocket4C.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "../../¿â/SDLplus.h"
#include <thread>
#include <mutex>

using namespace std;

#define MB_RETRY SDL_MB_RETRY

#define FPS 25
#define FPS_TICK 1000

#define max_len_telephone 11
#define max_len_password 16

#define max_loading_tick 3000

__declspec(selectany) HP_TcpClientListener Listener = Create_HP_TcpClientListener();
__declspec(selectany) HP_TcpClient Client = Create_HP_TcpClient(Listener);
__declspec(selectany) bool HP_TcpConnect = false;
__declspec(selectany) bool LoginState = false;
__declspec(selectany) int ConnNUM;

static const unsigned int screenW = GetSystemMetrics(SM_CXSCREEN);
static const unsigned int screenH = GetSystemMetrics(SM_CYSCREEN);
__declspec(selectany) int winW = screenW * 2 / 7;
__declspec(selectany) int winH = screenH * 2 / 3;

__declspec(selectany) SDL_Window* Win_Login = NULL;
__declspec(selectany) SDL_Window* Win_Main = NULL;
__declspec(selectany) std::thread* Thread_Win_Main;
__declspec(selectany) SDL_Renderer* Rdr_Login = NULL;
__declspec(selectany) SDL_Renderer* Rdr_Main = NULL;

__declspec(selectany) SDL_Surface* Surf_Text_Loading = NULL;
__declspec(selectany) SDL_Texture* Ture_Text_Loading = NULL;
__declspec(selectany) const char* Path_Image_BG = "Files\\Images\\BG.jpg";
__declspec(selectany) SDL_Surface* Surf_Img_BG = NULL;
__declspec(selectany) SDL_Texture* Ture_Img_BG = NULL;
__declspec(selectany) const char* Path_Image_Icon = "Files\\Images\\Icon.png";
__declspec(selectany) SDL_Surface* Surf_Img_Icon = NULL;
__declspec(selectany) SDL_Texture* Ture_Img_Icon = NULL;
__declspec(selectany) const char* Path_Image_BG2 = "Files\\Images\\BG2.jpg";
__declspec(selectany) SDL_Surface* Surf_Img_BG2 = NULL;
__declspec(selectany) SDL_Texture* Ture_Img_BG2 = NULL;

__declspec(selectany) const char* Path_Font_Hei = "Files\\Fonts\\simhei.ttf";
__declspec(selectany) TTF_Font* Font_Hei = NULL;
__declspec(selectany) const char* Path_Font_Deng = "Files\\Fonts\\Deng.ttf";
__declspec(selectany) TTF_Font* Font_Deng = NULL;
__declspec(selectany) const char* Path_Font_Sun = "Files\\Fonts\\simsun.ttc";
__declspec(selectany) TTF_Font* Font_Sun = NULL;

__declspec(selectany) const char* Path_autofill = "Appdata\\autofill.txt";
__declspec(selectany) std::string autofill_telephone;
__declspec(selectany) std::string autofill_password;
__declspec(selectany) bool autofill = false;
void Init_Login();
void Quit_Login();
struct User
{
	std::string permission;
	std::string telephone;
	std::string password;
	std::string name;
	std::string year_up;
	std::string month_up;
	std::string day_up;
	std::string hour_up;
	std::string minute_up;
	std::string second_up;
};
__declspec(selectany) User user;
void Init_Main();
void Quit_Main();

enum LOGINSTATE { NONE, FAILED, admin, leader, teacher };
__declspec(selectany) LOGINSTATE Login_State = NONE;
void Login();

void MainWin();

void Quit();