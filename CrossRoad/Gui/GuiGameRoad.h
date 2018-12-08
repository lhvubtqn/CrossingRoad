#ifndef _GUI_GAMEROAD_H
#define _GUI_GAMEROAD_H

#include "../Objects/Dinosaur.h"
#include "../Console/Console.h"
#include "../Objects/People.h"
#include "../Console/utils.h"
#include "../Objects/Truck.h"
#include "../Objects/Bird.h"
#include "../Objects/Car.h"
#include "../Objects/Bus.h"

#include "GuiIngameMenu.h"
#include "GuiSidePanel.h"
#include "PopUp.h"
#include "Gui.h"

class GuiGameRoad : public Gui
{
public:
	enum DIFFICULTY
	{
		EASY, NORMAL, HARD
	};

private:
	struct Lane
	{
		int X = 0, Y = 0;
		int Velocity = 1;
		std::vector<Object*> Objects;
	};

private:
	int _laneNum = 5;
	int _laneHeight = 5;
	int _laneWidth = 119;

	int _level = 1;
	int _minSpace = 20;
	DIFFICULTY _difficulty = DIFFICULTY::EASY;

	const int _factor[3][3] = {
		{1, 1, 3},
		{2, 2, 4},
		{3, 3, 6}
	};

	std::vector<Lane> _lanes;
	People* _people;

	int _pauseFrame = 10;
	bool _pause = false;
	bool _mute = false;

private:
	static GuiGameRoad* _instance;
	GuiGameRoad();

public:
	static GuiGameRoad* GetInstance();
	~GuiGameRoad();

private:
	void DrawSideRoad(int y, int attr);
	void DrawLaneBreak(int y, int attr);
	void hDrawTrafficLight(int attr);
	int hFindLane(Object* obj);
	int hRand(int left, int right);

	void hLoadPeople(std::istream& stream);
	void hSavePeople(std::ostream& stream);
	Object* hLoadObject(std::istream& stream);
	void hSaveObject(std::ostream& stream, Object* obj);

public:
	std::string DifficultyToString();

	void Reset();
	void Init();
	void Check();
	void AddObject();
	void WriteInfo();
	void Update();

	void Pause();
	void Continue();
	void SetDifficulty(DIFFICULTY dif);
	void Setting();
	void Load();
	void Save();

	void IngameMenu();
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	void Draw();

	Object* NewObject(int type, int attr = 15);
	void DeleteObjects();
};
#endif // !_GUI_GAMEROAD_H
