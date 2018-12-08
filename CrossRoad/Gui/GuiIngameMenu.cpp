#include "GuiIngameMenu.h"

#include <sstream>
#include <thread>

#include "../Console/Console.h"
#include "../Console/Config.h"
#include "../Console/utils.h"

using namespace std;

GuiIngameMenu* GuiIngameMenu::_instance = nullptr;
GuiIngameMenu* GuiIngameMenu::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiIngameMenu();
	return _instance;
}

GuiIngameMenu::GuiIngameMenu(int x, int y, int width, int height) {
	using namespace utils;

	_show = true;
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	TextWithBorderOptions options{ 18 };
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 0 + 1, textWithBorder(options, "Continue")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 1 + 1, textWithBorder(options, "Setting")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 2 + 1, textWithBorder(options, "Load")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 3 + 1, textWithBorder(options, "Save")));
	_buttons.push_back(GuiButton(_x + (_width - 19) / 2, _y + 4 * 4 + 1, textWithBorder(options, "Exit")));
}

GuiIngameMenu::~GuiIngameMenu() {
	delete _instance;
}

GuiIngameMenu::INGAME_OPTION GuiIngameMenu::GetOption()
{
	this->Show();
	while (this->IsShowing())
	{
		Console::WaitInput(this);
		this->Draw();
		Console::Draw();
		Sleep(Config::Get(Config::FRAME_LENGTH));
	}
	return INGAME_OPTION(_hoveredButton);
}

void GuiIngameMenu::Draw()
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

void GuiIngameMenu::Show()
{
	for (auto i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i].Show();
	}
	_show = true;
}

void GuiIngameMenu::Hide()
{
	for (auto i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i].Hide();
	}
	Console::Clear(_x - 1, _y - 1, _width + 2, _height + 2);
	_show = false;
}

bool GuiIngameMenu::OnMouseMove(short x, short y)
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

bool GuiIngameMenu::OnKeyPressed(unsigned short key) {
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

		case Config::KEY_ESCAPSE:
			_hoveredButton = 0;
			OnButtonClick(_hoveredButton);
			return true;
		}
	}
	return false;
}

bool GuiIngameMenu::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	if (this->OnMouseMove(x, y))
	{
		return this->OnKeyPressed(Config::Get(Config::KEY_ENTER));
	}
	return false;
}

void GuiIngameMenu::OnButtonClick(int btnId) {
	this->Hide();
}