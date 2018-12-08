#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

class InputHandler {
public:
	// If the result of the function is true then the input is consumed
	virtual bool OnKeyPressed(unsigned short key) = 0;
	// Run through Config::Parse to get intented behavior
	virtual bool OnMouseMove(short x, short y) = 0;
	virtual bool OnMouseClick(short x, short y, unsigned long mouseBtn) = 0; 
	// mouseBtn == (1) Left click
	//             (2) Right click
};

#endif // !INPUTHANDLER_H
