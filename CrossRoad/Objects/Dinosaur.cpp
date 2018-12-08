#include "Dinosaur.h"

Dinosaur::Dinosaur(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 17;
	_height = 5;
	_show = false;
}

void Dinosaur::Draw()
{
	if (_velocityX < 0)
	{
		return DrawReverse();
	}

	std::stringstream stream;
	stream
		<< R"(              __ )" << "\n"
		<< R"(     _.----._/ _))" << "\n"
		<< R"(    /         /  )" << "\n"
		<< R"( __/ (  | (  |   )" << "\n"
		<< R"(/__.-'|_|--|_|   )";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void Dinosaur::DrawReverse()
{
	std::stringstream stream;
	stream
		<< R"( __              )" << "\n"
		<< R"((_ \_.----._     )" << "\n"
		<< R"(  \         \    )" << "\n"
		<< R"(   |  ) |  ) \__ )" << "\n"
		<< R"(   |_|--|_|'-.__\)";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

int Dinosaur::Type() const
{
	return Object::DINOSAUR;
}


