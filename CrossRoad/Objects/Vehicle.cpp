#include "Vehicle.h"

void Vehicle::SetVelocity(int velocity)
{
	_velocity = velocity;
}

void Vehicle::Move()
{
	if (double(clock() - _lastMovingTime) / CLOCKS_PER_SEC >= abs(1.0 / _velocity))
	{
		_x += _velocity / abs(_velocity);
		_lastMovingTime = clock();
	}
}
