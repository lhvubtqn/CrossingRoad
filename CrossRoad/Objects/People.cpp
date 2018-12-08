#include "People.h"

People::People(int x, int y, int attr)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
	_attr = attr;
	_width = 9;
	_height = 5;
	_show = false;
}

void People::Draw()
{
	std::stringstream stream;
	stream
		<< R"(   .-.   )" << "\n"
		<< R"( _( " )_ )" << "\n"
		<< R"((_  :  _))" << "\n"
		<< R"(  / ' \  )" << "\n"
		<< R"( (_/^\_) )";
	Console::Write(_x, _y, stream.str(), _attr);
	_show = true;
}

void People::DrawReverse()
{
	Draw();
}

int People::Type() const
{
	return Object::PEOPLE;
}
