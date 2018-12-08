#ifndef _PEOPLE_H
#define _PEOPLE_H

#include "Object.h"

class People : public Object
{
public:
	People(int x = 0, int y = 0, int attr = 15);
	void Draw();
	void DrawReverse();
	int Type() const;
};

#endif // !_PEOPLE_H
