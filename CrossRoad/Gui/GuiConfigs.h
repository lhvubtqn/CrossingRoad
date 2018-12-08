#ifndef GUICONFIGS_H
#define GUICONFIGS_H

#include "GuiScreen.h"
#include <string>

class GuiConfigs : public GuiScreen {
private:	
	static Gui* _instance;
public:
	static Gui* GetInstance();	
	~GuiConfigs();
private:
	bool _expectKeypress;
	int _cursorX = 0;
	int _cursorY = 1;
	GuiConfigs();
public:
	void Draw();
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	void OnButtonClick(int btnId) {};
	void OnButtonHover(int btnId) {};
};

#endif // !GUICONFIGS_H
