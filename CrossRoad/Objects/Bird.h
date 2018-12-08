#ifndef _BIRD_H
#define _BIRD_H

#include "Object.h"

class Bird : public Object
{
public:
	Bird(int x = 0, int y = 0, int attr = 15);

	void Draw();
	void DrawReverse();

	int Type() const;
};

#endif // !_BIRD_H
