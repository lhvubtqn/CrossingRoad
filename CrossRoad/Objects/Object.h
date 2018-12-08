#ifndef _OBJECT_H
#define _OBJECT_H

#include <sstream>
#include <ctime>
#include "../Console/Console.h"
#include "../Console/Config.h"

#pragma comment(lib, "Winmm.lib")

class Object
{
public:
	enum OBJECT_TYPE
	{
		BIRD, BUS, CAR, DINOSAUR, TRUCK, PEOPLE
	};

protected:
	int _x = 0, _y = 0;
	int _attr = 15;
	int _width = 0, _height = 0;
	bool _show = false;

	double _absoluteX = 0, _absoluteY = 0;
	int _velocityX = 1;
	int _velocityY = 1;
	clock_t _lastMovingTime = clock();

public:
	virtual double X();
	virtual double Y();
	virtual int VelocityX();
	virtual int VelocityY();
	virtual int Attribute();
	virtual int Width();
	virtual int Height();
	virtual double Distance(Object* another) const;
	virtual double Distance(double x, double y) const;

	// If this contains point (x, y)
	virtual bool Contains(int x, int y) const;
	virtual bool Touches(Object* another) const;
	virtual bool IsShowing() const;
	virtual void Set(int x, int y);

	virtual void ResetTime();
	virtual void Move();
	virtual void SetVelocity(int velocityX, int velocityY);

	// Draw object
	virtual void Draw() = 0;
	virtual void DrawReverse() = 0;

	// Clear object in console
	virtual void Clear();
	virtual int Type() const = 0;
	virtual void Crash() const;
};


#endif // !_OBJECT_H
