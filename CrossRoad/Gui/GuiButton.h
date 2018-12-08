#pragma once
#include "Gui.h"
#include <string>

class GuiButton :
	public Gui
{
private:
	std::string _label;

public:
	GuiButton(int x = 0, int y = 0, std::string label = "");

public:
	void Draw();

	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);

};

