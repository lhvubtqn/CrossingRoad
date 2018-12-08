#ifndef _TRUCK_H
#define _TRUCK_H

#include "Object.h"

class Truck : public Object
{
public:
	Truck(int x = 0, int y = 0, int attr = 15);

	void Draw();
	void DrawReverse();

	int Type() const;
};

#endif // !_TRUCK_H
