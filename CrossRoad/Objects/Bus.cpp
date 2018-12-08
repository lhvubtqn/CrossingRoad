#include "Bus.h"

Bus::Bus(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 29;
	_height = 5;
	_show = false;
}

void Bus::Draw()
{
	if (_velocityX < 0)
	{
		return DrawReverse();
	}

	std::stringstream stream;
	stream
		<< R"( _________________________   )" << "\n"
		<< R"(|   |     |     |    | |  \  )" << "\n"
		<< R"(|___|_____|_____|____|_|___\ )" << "\n"
		<< R"(|                    | |    \)" << "\n"
		<< R"(`--(o)(o)--------------(o)--')";

	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void Bus::DrawReverse()
{
	std::stringstream stream;
	stream
		<< R"(   _________________________ )" << "\n"
		<< R"(  /  | |    |     |     |   |)" << "\n"
		<< R"( /___|_|____|_____|_____|___|)" << "\n"
		<< R"(/    | |                    |)" << "\n"
		<< R"('--(o)--------------(o)(o)--')";

	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

int Bus::Type() const
{
	return Object::BUS;
}


