#include "Client_Init.h"

#undef main

int main()
{
	Init_Login();
	Login();
	Quit_Login();

	Init_Main();
	MainWin();
	Quit_Main();

	Quit();

	return 0;
}