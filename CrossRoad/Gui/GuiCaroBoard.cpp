#include "GuiCaroBoard.h"

#include <sstream>
#include "PopUp.h"
#include "GuiManager.h"
#include "GuiSidePanel.h"
#include "../Console/Console.h"
#include "../Console/Config.h"
#include "../Console/utils.h"

GuiCaroBoard* GuiCaroBoard::_instance = nullptr;

GuiCaroBoard* GuiCaroBoard::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiCaroBoard();
	return _instance;
}

GuiCaroBoard::GuiCaroBoard() {
	
	// Bounding Box
	x = 34; y = 0;
	// width = 120; height = 48; // unchange

	std::stringstream tempXO;
	tempXO
		<< R"(__   __)" << "\n"
		<< R"(\ \ / /)" << "\n"
		<< R"( \ V / )" << "\n"
		<< R"(  > <  )" << "\n"
		<< R"( / . \ )" << "\n"
		<< R"(/_/ \_\)";
	_xSign = tempXO.str();
	tempXO.str("");
	tempXO
		<< R"(  ____  )" << "\n"
		<< R"( / __ \ )" << "\n"
		<< R"(| |  | |)" << "\n"
		<< R"(| |  | |)" << "\n"
		<< R"(| |__| |)" << "\n"
		<< R"( \____/ )";
	_oSign = tempXO.str();
	
	_winLen = Config::Get(Config::WINNING_LENGTH);	
	_size = Config::Get(Config::SIZE);
	_arr = new char[_size * _size];	

	RebuildGrid();
	ResetBoard();
}

GuiCaroBoard::~GuiCaroBoard() {
	delete[] _arr;
	if (_checkResult.winMarks)
		delete[] _checkResult.winMarks;
}

void GuiCaroBoard::ResetBoard() {
	if (_size == 0)
		return;

	_cursorX = 0;
	_cursorY = 0;
	_isXTurn = true;
	_checkResult.hasWinner = false;
	for (int i = 0; i < _size * _size; i++)
		_arr[i] = ' ';

	GuiSidePanel::GetInstance()->AddHistory("Reset board!");
}

void GuiCaroBoard::SetBotState(bool useBot, bool botIsX) {
	_useBot = useBot;
	_botIsX = botIsX;
}

int GuiCaroBoard::ToIndex(const int &x, const int &y) {
	return x + y * _size;
}

bool GuiCaroBoard::InBound(const int &x, const int &y) {
	return 0 <= x && x < _size && 
		0 <= y && y < _size;
}

static const int checkX[] = {1, 0, 1,  1};
static const int checkY[] = {0, 1, 1, -1};
void GuiCaroBoard::CheckWinner(int x, int y) {
	int aX, aY;
	int endP;
	int checkSum, tempSum;
	char c = _arr[ToIndex(x, y)];
	_checkResult.hasWinner = false;
	if (!_checkResult.winMarks)				
		_checkResult.winMarks = new int[_winLen];

	for (int i = 0; i < 4; i++) {
		endP = _winLen - 1;
		tempSum = 0; checkSum = 0;
		for (int j = -_winLen + 1; j < _winLen; j++) {
			aX = x + checkX[i] * j;
			aY = y + checkY[i] * j;
			if (InBound(aX, aY) && c == _arr[ToIndex(aX, aY)]) {
				tempSum++;
			} else {
				if (tempSum > checkSum) {
					endP = j - 1;
					checkSum = tempSum;
				}
				tempSum = 0;
			}
		}

		if (tempSum > checkSum) {
			endP = _winLen - 1;
			checkSum = tempSum;
		}

		if (checkSum >= _winLen) {
			_checkResult.winner = c;
			_checkResult.hasWinner = true;		

			for (int j = 0; j < _winLen; j++)
				_checkResult.winMarks[j] = ToIndex(
					x + checkX[i] * (endP - j),
					y + checkY[i] * (endP - j)
				);
			// // Debug info
			// Console::setCursor(0, 0);
			// std::cout << _checkResult.winMarks[0] << '\n'
			// << _checkResult.winMarks[1] << '\n'
			// << _checkResult.winMarks[2];
			break;
		}			
	}

	if (!_checkResult.hasWinner) {
		for (int i = 0; i < _size * _size; i++)
			if (_arr[i] == ' ')
				return;

		_checkResult.hasWinner = true;
		_checkResult.winner = ' ';
	}

	if (_checkResult.hasWinner) {		
		std::stringstream t;
		if (_checkResult.winner != ' ')
			t << "Player " << _checkResult.winner << " WIN!";
		else
			t << "Player X and O DRAW!";

		GuiSidePanel::GetInstance()->AddHistory(t.str());
	}
}

void GuiCaroBoard::LoadBoard(char* a) {
	if (_size == 0)
		return;

	for (int i = 0; i < _size * _size;)
		_arr[i] = a[i];
}

void GuiCaroBoard::RebuildGrid() {

	int i;
	int n = _size * 4;
	_grid.clear();
	std::stringstream buffer;

	auto chars = utils::getChars(utils::DOUBLE);

	// First line
	buffer << chars.topLeft;
	for (i = 1; i < n; i++)
		if (i % 4 == 0)
			buffer << chars.topSeparation;
		else
			buffer << chars.horizontal;
	buffer << chars.topRight;
	_grid.push_back(buffer.str());
	// Odd line
	buffer.str("");
	for (i = 0; i < n + 1; i++)
		if (i % 4 == 0)
			buffer << chars.vertical;
		else
			buffer << ' ';
	std::string oddLine = buffer.str();
	// Even line
	buffer.str("");
	buffer << chars.leftSeparation;
	for (i = 1; i < n; i++)
		if (i % 4 == 0)
			buffer << chars.centreSeparation;
		else
			buffer << chars.horizontal;
	buffer << chars.rightSeparation;
	std::string evenLine = buffer.str();
	// Push it together
	for (i = 1; i < _size * 2; i++)
		_grid.push_back(i % 2 ? oddLine : evenLine);
	// Last line
	buffer.str("");
	buffer << chars.bottomLeft;
	for (i = 1; i < n; i++)
		if (i % 4 == 0)
			buffer << chars.bottomSeparation;
		else
			buffer << chars.horizontal;
	buffer << chars.bottomRight;
	_grid.push_back(buffer.str());
}

void GuiCaroBoard::WriteSidePanel() {
	using namespace utils;
	
	auto sidePanelInst = GuiSidePanel::GetInstance();

	sidePanelInst->WriteInfo(0, 0, "Current Player turn is:", Color::White);
	sidePanelInst->WriteInfo(12, 1,
		_isXTurn ? _xSign : _oSign,
		_isXTurn ? Color::LightRed : Color::LightBlue
	);

	sidePanelInst->WriteInfo(0, 8,  "Board size :", Color::White);
	std::stringstream strStream;
	strStream << _size << " x " << _size;
	sidePanelInst->WriteInfo(13, 8, strStream.str(), Color::Yellow);

	sidePanelInst->WriteInfo(0, 9,  "Win Length :", Color::White);
	sidePanelInst->WriteInfo(13, 9, std::to_string(_winLen), Color::Yellow);

	sidePanelInst->WriteInfo(0, 10, "Game Mode  :", Color::White);
	sidePanelInst->WriteInfo(13, 10,
		_useBot ? "Player vs Computer" : "Player vs Player",
		_useBot ? Color::LightGreen : Color::Red
	);
	if (_useBot) {
		sidePanelInst->WriteInfo(0, 11, "Bot is     :", Color::White);
		sidePanelInst->WriteInfo(13, 11,
			_botIsX ? "X" : "O",
			_botIsX ? Color::LightRed : Color::LightBlue
		);
	}
}

void GuiCaroBoard::Draw() {
	using namespace utils;

	if (_arr == NULL)
		return;

	Console::Clear();	

	int offsetX = x;// Config::Get(Config::VIEW_X);
	int offsetY = y;// Config::Get(Config::VIEW_Y);

	for (unsigned short i = 0; i < _grid.size(); i++)
		Console::Write(offsetX, offsetY + i, _grid[i], Color::Yellow);
	
	WriteSidePanel();
	
	int absI, flag;
	for (int i = 0; i < _size; i++)
		for (int j = 0; j < _size; j++) {			
			absI = ToIndex(i, j);
			char c = _arr[absI];
			// // Debug info
			// Console::setCursor(4 + i * 3, 20 + j);
			// std::cout << absI;									
			if (c != ' ') {
				flag = c == 'O' ? Color::LightBlue : Color::LightRed;				
				if (_checkResult.hasWinner && _checkResult.winner != ' ')
					for (int k = 0; k < _winLen; k++)
						if (_checkResult.winMarks[k] == absI)
							flag = color(flag, Color::LightGray);
				
				Console::Write(offsetX + 4 * i + 2, offsetY + 2 * j + 1, c, flag);
			}
		}				
	
	Console::GotoXY(offsetX + 4 * _cursorX + 2, offsetY + 2 * _cursorY + 1);
}

void GuiCaroBoard::CheckBoard() {
	if (_checkResult.hasWinner)
		return;

	int i = ToIndex(_cursorX, _cursorY);
	if (_arr[i] == ' ') {
		_arr[i] = _isXTurn ? 'X' : 'O';

		std::stringstream t;
		t << "Player " << _arr[i]
			<< " check at [" << _cursorX << ", "
			<< _cursorY << "]";
		GuiSidePanel::GetInstance()->AddHistory(t.str());

		_isXTurn = !_isXTurn;
		CheckWinner(_cursorX, _cursorY);
	}
}

bool GuiCaroBoard::OnKeyPressed(unsigned short k) {
	bool result = false;
	k = Config::ParseKey(k);
	int offsetX = x;//Config::Get(Config::VIEW_X) - _size * 2;
	int offsetY = y;//Config::Get(Config::VIEW_Y) - _size;

	switch (k) {
		case Config::Key::KEY_ESCAPSE:
			
			GuiManager::SetPrimaryGui(GuiManager::MainMenu);
		return true;
		case Config::Key::KEY_UP:
			if (--_cursorY < 0)
				_cursorY = _size -1;
			result = true;
		break;
		case Config::Key::KEY_DOWN:
			if (++_cursorY >= _size)
				_cursorY = 0;
			result = true;
		break;
		case Config::Key::KEY_LEFT:
			if (--_cursorX < 0)
				_cursorX = _size -1;
			result = true;
		break;
		case Config::Key::KEY_RIGHT:
			if (++_cursorX >= _size)
				_cursorX = 0;
			result = true;
		break;
		case Config::Key::KEY_ENTER:
			CheckBoard();
			result = true;
		break;
	}

	Console::GotoXY(offsetX + 4 * _cursorX + 2, offsetY + 2 * _cursorY + 1);
	return result;
}

bool GuiCaroBoard::OnMouseMove(short inX, short inY) {
	int offsetX = x;//Config::Get(Config::VIEW_X);
	int offsetY = y;//Config::Get(Config::VIEW_Y);
	
	_cursorX = (inX - offsetX) / 4;
	_cursorY = (inY - offsetY - 1) / 2;
	_cursorX = utils::cramp(_cursorX, 0, _size);
	_cursorY = utils::cramp(_cursorY, 0, _size);

	Console::GotoXY(offsetX + 4 * _cursorX + 2, offsetY + 2 * _cursorY + 1);
	return (
		offsetX <= x && x < offsetX + _size * 4 + 2 &&
		offsetY <= y && y < offsetY + _size * 2 + 1
	);
}

bool GuiCaroBoard::OnMouseClick(short x, short y, unsigned long mouseBtn) {
	if (OnMouseMove(x, y)) {
		CheckBoard();
		return true;
	}
	return false;
}