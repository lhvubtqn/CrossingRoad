#include "Truck.h"

Truck::Truck(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 14;
	_height = 4;
	_show = false;
}

void Truck::Draw()
{
	if (_velocityX < 0)
	{
		return DrawReverse();
	}

	std::stringstream stream;
	stream
		<< R"(        __    )" << "\n"
		<< R"( ______|__\__ )" << "\n"
		<< R"(|      |     |)" << "\n"
		<< R"(`(o)------(o)')";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void Truck::DrawReverse()
{
	std::stringstream stream;
	stream
		<< R"(    __        )" << "\n"
		<< R"( __/__|______ )" << "\n"
		<< R"(|     |      |)" << "\n"
		<< R"(`(o)------(o)')";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

int Truck::Type() const
{
	return Object::TRUCK;
}


