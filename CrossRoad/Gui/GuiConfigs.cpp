#include "GuiConfigs.h"

#include "GuiManager.h"
#include "../Console/Config.h"
#include "../Console/Console.h"
#include "../Console/utils.h"
#include <sstream>

Gui* GuiConfigs::_instance = nullptr;

Gui* GuiConfigs::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiConfigs();
	return _instance;
}

GuiConfigs::GuiConfigs() {

	// Bounding Box
	x = 34; y = 0;
	// width = 120; height = 48; // unchange

}

GuiConfigs::~GuiConfigs() {
}

void GuiConfigs::Draw() {

    #define maxLenCol0 11 // "KEY_ESCAPSE".length()
    #define defCol1Len 9  // "Primary".length()
    #define defCol2Len 9  // "Secondary".length() 

	using namespace utils;

	Clear();
	int oX = Config::Get(Config::VIEW_X);
	int oY = Config::Get(Config::VIEW_Y);
    std::stringstream s;

    int maxLenCol1 = defCol1Len, 
        maxLenCol2 = defCol2Len, 
        temp, i;

    for (i = Config::Key::KEY_UP; i <= Config::Key::KEY_ESCAPSE; i++) {
        temp = vk2Str(Config::Get(i, 0)).length() + 2;
        if (temp > maxLenCol1)
            maxLenCol1 = temp;

        temp = vk2Str(Config::Get(i, 1)).length() + 2;
        if (temp > maxLenCol2)
            maxLenCol2 = temp;
    }

	auto charsDouble = getChars(StrokeType::DOUBLE);

	s << charsDouble.topLeft;
	for (i = 0; i < maxLenCol0; i++) s << charsDouble.horizontal;
	s << charsDouble.topSeparation;
	for (i = 0; i < maxLenCol1; i++) s << charsDouble.horizontal;
	s << charsDouble.topSeparation;
	for (i = 0; i < maxLenCol2; i++) s << charsDouble.horizontal;
	s << charsDouble.topRight << "\n"
	<< charsDouble.vertical << whitespace(maxLenCol0) 
	<< charsDouble.vertical << whitespace(maxLenCol1) 
	<< charsDouble.vertical << whitespace(maxLenCol2)
	<< charsDouble.vertical << "\n";

	s << charsDouble.leftSeparation;
	for (i = 0; i < maxLenCol0; i++) s << charsDouble.horizontal;
	s << charsDouble.centreSeparation;
    for (i = 0; i < maxLenCol1; i++) s << charsDouble.horizontal;
	s << charsDouble.centreSeparation;
    for (i = 0; i < maxLenCol2; i++) s << charsDouble.horizontal;
	s << charsDouble.rightSeparation << "\n";

    for (i = Config::Key::KEY_UP; i <= Config::Key::KEY_ESCAPSE; i++) {
		s << charsDouble.vertical
			<< whitespace(maxLenCol0)
			<< charsDouble.vertical
			<< whitespace(maxLenCol1)
			<< charsDouble.vertical
			<< whitespace(maxLenCol2)
			<< charsDouble.vertical << "\n";        
    }
	s << charsDouble.bottomLeft;
	for (i = 0; i < maxLenCol0; i++) s << charsDouble.horizontal;
	s << charsDouble.bottomSeparation;
	for (i = 0; i < maxLenCol1; i++) s << charsDouble.horizontal;
	s << charsDouble.bottomSeparation;
	for (i = 0; i < maxLenCol2; i++) s << charsDouble.horizontal;
	s << charsDouble.bottomRight << "\n";

    Console::Write(oX, oY + 1, s.str(), Color::LightGreen);

	Console::Write(oX + 1, oY + 2,
		alignText(TextAlign::Center, "Control", maxLenCol0),
		Color::Cyan
	);
	Console::Write(oX + maxLenCol0 + 2, oY + 2,
		alignText(TextAlign::Center, "Primary", maxLenCol1),
		Color::Cyan
	);
	Console::Write(oX + maxLenCol0 + maxLenCol1 + 3, oY + 2,
		alignText(TextAlign::Center, "Secondary", maxLenCol2),
		Color::Cyan
	);

	for (i = Config::Key::KEY_UP; i <= Config::Key::KEY_ESCAPSE; i++) {
		Console::Write(
			oX + 2, oY + 3 + i,
			Config::STR_CONFIG[i],
			Color::LightCyan
		);
		Console::Write(
			oX + maxLenCol0 + 3, oY + 3 + i,
			vk2Str(Config::Get(i, 0)),
			Color::Yellow
		);
		Console::Write(
			oX + maxLenCol0 + maxLenCol1 + 4, oY + 3 + i,
			vk2Str(Config::Get(i, 1)),
			Color::Yellow
		);		
	}

    Console::Write(
		oX, oY , 
		"If you mess thing up delete config.ini",
		Color::LightGreen
	);
}

bool GuiConfigs::OnKeyPressed(unsigned short key) {
	bool result = false;
	return result;
}

bool GuiConfigs::OnMouseMove(short x, short y) {
	bool result = false;
	return result;
}

bool GuiConfigs::OnMouseClick(short x, short y, unsigned long mouseBtn) {
	bool result = false;
	return result;
}