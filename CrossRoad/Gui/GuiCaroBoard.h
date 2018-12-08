#ifndef GUICAROBOARD_H
#define GUICAROBOARD_H

#include <vector>
#include <functional>

#include "GuiScreen.h"

struct CheckResult {
	bool hasWinner = false;
	char winner = ' ';
	int* winMarks = nullptr;
};

class GuiCaroBoard: public GuiScreen {
private:
	char* _arr;
	int _size;
	int _winLen;
	int _cursorX;
	int _cursorY;
	bool _isXTurn;
	bool _useBot;
	bool _botIsX;
	std::string _xSign;
	std::string _oSign;
	std::vector<std::string> _grid;
	CheckResult _checkResult;
	static GuiCaroBoard* _instance;
public:
	void SetWinLen(int l) { _winLen = l; RebuildGrid(); }
	int GetWinLen() { return _winLen; }
	void SetSize(int s) { _size = s; RebuildGrid(); }
	int GetSize() { return _size; }
private:
	GuiCaroBoard();
	int ToIndex(const int&, const int&);
	bool InBound(const int&, const int&);
	void CheckBoard();
	void CheckWinner(int, int);
	void RebuildGrid();	
	void WriteSidePanel();
public:
	static GuiCaroBoard* GetInstance();
	~GuiCaroBoard();
public:
	void SetBotState(bool useBot, bool botIsX);
	void ResetBoard();
	void LoadBoard(char*);
	void Draw();	
	bool OnKeyPressed(unsigned short key);
	bool OnMouseMove(short x, short y);
	bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	void OnButtonClick(int) {};
};

#endif // !GUICAROBOARD_H
