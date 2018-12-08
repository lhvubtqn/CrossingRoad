#include "Car.h"

Car::Car(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 13;
	_height = 4;
	_show = false;
}

void Car::Draw()
{
	if (_velocityX < 0)
	{
		return DrawReverse();
	}

	std::stringstream stream;
	stream
		<< R"(  ______     )" << "\n"
		<< R"( /|_||_\`.__ )" << "\n"
		<< R"((   _    _ _\)" << "\n"
		<< R"( `-(_)--(_)-')";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void Car::DrawReverse()
{
	std::stringstream stream;
	stream
		<< R"(      _____  )" << "\n"
		<< R"( ___./_||_|\ )" << "\n"
		<< R"(/_ _    _   ))" << "\n"
		<< R"(`-(_)--(_)-' )";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

int Car::Type() const
{
	return Object::CAR;
}


