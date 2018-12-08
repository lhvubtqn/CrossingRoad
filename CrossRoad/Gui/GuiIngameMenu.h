#ifndef GUI_INGAME_MENU_H
#define GUI_INGAME_MENU_H

#include "GuiButton.h"

#include <vector>
#include <functional>

class GuiIngameMenu : public Gui {
public:
	enum INGAME_OPTION
	{
		CONTINUE, SETTING, LOAD, SAVE, EXIT
	};

private:
	std::vector<GuiButton> _buttons;
	int _hoveredButton = 0;

private:
	static GuiIngameMenu * _instance;
	GuiIngameMenu(int x = 80, int y = 10, int width = 28, int height = 21);

public:
	static GuiIngameMenu * GetInstance();
	~GuiIngameMenu();

public:
	INGAME_OPTION GetOption();

public:
	void Draw();
	void Show();
	void Hide();

	bool OnMouseMove(short x, short y);
	bool OnKeyPressed(unsigned short key);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);

	void OnButtonClick(int btnId);
};

#endif // !GUI_INGAME_MENU_H