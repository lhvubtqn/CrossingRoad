#ifndef _DINOSAUR_H
#define _DINOSAUR_H

#include "Object.h"

class Dinosaur : public Object
{
public:
	Dinosaur(int x = 0, int y = 0, int attr = 15);

	void Draw();
	void DrawReverse();

	int Type() const;
};

#endif // !_DINOSAUR_H
