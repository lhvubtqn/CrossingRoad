#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>
#include <string>

#include "InputHandler.h"

class Console
{
private:
	static HANDLE _hStdin;
	static HANDLE _hStdout;
	static HWND _consoleW;
	static size_t _bufferSize;
	static CHAR_INFO* _buffer;
    static CONSOLE_SCREEN_BUFFER_INFO _info;	
private:
	static void MapXY(short &x, short &y);
public:
	
public:
	static void Init();
    static void Clear(); // Clear the console
	static void Clear(short x, short y, short width, short height);
	static COORD GetSize() { return _info.dwMaximumWindowSize; }
	static void WaitInput(InputHandler*);
	static void GotoXY(short, short);
    static void Write(short, short, std::string);
	static void Write(short, short, std::string, int attr);
	static void Write(short, short, char, int attr);
	static void Draw();
	static void Destroy(); // Free memory
};
#endif // !CONSOLE_H