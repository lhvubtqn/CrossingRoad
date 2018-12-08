#include "Console.h"

#include <sstream>
#include <iostream>
#include "Config.h"
#include "utils.h"

HANDLE Console::_hStdin;
HANDLE Console::_hStdout;
HWND Console::_consoleW;
size_t Console::_bufferSize;
CHAR_INFO* Console::_buffer;
CONSOLE_SCREEN_BUFFER_INFO Console::_info;

void ErrorExit(const char* msg) {
	fprintf(stderr, "%s\n", msg);
	ExitProcess(0);
}

void Console::Init() {
	_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (_hStdout == INVALID_HANDLE_VALUE || _hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle Error");
    _consoleW = GetConsoleWindow();

	if (!SetConsoleTitleA("CrossRoad by HOANG-VU LE - 1712250"))
		ErrorExit("SetConsoleTitle Error");

	// Set window size

	if (!MoveWindow(_consoleW,
		0, 0,
		Config::Get(Config::CONSOLE_WIDTH),
		Config::Get(Config::CONSOLE_HEIGHT),
		TRUE
	))
		ErrorExit("MoveWindow Error");

    // Unresizable
	auto style = GetWindowLongA(_consoleW, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLongA(_consoleW, GWL_STYLE, style);

    // Unscrollable
	if (!GetConsoleScreenBufferInfo(_hStdout, &_info))
		ErrorExit("GetConsoleScreenBufferInfo Error");
    COORD new_size{ 
        static_cast<short>(_info.srWindow.Right - _info.srWindow.Left + 1),
        static_cast<short>(_info.srWindow.Bottom - _info.srWindow.Top + 1)
    };
    _info.dwMaximumWindowSize = new_size;
    if (!SetConsoleScreenBufferSize(_hStdout, new_size))
		ErrorExit("SetConsoleScreenBufferSize");

	// Ready to get mouse input
	if (!SetConsoleMode(_hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
		ErrorExit("SetConsoleMode");

	_bufferSize = new_size.X * new_size.Y;
	_buffer = new CHAR_INFO[_bufferSize];
	Clear();
}

void Console::Clear() {
	for (size_t i = 0; i < _bufferSize; i++) {
		_buffer[i].Attributes = 0;
		_buffer[i].Char.AsciiChar = ' ';
	}		
}

void Console::Clear(short x, short y, short width, short height) {
	for (size_t i = 0; i < width; i++)
		for (size_t j = 0; j < height; j++) {
			if (x + i + 1 > _info.dwMaximumWindowSize.X)
				break;

			if (y + j + 1 > _info.dwMaximumWindowSize.Y)
				break;

			_buffer[(x + i) + (y + j) * _info.dwMaximumWindowSize.X].Attributes = 0;
			_buffer[(x + i) + (y + j) * _info.dwMaximumWindowSize.X].Char.AsciiChar = ' ';
		}
}

void Console::WaitInput(InputHandler* inpHandler) {
	static DWORD cNumRead, i;
	static COORD cursorPos = { 0, 0 };
	static INPUT_RECORD irInBuf[128];

	auto handleKeyEvent = [&inpHandler](KEY_EVENT_RECORD ker) {
		if (ker.bKeyDown)
			inpHandler->OnKeyPressed(ker.wVirtualKeyCode);
	};
	auto handleMouseEvent = [&inpHandler](MOUSE_EVENT_RECORD mer) {
		switch (mer.dwEventFlags) {
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED ||
				mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				inpHandler->OnMouseClick(
					mer.dwMousePosition.X,
					mer.dwMousePosition.Y,
					mer.dwButtonState
				);
			}
			break;
		case MOUSE_MOVED:
			if (mer.dwMousePosition.X != cursorPos.X ||
				mer.dwMousePosition.Y != cursorPos.Y)
			{
				cursorPos = mer.dwMousePosition;
				inpHandler->OnMouseMove(
					cursorPos.X,
					cursorPos.Y
				);
			}
			break;
		case DOUBLE_CLICK:
		case MOUSE_WHEELED:
		default:
			// IGNORE
			break;
		}
	};	

	PeekConsoleInput(_hStdin, irInBuf, 128, &cNumRead);
	if (cNumRead == 0)
	{
		return;
	}

	if (!ReadConsoleInput(_hStdin, irInBuf, 128, &cNumRead)) 
		ErrorExit("ReadConsoleInput");

	for (i = 0; i < cNumRead; i++) {
		switch (irInBuf[i].EventType) {
		case KEY_EVENT: // keyboard input 
			handleKeyEvent(irInBuf[i].Event.KeyEvent);
			break;
		case MOUSE_EVENT: // mouse input 
			handleMouseEvent(irInBuf[i].Event.MouseEvent);			
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
		case FOCUS_EVENT:
		case MENU_EVENT:
			// IGNORE
		break;
		default:
			ErrorExit("Unknown event type");
			break;
		}
	}
}

void Console::MapXY(short &x, short &y) {
	if (x < 0)
		x = _info.dwMaximumWindowSize.X + x;
	if (y < 0)
		y = _info.dwMaximumWindowSize.Y + y;
}

void Console::GotoXY(short pX, short pY) {
	static COORD cursor;
	MapXY(pX, pY);
	cursor.X = pX;
	cursor.Y = pY;
	SetConsoleCursorPosition(_hStdout, cursor);
}

void Console::Write(short x, short y, std::string s, int attr) {
    
	MapXY(x, y);
    s.append("\n");
	size_t i, j = 0;
	size_t offset, pos, last = 0;    
    std::string temp;
    while ((pos = s.find_first_of('\n', last)) != std::string::npos) {

        temp = s.substr(last, pos - last);
        
		if (temp.length() > 0) {			
			offset = x + (y + j) * _info.dwMaximumWindowSize.X;
			for (i = 0; i < temp.length(); i++) {
				if (x + i + 1 > _info.dwMaximumWindowSize.X)
					break;
				_buffer[offset + i].Char.AsciiChar = temp[i];
				_buffer[offset + i].Attributes = attr;
			}
		}                    

        last = pos + 1;
        j++;
    }
}

void Console::Write(short x, short y, char c, int attribute) {
	MapXY(x, y);
	size_t offset = x + y * _info.dwMaximumWindowSize.X;
	_buffer[offset].Char.AsciiChar = c;
	_buffer[offset].Attributes = attribute;
}

void Console::Write(short x, short y, std::string s) {
    Write(x, y, s, utils::Color::White);
}

void Console::Draw() {
	static COORD p0{ 0, 0 };
	static SMALL_RECT writeArea{
		0, 0, 
		Config::Get(Config::CONSOLE_WIDTH), 
		Config::Get(Config::CONSOLE_HEIGHT)
	};

	WriteConsoleOutputA(
		_hStdout, _buffer,
		_info.dwMaximumWindowSize,
		p0, &writeArea
	);
}

void Console::Destroy() {	
	delete _buffer;
}