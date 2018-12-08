#ifndef _CAR_H
#define _CAR_H

#include "Object.h"

class Car : public Object
{
public:
	Car(int x = 0, int y = 0, int attr = 15);

	void Draw();
	void DrawReverse();

	int Type() const;
};

#endif // !_CAR_H
