#ifndef GUISCREEN_H
#define GUISCREEN_H

#include "Gui.h"

class GuiScreen: public Gui {
private:	
	int _selectedButton = 0;
	std::vector<GuiButton> _buttons;
public:
	int AddButton(short x, short y, std::string label);
	void RemoveButton(int btnId);
	GuiButton& GetButton(int btnId) { return _buttons[btnId]; }
	void Clear();
public:
	void Draw();
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn); 
	void OnButtonHover(int btnId);
public:
	~GuiScreen() {};	
	virtual void OnButtonClick(int btnId) = 0;
};

#endif // !GUISCREEN_H