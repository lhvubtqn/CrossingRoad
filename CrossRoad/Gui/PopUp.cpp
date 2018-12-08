#include "PopUp.h"

#include "../Console/utils.h"
#include "../Console/Console.h"
#include "../Console/Config.h"

#include <sstream>
#include <iostream>

#define POPUP_WIDTH 48
#define POPUP_HEIGHT 12

void PopUp::Alert(short x, short y, std::string msg)
{
	PopUp p(x, y, msg, PopUpType::ALERT);
	while (!p._done) {
		Console::WaitInput(&p);
		p.Draw();
		Console::Draw();
	}
}

bool PopUp::Question(short x, short y, std::string msg)
{
	PopUp p(x, y, msg, PopUpType::QUESTION);
	while (!p._done) {
		Console::WaitInput(&p);
		p.Draw();
		Console::Draw();
	}
	return *((bool*)p._result);
	return false;
}

bool PopUp::Confirm(short x, short y, std::string msg) {
	PopUp p(x, y, msg, PopUpType::CONFIRM);
	while (!p._done) {
		Console::WaitInput(&p);
		p.Draw();
		Console::Draw();
	}
	return *((bool*) p._result);
}

std::string PopUp::Prompt(short x, short y, std::string msg) {
	PopUp p(x, y, msg, PopUpType::PROMPT);
	while (!p._done) {
		Console::WaitInput(&p);
		p.Draw();
		Console::Draw();
	}
	if (p._result)
		return *((std::string*) p._result);
	else
		return "";
}

PopUp::PopUp(short x, short y, std::string msg, PopUpType type) {
	using namespace utils;

	_type = type;
	_done = false;
	_result = nullptr;

	auto size = Console::GetSize();
	_width = POPUP_WIDTH;
	_height = POPUP_HEIGHT;
	_x = x;
	_y = y;
	
	size_t i = _width - 1;
	while (i < msg.length()) {
		msg.insert(i, 1, '\n');
		i += _width - 1;
	}
	_msg = msg;

	if (type == PopUp::ALERT)
	{
		_buttons.push_back(GuiButton(_x + _width - 8, _y + _height - 2, alignText(Center, "Ok", 4)));
	}
	else if (type == PopUp::QUESTION)
	{
		_buttons.push_back(GuiButton(_x + _width - 18, _y + _height - 2, alignText(Center, "Yes", 6)));
		_buttons.push_back(GuiButton(_x + _width - 8, _y + _height - 2, alignText(Center, "No", 6)));
	}
	else
	{
		_buttons.push_back(GuiButton(_x + _width - 18, _y + _height - 2, alignText(Center, "Ok", 4)));
		_buttons.push_back(GuiButton(_x + _width - 10, _y + _height - 2, alignText(Center, "Cancel", 8)));
	}

	if (type == PopUpType::PROMPT) {
		_result = new std::string;
		Console::GotoXY(_x + 2, _y + _height - 4);
	}
}

PopUp::~PopUp() {
	delete _result;
}

void PopUp::drawBorder(int x, int y, int width, int height, int color) {	
	int i;
	auto chars = utils::getChars(utils::DOUBLE);

	Console::Write(x, y, chars.topLeft, color);
	Console::Write(x + width - 1, y, chars.topRight, color);
	Console::Write(x, y + height - 1, chars.bottomLeft, color);
	Console::Write(x + width - 1, y + height - 1, chars.bottomRight, color);

	for (i = x + 1; i < x + width - 1; i++) {
		Console::Write(i, y, chars.horizontal, color);
		Console::Write(i, y + height - 1, chars.horizontal, color);
	}

	for (i = y + 1; i < y + height - 1; i++) {
		Console::Write(x, i, chars.vertical, color);
		Console::Write(x + width - 1, i, chars.vertical, color);
	}
}

void PopUp::Draw() {

	using namespace utils;

	Clear();

	if (!_done)
	{
		auto size = Console::GetSize();
		int color = Color::LightGreen;

		Console::Write(_x + 1, _y + 1, _msg, Color::White);
		drawBorder(_x, _y, _width, _height, color);

		if (_type == PopUpType::PROMPT) {
			auto chars = getChars(SINGLE);
			Console::Write(_x + 1, _y + _height - 5, chars.topLeft, color);
			Console::Write(_x + 1, _y + _height - 4, chars.vertical, color);
			Console::Write(_x + 1, _y + _height - 3, chars.bottomLeft, color);
			Console::Write(_x + _width - 2, _y + _height - 5, chars.topRight, color);
			Console::Write(_x + _width - 2, _y + _height - 4, chars.vertical, color);
			Console::Write(_x + _width - 2, _y + _height - 3, chars.bottomRight, color);
			for (int i = _x + 2; i <= _x + _width - 3; i++) {
				Console::Write(i, _y + _height - 5, chars.horizontal, color);
				Console::Write(i, _y + _height - 3, chars.horizontal, color);
			}
		}

		for (int i = 0; i < _buttons.size(); ++i)
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

		if (_type == PopUp::ALERT)
		{
			Console::Write(_x + _width - 4, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 9, _y + _height - 2, '|', color);
		} 
		else if (_type == PopUp::QUESTION)
		{
			Console::Write(_x + _width - 3, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 8, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 12, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 18, _y + _height - 2, '|', color);
		}
		else
		{
			Console::Write(_x + _width - 2, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 11, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 14, _y + _height - 2, '|', color);
			Console::Write(_x + _width - 19, _y + _height - 2, '|', color);
		}

		if (_type == PopUpType::PROMPT) {
			std::string temp = *((std::string*) _result);
			Console::Write(_x + 2, _y + _height - 4, temp, Color::Yellow);
			if (!_donePrompt)
				Console::GotoXY(_x + 2 + temp.length(), _y + _height - 4);
		}
	}
}

bool PopUp::OnMouseMove(short x, short y)
{
	for (auto i = 0; i < _buttons.size(); ++i)
	{
		if (_buttons[i].OnMouseMove(x, y))
		{
			_hoveredButton = i;
			return true;
		}
	}
	return false;
}

bool PopUp::OnKeyPressed(unsigned short key) {
	if (_type == PopUp::PROMPT && _donePrompt == false)
	{
		size_t len = ((std::string*) _result)->length();
		if (key == VK_BACK) {
			if (len > 0)
				((std::string*) _result)->pop_back();
			return true;
		}

		std::string temp = key == VK_SPACE ? " " : utils::vk2Str(key);
		if (temp.length() == 1 && len < POPUP_WIDTH - 10) {
			*((std::string*) _result) += temp[0];
			return true;
		}
	}

	WORD parsedKey = Config::ParseKey(key);
	switch (parsedKey) {
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
		_donePrompt = true;	
		_hoveredButton = 1;
		OnButtonClick(_hoveredButton);
		return true;
	}
	return true;
}

bool PopUp::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	if (OnMouseMove(x, y))
	{
		OnButtonClick(_hoveredButton);
		return true;
	}
	return false;
}

void PopUp::OnButtonClick(int btnId) {
	_done = true;
	if (_type != PopUpType::PROMPT)
		_result = new bool(!btnId);
	else if (btnId == 1) {
		delete _result;
		_result = nullptr;
	}
}