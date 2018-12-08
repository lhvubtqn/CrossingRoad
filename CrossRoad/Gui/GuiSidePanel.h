#ifndef GUISIDEPANEL_H
#define GUISIDEPANEL_H

#include "Gui.h"

#define HISTORY_SIZE 5

class GuiSidePanel : public Gui {
private:
	static GuiSidePanel* _instance;
	GuiSidePanel();

public:
	static GuiSidePanel * GetInstance();
	~GuiSidePanel();

private:
	std::string _info;

public:
	void SetInfo(std::string info);
	void WriteHeader(short y, std::string header, short attr = 15);
	void WriteInfo(short x, short y, std::string text, int attr);
	void ClearInfo();
	void WriteInstruct(short x, short y, std::string text, int attr);
	void ClearInstruct();

public:
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	void OnButtonClick(int btnId) {};
	void OnButtonHover(int btnId) {};

	void Draw();
	
};

#endif // !GUISIDEPANEL_H