#include "Gui.h"
#include "../Console/utils.h" 

void Gui::Clear()
{
	Console::Clear(_x, _y, _width, _height);
}

void Gui::Show()
{
	_show = true;
}

void Gui::Hide()
{
	_show = false;
}

int Gui::Width() const
{
	return _width;
}

int Gui::Height() const
{
	return _height;
}

bool Gui::IsShowing() const
{
	return _show;
}

bool Gui::IsHovered() const
{
	return _hover;
}

bool Gui::IsClicked() const
{
	return _click;
}

void Gui::HighLight()
{
	_hover = true;
}

void Gui::UnHighLight()
{
	_hover = false;
}
