#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "GuiButton.h"
#include "GuiGameRoad.h"
#include "GuiSidePanel.h"

#include <vector>
#include <functional>

class GuiMainMenu : public Gui {
private:
	std::vector<GuiButton> _buttons;
	int _hoveredButton = 0;
	GuiGameRoad* _game = nullptr;

private:
	static GuiMainMenu * _instance;
	GuiMainMenu(int x = 80, int y = 10, int width = 28, int height = 21);

public:
	static GuiMainMenu * GetInstance();
	~GuiMainMenu();

public:
	void NewGame();
	void StartGame();

public:
	void Draw();
	void Show();
	void Hide();

	bool OnMouseMove(short x, short y);
	bool OnKeyPressed(unsigned short key);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);

	void OnButtonClick(int btnId);
};

#endif // !GUIMAINMENU_H