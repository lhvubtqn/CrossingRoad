#include "GuiManager.h"

#include "../Console/Console.h"
#include "GuiLoadSavedGame.h"
#include "GuiSidePanel.h"
#include "GuiCrossRoad.h"
#include "GuiMainMenu.h"
#include "GuiConfigs.h"

void GuiManager::Draw() {
	GuiContainer::GetInstance()->Draw();
}

void GuiManager::AddLayer(Gui* layer) {
	GuiContainer::layers.push_back(layer);
}

void GuiManager::RemoveLayer(Gui* layer) {
	for (auto i = GuiContainer::layers.size(); i > 0; i--)		
		if (GuiContainer::layers[i - 1] == layer) {
			GuiContainer::layers.erase(
				GuiContainer::layers.begin() + i - 1
			);
			break;
		}
}

void GuiManager::SetPrimaryGui(GuiType type, bool thenDraw) {

	switch (type){
		case GuiType::MainMenu:
			GuiContainer::layers[0] = GuiMainMenu::GetInstance();
			break;
		case GuiType::LoadSavedGame:
			GuiContainer::layers[0] = GuiLoadSavedGame::GetInstance();
			break;
		case GuiType::CrossRoad:			
			GuiContainer::layers[0] = GuiCrossRoad::GetInstance();
			break;
		case GuiType::Settings:
			GuiContainer::layers[0] = GuiConfigs::GetInstance();
			break;
		default:
			GuiContainer::layers[0] = nullptr;
			break;
	}          	

	if (GuiContainer::layers[0] && thenDraw) 
		Draw();	
}

void GuiManager::WaitInput() {
	Console::WaitInput(GuiContainer::GetInstance());
}

void GuiManager::DeleteInstances() {
	delete GuiMainMenu::GetInstance();
	delete GuiLoadSavedGame::GetInstance();
	delete GuiCrossRoad::GetInstance();
	delete GuiConfigs::GetInstance();
	delete GuiSidePanel::GetInstance();
}

std::vector<Gui*> GuiManager::GuiContainer::layers;
GuiManager::GuiContainer* GuiManager::GuiContainer::_instance = nullptr;
GuiManager::GuiContainer* GuiManager::GuiContainer::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiContainer();
	return _instance;
}

void GuiManager::GuiContainer::Draw() {
	Console::GotoXY(-1, -1);
	for (int i = 0; i < layers.size(); i++)
		layers[i]->Draw();
	Console::Draw();
}

bool GuiManager::GuiContainer::OnKeyPressed(unsigned short key) {
	if (!layers[0])
		return true;

	for (int i = layers.size() - 1; i >= 0; i--)
		if (layers[i]->OnKeyPressed(key))
			break;
	
	return true;
}

bool GuiManager::GuiContainer::OnMouseMove(short x, short y) {
	if (!layers[0])
		return true;

	for (int i = layers.size() - 1; i >= 0; i--) {
		if (layers[i]->OnMouseMove(x, y))
			break;
	}

	return true;
}

bool GuiManager::GuiContainer::OnMouseClick(short x, short y, unsigned long mouseBtn) {
	if (!layers[0])
		return true;

	for (int i = layers.size() - 1; i >= 0; i--) {
		if (layers[i]->OnMouseClick(x, y, mouseBtn))
			break;
	}

	return true;
}