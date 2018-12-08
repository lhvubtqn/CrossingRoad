#ifndef _VEHICLE_H
#define _VEHICLE_H

#include <ctime>
#include "Object.h"

class Vehicle : public Object
{
protected:
	int _velocity = 1; // step per second, only move left or right
	clock_t _lastMovingTime = clock();

public:
	virtual void SetVelocity(int velocity);
	virtual void Move();
};

#endif // !_VEHICLE_H
