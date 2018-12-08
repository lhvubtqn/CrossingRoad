#ifndef POPUP_H
#define POPUP_H

#include "GuiButton.h"
#include <string>

class PopUp : public Gui {
public:
	static void Alert(short x, short y, std::string msg);
	static bool Question(short x, short y, std::string msg);
	static bool Confirm(short x, short y, std::string msg);
	static std::string Prompt(short x, short y, std::string msg);

private:
	enum PopUpType {
		ALERT, QUESTION, CONFIRM, PROMPT
	};

private:
	bool _done;
	bool _donePrompt = false;
	void* _result;
	PopUpType _type;
	std::string _msg;	
	std::vector<GuiButton> _buttons;
	int _hoveredButton = 0;

	PopUp(short x, short y, std::string msg, PopUpType type);
	~PopUp();

	void drawBorder(int top, int left, int width, int height, int color);

public:
	void Draw();
	bool OnMouseMove(short x, short y);
	bool OnKeyPressed(unsigned short key);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	void OnButtonClick(int btnId);
};

#endif // !POPUP_H
