#include "GuiCrossRoad.h"

#include "../Console/utils.h"
#include "GuiSidePanel.h"

GuiCrossRoad* GuiCrossRoad::_instance = nullptr;

GuiCrossRoad::GuiCrossRoad()
{
	srand(time(NULL));
	_x = 34;
	_y = 0;
	_width = 119;
	_height = 41;
	_show = true;

	_level = 1;
	_run = false;
	_road = new GuiGameRoad(20 - _level, _level, _level * 5);
}

GuiCrossRoad * GuiCrossRoad::GetInstance()
{
	if (!_instance)
	{
		_instance = new GuiCrossRoad();
	}
	return _instance;
}

GuiCrossRoad::~GuiCrossRoad()
{
	delete _instance;
	delete _road;
}

void GuiCrossRoad::Start()
{
	this->Show();
	this->Draw();
}

void GuiCrossRoad::Exit()
{
}

void GuiCrossRoad::Reset()
{
}

void GuiCrossRoad::Pause()
{
}

void GuiCrossRoad::Resume()
{
}

void GuiCrossRoad::Load()
{
}

void GuiCrossRoad::Save()
{
}

void GuiCrossRoad::Update()
{
}

bool GuiCrossRoad::OnKeyPressed(unsigned short key)
{
	if (IsShowing())
	{
		WORD parsedKey = Config::ParseKey(key);

		switch (parsedKey)
		{
		case Config::Key::KEY_ESCAPSE:
			this->Hide();
			return true;

		case Config::Key::KEY_PAUSE:
			if (this->IsShowing())
			{
				this->Hide();
			}
			else
			{
				this->Show();
			}
			return true;

		case Config::Key::KEY_LOAD:
			return true;

		case Config::Key::KEY_SAVE:
			return true;
		}
		return _road->OnKeyPressed(key);
	}
	return false;
}

bool GuiCrossRoad::OnMouseMove(short x, short y)
{
	return false;
}

bool GuiCrossRoad::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	return false;
}

void GuiCrossRoad::Draw()
{
	if (_show)
	{
		while (_road->IsShowing())
		{
			Console::WaitInput(_road);
			_road->Update();
			_road->Draw();
			GuiSidePanel::GetInstance()->Draw();
			Console::Draw();
			Sleep(Config::Get(Config::FRAME_LENGTH));
		}
	}
}

void GuiCrossRoad::Show()
{
	_show = true;
	_road->Show();
}

void GuiCrossRoad::Hide()
{
	_show = false;
	_road->Hide();
}
