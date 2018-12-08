#include <fstream>

#include "GuiGameRoad.h"

GuiGameRoad* GuiGameRoad::_instance = nullptr;

GuiGameRoad* GuiGameRoad::GetInstance()
{
	if (_instance) return _instance;
	return new GuiGameRoad();
}

int GuiGameRoad::hRand(int left, int right)
{
	return (rand() * (RAND_MAX + 1)
		+ rand()) % (right - left + 1) + left;
}

void GuiGameRoad::hLoadPeople(std::istream & stream)
{
	int x, y;
	stream.read(reinterpret_cast<char *>(&x), sizeof(x));
	stream.read(reinterpret_cast<char *>(&y), sizeof(y));
	_people = new People(x, y);
}

void GuiGameRoad::hSavePeople(std::ostream & stream)
{
	int x = _people->X();
	int y = _people->Y();
	stream.write(reinterpret_cast<char *>(&x), sizeof(x));
	stream.write(reinterpret_cast<char *>(&y), sizeof(y));
}

Object* GuiGameRoad::hLoadObject(std::istream& stream)
{
	int type, x, y, attr;
	stream.read(reinterpret_cast<char *>(&type), sizeof(type));
	stream.read(reinterpret_cast<char *>(&x), sizeof(x));
	stream.read(reinterpret_cast<char *>(&y), sizeof(y));
	stream.read(reinterpret_cast<char *>(&attr), sizeof(attr));

	Object* obj = NewObject(type, attr);
	obj->Set(x, y);
	return obj;
}

void GuiGameRoad::hSaveObject(std::ostream& stream, Object* obj)
{
	int type = obj->Type();
	int x = obj->X();
	int y = obj->Y();
	int attr = obj->Attribute();
	stream.write(reinterpret_cast<char *>(&type), sizeof(type));
	stream.write(reinterpret_cast<char *>(&x), sizeof(x));
	stream.write(reinterpret_cast<char *>(&y), sizeof(y));
	stream.write(reinterpret_cast<char *>(&attr), sizeof(attr));
}

GuiGameRoad::GuiGameRoad()
{
	srand(time(NULL));
	_x = 34;
	_y = 0;
	_width = 119;
	_height = 41;
	_level = 1;
	_show = true;
	_pause = false;

	for (int i = 0; i < _laneNum; ++i)
	{
		_lanes.push_back(Lane{ _x, _y + (i + 1) * (_laneHeight + 1)});
	}
	Reset();
}

GuiGameRoad::~GuiGameRoad()
{
	if (_people != nullptr)
	{
		delete _people;
		_people = nullptr;
	}
	DeleteObjects();
}

std::string GuiGameRoad::DifficultyToString()
{
	switch (_difficulty)
	{
	case GuiGameRoad::EASY:
		return "Easy";

	case GuiGameRoad::NORMAL:
		return "Normal";

	case GuiGameRoad::HARD:
		return "Hard";
	}
	return "";
}

void GuiGameRoad::Reset()
{
	_level = 1;
	_show = true;
	_pause = false;
	DeleteObjects();
	Init();
}

void GuiGameRoad::Init()
{
	_minSpace = 20 - _level * _factor[_difficulty][0];
	int minVel = _level * _factor[_difficulty][1];
	int maxVel = _level * _factor[_difficulty][2];

	for (int i = 0; i < _laneNum; ++i)
	{
		_lanes[i].Velocity = hRand(minVel, maxVel);
		_lanes[i].Velocity *= hRand(0, 1) ? -1 : 1;
	}

	Object* newObj = nullptr;
	for (int k = 0; k < 10; ++k)
	{
		for (int i = 0; i < _laneNum; ++i)
		{
			// New Objects
			if (!newObj)
				newObj = NewObject(rand() % 4, 9 + hRand(0, 6));

			int newObjX = _lanes[i].X + hRand(0, _width - 1);
			int newObjY = _lanes[i].Y + _laneHeight - newObj->Height();

			newObj->Set(newObjX, newObjY);
			newObj->SetVelocity(_lanes[i].Velocity, 0);

			bool canAddObj = true;
			for (int j = 0; j < _lanes[i].Objects.size(); ++j)
			{
				Object* obj = _lanes[i].Objects[j];
				int space = hRand(_minSpace, _width);

				if (newObj->Touches(obj) || newObj->Distance(obj) < space)
				{
					canAddObj = false;
				}
			}

			if (canAddObj)
			{
				_lanes[i].Objects.push_back(newObj);
				newObj = nullptr;
			}
		}
	}
	_people = new People();
	_people->Set(_x + _width / 2, _lanes[_laneNum - 1].Y + (_laneHeight + 1));
	_people->SetVelocity(0, 0);
}

void GuiGameRoad::Check()
{
	if (_people->Y() < _lanes[0].Y)
	{
		_level++;
		DeleteObjects();
		Init();
		PopUp::Alert(65, 15, "NEXT LEVEL: " + std::to_string(_level));
		return;
	}

	for (int i = 0; i < _laneNum; ++i)
	{
		for (int j = 0; j < _lanes[i].Objects.size(); ++j)
		{
			if (_people->Touches(_lanes[i].Objects[j]))
			{
				std::stringstream stream;
				stream
					<< R"(                             ____                       )" << "\n"
					<< R"(                     __,-~~/~    `---.                  )" << "\n"
					<< R"(                   _/_,---(      ,    )                 )" << "\n"
					<< R"(               __ /        <    /   )  \___             )" << "\n"
					<< R"(- ------===;;;'====------------------===;;;===----- -  -)" << "\n"
					<< R"(                  \/  ~"~"~"~"~"~\~"~)~"/               )" << "\n"
					<< R"(                  (_ (   \  (     >    \)               )" << "\n"
					<< R"(                   \_( _ <         >_>'                 )" << "\n"
					<< R"(                      ~ `-i' ::>|--"                    )" << "\n"
					<< R"(                          I;|.|.|                       )" << "\n"
					<< R"(                         <|i::|i|`.                     )" << "\n"
					<< R"(                        (` ^'"`-' ")                    )";

				Console::Write(_people->X() - 20, max(0, _people->Y() - 5), stream.str(), utils::Color::Yellow);
				if (!_mute)
				{
					_people->Crash();
				}

				Console::Draw();
				Sleep(10 * Config::Get(Config::FRAME_LENGTH));

				bool result = PopUp::Question(60, 15, "You lose! Play again?");
				if (result)
				{
					this->Reset();
				}
				else
				{
					this->Hide();
				}
			}
		}
	}
}

void GuiGameRoad::AddObject()
{
	Object* newObj = nullptr;
	for (int i = 0; i < _laneNum; ++i)
	{
		// New Objects
		if (!newObj)
			newObj = NewObject(rand() % 4, 9 + hRand(0, 6));

		int newObjX = _lanes[i].X + _width - 1;
		if (_lanes[i].Velocity > 0)
			newObjX = 2;
		int newObjY = _lanes[i].Y + _laneHeight - newObj->Height();

		newObj->Set(newObjX, newObjY);
		newObj->SetVelocity(_lanes[i].Velocity, 0);

		// Check if we can add an object to current lane
		bool canAddObj = true;
		for (int j = 0; j < _lanes[i].Objects.size(); ++j)
		{
			Object* obj = _lanes[i].Objects[j];
			int space = hRand(_minSpace, _width);

			if (newObj->Touches(obj) || newObj->Distance(obj) < space)
			{
				canAddObj = false;
			}
		}

		if (canAddObj)
		{
			_lanes[i].Objects.push_back(newObj);
			newObj = nullptr;
		};
	}
}

std::string TextWithLength(std::string text, int length)
{
	while (text.length() < length)
		text += ' ';
	return text;
}

void GuiGameRoad::WriteInfo()
{
	using namespace utils;
	GuiSidePanel* panel = GuiSidePanel::GetInstance();
	panel->ClearInfo();
	panel->SetInfo(
		alignText(Center, TextWithLength("Level      : " + std::to_string(_level), 20) + '\n', panel->Width()) +
		alignText(Center, TextWithLength("Difficulty : " + DifficultyToString(), 20) + '\n', panel->Width()) +
		alignText(Center, TextWithLength("Sound      : " + std::string((_mute) ? "Off" : "On"), 20) + '\n', panel->Width())
	);
}

void GuiGameRoad::Update()
{
	int now_pause = hRand(0, 100);
	if (now_pause == 0 && _pauseFrame == 0)
	{
		_pauseFrame = (3 - _difficulty) * 10;
	}

	if (_pauseFrame > 0)
	{
		_pauseFrame--;
		if (_pauseFrame == 0)
			this->Continue();
	}

	if (_pause) return;

	this->Check();
	this->AddObject();

	for (int i = 0; i < _laneNum; ++i)
	{
		for (int j = 0; j < _lanes[i].Objects.size(); ++j)
		{
			Object* obj = _lanes[i].Objects[j];	
			if (obj->X() >= _x + _width || obj->X() < 2)
			{
				obj->Clear();
				_lanes[i].Objects.erase(_lanes[i].Objects.begin() + j++);
				continue;
			}
			obj->Clear();
			if (_pauseFrame == 0)
				obj->Move();
		}
	}
}

void GuiGameRoad::Pause()
{
	_pause = true;
}

void GuiGameRoad::Continue()
{
	_pause = false;
	for (int i = 0; i < _laneNum; ++i)
	{
		for (int j = 0; j < _lanes[i].Objects.size(); ++j)
		{
			_lanes[i].Objects[j]->ResetTime();
		}
	}
}

void GuiGameRoad::SetDifficulty(DIFFICULTY dif)
{
	_difficulty = dif;
	DeleteObjects();
	Init();
}

void GuiGameRoad::Setting()
{
	this->Pause();
	std::string result = PopUp::Prompt(65, 15, "ENTER DIFFICULTY\n\nEasy - 1\nNormal - 2\nHard - 3");

	if (result == "")
	{
		this->Continue();
		return;
	}

	if (result == "1")
		SetDifficulty(DIFFICULTY::EASY);
	else if (result == "2")
		SetDifficulty(DIFFICULTY::NORMAL);
	else if (result == "3")
		SetDifficulty(DIFFICULTY::HARD);
	else 
		PopUp::Alert(65, 15, "Invalid Input!");

	this->Continue();
}

int GuiGameRoad::hFindLane(Object* obj)
{
	for (int i = 0; i < _laneNum; ++i)
	{
		if (obj->Y() >= _lanes[i].Y)
		{
			return i;
		}
	}
}

void GuiGameRoad::Load()
{;
	this->Pause();
	
	std::string filename = PopUp::Prompt(60, 15, "Enter file name:");
	if (filename == "")
	{
		this->Continue();
		return;
	}

	filename += ".crr";
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		PopUp::Alert(60, 15, "No such file or directory!");
	}
	else
	{
		this->DeleteObjects();
		if (_people != nullptr)
		{
			delete _people;
			_people = nullptr;
		}

		file.read(reinterpret_cast<char *>(&_level), sizeof(_level));
		file.read(reinterpret_cast<char *>(&_difficulty), sizeof(_difficulty));
		this->hLoadPeople(file);

		for (int i = 0; i < _laneNum; ++i)
		{
			int size;
			file.read(reinterpret_cast<char *>(&_lanes[i].Velocity), sizeof(_lanes[i].Velocity));
			file.read(reinterpret_cast<char *>(&size), sizeof(size));
			for (int j = 0; j < size; ++j)
			{
				Object* obj = hLoadObject(file);
				obj->SetVelocity(_lanes[i].Velocity, 0);
				_lanes[i].Objects.push_back(obj);
			}
		}
		file.close();
		//PopUp::Alert(60, 15, "Successfully!");
	}
	this->Continue();
}

void GuiGameRoad::Save()
{
	this->Pause();

	std::string filename = PopUp::Prompt(60, 15, "Enter file name:");
	if (filename == "")
	{
		this->Continue();
		return;
	}

	filename += ".crr";
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		PopUp::Alert(60, 15, "Fail to save!");
	}
	else
	{
		file.write(reinterpret_cast<char *>(&_level), sizeof(_level));
		file.write(reinterpret_cast<char *>(&_difficulty), sizeof(_difficulty));
		this->hSavePeople(file);

		for (int i = 0; i < _laneNum; ++i)
		{
			int size = _lanes[i].Objects.size();
			file.write(reinterpret_cast<char *>(&_lanes[i].Velocity), sizeof(_lanes[i].Velocity));
			file.write(reinterpret_cast<char *>(&size), sizeof(size));
			for (int j = 0; j < _lanes[i].Objects.size(); ++j)
			{
				this->hSaveObject(file, _lanes[i].Objects[j]);
			}
		}
		file.close();
		PopUp::Alert(60, 15, "Successfully!");
	}
	this->Continue();
}

void GuiGameRoad::IngameMenu()
{
	this->Pause();

	GuiIngameMenu::INGAME_OPTION option = GuiIngameMenu::GetInstance()->GetOption();
	switch (option)
	{
	case GuiIngameMenu::CONTINUE:
		this->Continue();
		break;

	case GuiIngameMenu::SETTING:
		this->Setting();
		break;

	case GuiIngameMenu::LOAD:
		this->Load();
		PopUp::Alert(65, 15, "NOW LOADING...");
		Sleep(200);
		break;

	case GuiIngameMenu::SAVE:
		this->Save();
		break;

	case GuiIngameMenu::EXIT:
		this->Hide();
		break;
	}
}

bool GuiGameRoad::OnKeyPressed(unsigned short key)
{
	if (IsShowing())
	{
		WORD parsedKey = Config::ParseKey(key);
		switch (parsedKey)
		{
		case Config::KEY_PAUSE:
			if (_pause)
				this->Continue();
			else
				this->Pause();
			return true;

		case Config::KEY_ESCAPSE:
			this->IngameMenu();
			return true;

		case Config::KEY_LOAD:
			this->Load();
			return true;

		case Config::KEY_SAVE:
			this->Save();
			return true;

		case Config::KEY_MUTE:
			if (this->_mute)
			{
				mciSendString("resume mp3", NULL, 0, NULL);
				this->_mute = false;
			}
			else
			{
				mciSendString("stop mp3", NULL, 0, NULL);
				this->_mute = true;
			}
		}

		if (_pause)
			return false;

		switch (parsedKey)
		{
		case Config::KEY_UP:
			_people->Clear();
			if (_people->Y() > _lanes[0].Y - (_laneHeight + 1))
				_people->Set(_people->X(), _people->Y() - (_laneHeight + 1));
			return true;

		case Config::KEY_LEFT:
			_people->Clear();
			if (_people->X() > _x)
				_people->Set(_people->X() - 1, _people->Y());
			return true;

		case Config::KEY_DOWN:
			_people->Clear();
			if (_people->Y() < _lanes[_laneNum - 1].Y + (_laneHeight + 1))
				_people->Set(_people->X(), _people->Y() + (_laneHeight + 1));
			return true;

		case Config::KEY_RIGHT:
			_people->Clear();
			if (_people->X() < _x + _width - 1)
				_people->Set(_people->X() + 1, _people->Y());
			return true;
		}
	}
	return false;
}

bool GuiGameRoad::OnMouseMove(short x, short y)
{
	return false;
}

bool GuiGameRoad::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	return false;
}

void GuiGameRoad::DrawSideRoad(int y, int attr)
{
	for (int i = 0; i < _width; ++i)
	{
		Console::Write(_x + i, y, char(220), attr);
	}	
}

void GuiGameRoad::DrawLaneBreak(int y, int attr)
{
	int temp = 0;
	for (int i = 0; i < _width; ++i)
	{
		temp = temp % 6;
		if (temp < 3) 
			Console::Write(_x + i, y, char(196), attr);
		temp++;
	}
}

void GuiGameRoad::hDrawTrafficLight(int attr)
{
	std::stringstream stream;
	stream << char(219) << char(219);
	Console::Write(_x, _lanes[0].Y - 2, stream.str(), attr);
	Console::Write(_x + _width - 3, _lanes[0].Y - 2, stream.str(), attr);
}

void GuiGameRoad::Draw()
{
	Clear();
	Console::GotoXY(0, _y + _height);
	if (_show)
	{
		hDrawTrafficLight((_pauseFrame > 0) ? utils::Color::Red : utils::Color::Green);

		DrawSideRoad(5, utils::Color::Brown);
		for (int i = 1; i <= _laneNum - 1; ++i)
		{
			DrawLaneBreak(5 + i * (_laneHeight + 1), utils::Color::White);
		}
		DrawSideRoad(5 + _laneNum * (_laneHeight + 1), utils::Color::Brown);

		for (int i = 0; i < _laneNum; ++i)
		{
			for (int j = 0; j < _lanes[i].Objects.size(); ++j)
			{
				_lanes[i].Objects[j]->Draw();
			}
		}
		_people->Draw();
	}
	this->WriteInfo();
}

Object*  GuiGameRoad::NewObject(int type, int attr)
{
	Object* obj = nullptr;
	switch (type)
	{
	case Object::BIRD:
		obj = new Bird(0, 0, attr);
		break;
	case Object::BUS:
		obj = new Bus(0, 0, attr);
		break;
	case Object::CAR:
		obj = new Car(0, 0, attr);
		break;
	case Object::DINOSAUR:
		obj = new Dinosaur(0, 0, attr);
		break;
	case Object::TRUCK:
		obj = new Truck(0, 0, attr);
		break;
	}
	return obj;
}

void GuiGameRoad::DeleteObjects()
{
	for (int i = 0; i < _laneNum; ++i)
	{
		for (int j = 0; j < _lanes[i].Objects.size(); ++j)
			delete _lanes[i].Objects[j];
		_lanes[i].Objects.clear();
	}
}
