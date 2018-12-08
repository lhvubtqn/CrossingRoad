#include "GuiButton.h"
#include "../Console/Console.h"
#include "../Console/utils.h"
#include "../Console/Config.h"

GuiButton::GuiButton(int x, int y, std::string label)
{
	_x = x;
	_y = y;
	_width = _height = 0;
	_label = label;

	size_t pos;
	size_t last = 0;
	std::string s = label + '\n';
	while ((pos = s.find_first_of('\n', last)) != std::string::npos) {
		_height++;
		if (pos - last > _width)
			_width = pos - last;
		last = pos + 1;
	}
}

void GuiButton::Draw()
{
	Console::Write(
		_x, _y, _label, 
		_hover ? 
		utils::color(utils::Color::Black, utils::Color::Green) :
		utils::Color::Green);
	_show = true;
}

bool GuiButton::OnKeyPressed(unsigned short key)
{
	if (key == Config::KEY_ENTER)
	{

	}
	return false;
}

bool GuiButton::OnMouseMove(short x, short y)
{
	if (_show
		&& _y <= y && y < _y + _height
		&& _x <= x && x < _x + _width) 
	{
		_hover = true;
	}
	else
	{
		_hover = false;
	}
	return _hover;
}

bool GuiButton::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	if (OnMouseMove(x, y))
	{
		_click = true;
	}
	else
	{
		_click = false;
	}
	return _click;
}
