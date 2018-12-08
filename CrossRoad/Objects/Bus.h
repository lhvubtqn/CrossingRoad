#ifndef _BUS_H
#define _BUS_H

#include "Object.h"

class Bus : public Object
{
public:
	Bus(int x = 0, int y = 0, int attr = 15);

	void Draw();
	void DrawReverse();

	int Type() const;
};

#endif // !_BUS_H
