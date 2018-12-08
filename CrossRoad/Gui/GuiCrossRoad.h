#ifndef GUI_CROSS_ROAD_H
#define GUI_CROSS_ROAD_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <regex>

#include "../Console/Console.h"
#include "../Objects/People.h"
#include "GuiGameRoad.h"
#include "Gui.h"

class GuiCrossRoad :
	public Gui
{	

private:
	static GuiCrossRoad* _instance;
	GuiCrossRoad();

public:
	static GuiCrossRoad* GetInstance();
	~GuiCrossRoad();

private:
	GuiGameRoad* _road;
	int _level = 1;
	bool _run;

public:
	void Start();
	void Exit();
	void Reset();
	void Pause();
	void Resume();
	void Load();
	void Save();
	void Update();

public:
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);

	void Draw();
	void Show();
	void Hide();

};

#endif // !GUI_CROSS_ROAD_H



