#ifndef Gui_H
#define Gui_H

#include "../Console/InputHandler.h"
#include "../Console/Console.h"
#include <vector>
#include <string>

class Gui: public InputHandler {
protected:
	int _x = 0;
	int _y = 0;
	int _width = 120;
	int _height = 41;
	bool _show = false;
	bool _hover = false;
	bool _click = false;

public:
	virtual int Width() const;
	virtual int Height() const;

	virtual bool IsShowing() const;
	virtual bool IsHovered() const;
	virtual bool IsClicked() const;

	virtual void Draw() = 0;
	virtual void Clear();
	virtual void Show();
	virtual void Hide();
	virtual void HighLight();
	virtual void UnHighLight();
};

#endif // !Gui_H