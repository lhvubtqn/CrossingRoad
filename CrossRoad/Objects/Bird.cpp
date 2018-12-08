#include "Bird.h"

Bird::Bird(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 6;
	_height = 3;
	_show = false;
}

void Bird::Draw()
{
	if (_velocityX < 0)
	{
		return DrawReverse();
	}

	std::stringstream stream;
	stream
		<< R"(   _  )" << "\n"
		<< R"(__(.)=)" << "\n"
		<< R"(\___) )";

	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void Bird::DrawReverse()
{
	std::stringstream stream;
	stream
		<< R"(  _   )" << "\n"
		<< R"(=(.)__)" << "\n"
		<< R"( (___/)";

	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

int Bird::Type() const
{
	return Object::BIRD;
}


