#include "GuiMainMenu.h"

#include <sstream>
#include <thread>

#include "../Console/Console.h"
#include "../Console/Config.h"
#include "../Console/utils.h"

using namespace std;

GuiMainMenu* GuiMainMenu::_instance = nullptr;
GuiMainMenu* GuiMainMenu::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiMainMenu();
	return _instance;
}

GuiMainMenu::GuiMainMenu(int x, int y, int width, int height) {
	using namespace utils;

	_show = true;
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_game = GuiGameRoad::GetInstance();

	TextWithBorderOptions options{18};
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 0 + 1, textWithBorder(options, "New Game")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 1 + 1, textWithBorder(options, "Continue")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 2 + 1, textWithBorder(options, "Load Game")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 3 + 1, textWithBorder(options, "Setting")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 4 + 1, textWithBorder(options, "Exit")));
}

GuiMainMenu::~GuiMainMenu() {
	delete _instance;
}

void GuiMainMenu::NewGame()
{
	_game->Reset();
	StartGame();
}

void GuiMainMenu::StartGame()
{
	PopUp::Alert(65, 15, "NOW LOADING...");
	Sleep(200);

	_game->Show();
	_game->Continue();
	while (_game->IsShowing())
	{
		Console::WaitInput(_game);
		_game->Update();
		_game->Draw();

		GuiSidePanel* panel = panel->GetInstance();
		panel->Draw();

		Sleep(Config::Get(Config::FRAME_LENGTH));
		Console::Draw();
	}
}

void GuiMainMenu::Draw()
{
	if (_show)
	{
		utils::TextWithBorderOptions options{ _width, _height };
		Console::Write(_x - 1, _y - 1, utils::textWithBorder(options, ""), utils::Color::Green);

		for (auto i = 0; i < _buttons.size(); ++i)
		{
			if (i == _hoveredButton)
			{
				_buttons[i].HighLight();
				_buttons[i].Draw();
			}
			else
			{
				_buttons[i].UnHighLight();
				_buttons[i].Draw();
			}
		}
	}
}

void GuiMainMenu::Show()
{
	for (auto i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i].Show();
	}
	_show = true;
	Draw();
}

void GuiMainMenu::Hide()
{
	for (auto i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i].Hide();
	}
	Console::Clear(_x - 1, _y - 1, _width + 2, _height + 2);
	_show = false;
}

bool GuiMainMenu::OnMouseMove(short x, short y)
{
	if (_show
		&& _y <= y && y < _y + _height
		&& _x <= x && x < _x + _width)
	{
		for (auto i = 0; i < _buttons.size(); ++i)
		{
			if (_buttons[i].OnMouseMove(x, y))
			{
				_hoveredButton = i;
				return true;
			}	
		}
	}
	return false;
}

bool GuiMainMenu::OnKeyPressed(unsigned short key) {
	if (IsShowing())
	{
		WORD parsedKey = Config::ParseKey(key);

		switch (parsedKey) 
		{
		case Config::Key::KEY_UP:
		case Config::Key::KEY_LEFT:
			if (_hoveredButton == 0)
				_hoveredButton = _buttons.size() - 1;
			else
				_hoveredButton--;
			return true;

		case Config::Key::KEY_DOWN:
		case Config::Key::KEY_RIGHT:
			if (_hoveredButton == _buttons.size() - 1)
				_hoveredButton = 0;
			else
				_hoveredButton++;
			return true;

		case Config::Key::KEY_ENTER:
			OnButtonClick(_hoveredButton);
			return true;
		}
	}
	return _game->OnKeyPressed(key);
}

bool GuiMainMenu::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	if (this->OnMouseMove(x, y))
	{
		this->OnKeyPressed(Config::Get(Config::KEY_ENTER));
	}
	return false;
}

void GuiMainMenu::OnButtonClick(int btnId) {
	this->Hide();
	switch (btnId)
	{
	case 0: // New Game
		NewGame();
		break;
	case 1: // Continue
		StartGame();
		break;

	case 2: // Load Game
		_game->Load();
		StartGame();
		break;

	case 3: // Setting
		_game->Setting();
		break;

	case 4: // Exit
		this->Hide();
		return;
	}
	this->Show();
}