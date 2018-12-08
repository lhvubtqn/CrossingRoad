#ifndef GuiManager_H
#define GuiManager_H

#include "Gui.h"
#include <vector>

class GuiManager {
private:
	class GuiContainer : public InputHandler {
	private:
		static GuiContainer* _instance;
	public:
		static GuiContainer* GetInstance();
		static std::vector<Gui*> layers;
		void Draw();
		bool OnKeyPressed(unsigned short key);
		bool OnMouseMove(short x, short y);
		bool OnMouseClick(short x, short y, unsigned long mouseBtn);
	};
public:
    static Gui* GetPrimaryGui() { return GuiContainer::layers[0]; }
	enum GuiType {
		EXIT,
		MainMenu, LoadSavedGame, CrossRoad, Settings
	};
public:
	static void Draw();
	static void AddLayer(Gui* layer);
	static void RemoveLayer(Gui* layer);
	static void SetPrimaryGui(GuiType, bool thenDraw = true);
	static void WaitInput();
	static void DeleteInstances();
};

#endif // !GuiManager_H
