#include <iostream>
#include <thread>

#include "Console/Config.h"
#include "Console/Console.h"
#include "Console/utils.h"
#include "Gui/GuiMainMenu.h"
#include "Gui/GuiSidePanel.h"
#include "Gui/GuiGameRoad.h"
#include "Gui/PopUp.h"

#define CONSOLE_CONFIG "Config/config.ini"

using namespace std;
using namespace utils;

void ProgramStart()
{
	Config::Load(CONSOLE_CONFIG);
	Console::Init();

	GuiSidePanel* panel = GuiSidePanel::GetInstance();
	panel->Show();

	GuiMainMenu* menu = GuiMainMenu::GetInstance();
	menu->Show();

	mciSendString("open \"Sounds/background.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3 repeat", NULL, 0, NULL);

	while (menu->IsShowing())
	{
		Console::WaitInput(menu);
		menu->Draw();

		panel->SetInfo(
			alignText(Center, std::string("Truong: DH Khoa hoc Tu nhien\n"), panel->Width())
			+ alignText(Center, std::string("MSSV : 1712250              \n"), panel->Width())
			+ alignText(Center, std::string("Ho ten: Le Hoang Vu         \n"), panel->Width()));
		panel->Draw();
		Console::Draw();
		Sleep(Config::Get(Config::FRAME_LENGTH));
	}
}

int main()
{
	ProgramStart();
	return 0;
}