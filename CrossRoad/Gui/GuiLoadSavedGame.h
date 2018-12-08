#ifndef GUIVLOADSAVEDGAME_H
#define GUIVLOADSAVEDGAME_H

#include "GuiScreen.h"
#include "GuiManager.h"

class GuiLoadSavedGame : public GuiScreen {
private:
	static Gui * _instance;
	GuiLoadSavedGame();
public:
	static Gui* GetInstance();
	~GuiLoadSavedGame();
public:
	void Draw();
	void OnButtonClick(int btnId);
	void OnButtonHover(int btnId);
};

#endif // !GUIVLOADSAVEDGAME_H
