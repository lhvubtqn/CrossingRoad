#include "GuiLoadSavedGame.h"

Gui* GuiLoadSavedGame::_instance = nullptr;
Gui* GuiLoadSavedGame::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiLoadSavedGame();
	return _instance;
}

GuiLoadSavedGame::GuiLoadSavedGame() {

	// Bounding Box
	x = 34; y = 0;
	// width = 120; height = 48; // unchange

}

GuiLoadSavedGame::~GuiLoadSavedGame() {
}

void GuiLoadSavedGame::Draw() {

}

void GuiLoadSavedGame::OnButtonClick(int btnId) {

}

void GuiLoadSavedGame::OnButtonHover(int btnId) {

}