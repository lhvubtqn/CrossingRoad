#include <thread>
#include "Object.h"

double Object::X()
{
	return _absoluteX;
}

double Object::Y()
{
	return _absoluteY;
}

int Object::VelocityX()
{
	return _velocityX;
}

int Object::VelocityY()
{
	return _velocityY;
}

int Object::Attribute()
{
	return _attr;
}

int Object::Width()
{
	return _width;
}

int Object::Height()
{
	return _height;
}

double Object::Distance(Object * another) const
{
	double disX = abs(another->_absoluteX - (_absoluteX + _width));
	if (disX > abs(another->_absoluteX + another->_width - _absoluteX))
		disX = abs(another->_absoluteX + another->_width - _absoluteX);

	double disY = another->_absoluteY - (_absoluteY + _height);
	if (disY > abs(another->_absoluteY + another->_height - _absoluteY))
		disY = abs(another->_absoluteY + another->_height - _absoluteY);

	return sqrt(disX * disX + disY * disY);
}

double Object::Distance(double x, double y) const
{
	double disX = abs(x - (_absoluteX + _width));
	if (disX > abs(x - _absoluteX))
		disX = abs(x - _absoluteX);

	double disY = y - (_absoluteY + _height);
	if (disY > abs(y - _absoluteY))
		disY = abs(y - _absoluteY);

	return sqrt(disX * disX + disY * disY);
}

bool Object::Contains(int x, int y) const
{
	if (_x <= x && x < _x + _width
		&& _y <= y && y < _y + _height)
		return true;
	return false;
}

bool Object::Touches(Object * another) const
{
	for(int i = _x; i < _x + _width; ++i)
		for (int j = _y; j < _y + _height; ++j)
		{
			if (another->Contains(i, j))
				return true;
		}
	return false;
}

bool Object::IsShowing() const
{
	return _show;
}

void Object::Set(int x, int y)
{
	_absoluteX = _x = x;
	_absoluteY = _y = y;
}

void Object::SetVelocity(int velocityX, int velocityY)
{
	_velocityX = velocityX;
	_velocityY = velocityY;
}

void Object::ResetTime()
{
	_lastMovingTime = clock();
}

void Object::Move()
{
	if (_velocityX + _velocityY == 0) return;

	double deltaTime = double(clock() - _lastMovingTime);
	if (deltaTime >= Config::Get(Config::FRAME_LENGTH))
	{
		_absoluteX += 1.0 * _velocityX * deltaTime / 1000;
		_absoluteY += 1.0 * _velocityY * deltaTime / 1000;
		_x = int(_absoluteX);
		_y = int(_absoluteY);
		_lastMovingTime = clock();
	}
}

void Object::Clear()
{
	Console::Clear(_x, _y, _width, _height);
	_show = false;
}

void Object::Crash() const
{
	mciSendString("play \"Sounds/crash.mp3\"", NULL, 0, NULL);
}
