#include "GuiSidePanel.h"

#include "../Console/Console.h"
#include "../Console/Config.h"
#include "../Console/utils.h"

#include <sstream>

GuiSidePanel* GuiSidePanel::_instance = nullptr;
GuiSidePanel* GuiSidePanel::GetInstance() {
	if (_instance == nullptr)
		_instance = new GuiSidePanel();
	return _instance;
}

GuiSidePanel::GuiSidePanel() {
	using namespace utils;
	_x = 0;
	_y = 0;
	_width = 33;
	_height = 42;
	_info = "";
}

GuiSidePanel::~GuiSidePanel() {
}

void GuiSidePanel::SetInfo(std::string info)
{
	_info = info;
}

void GuiSidePanel::WriteHeader(short y, std::string header, short attr) {
	using namespace utils;
	auto charsDouble = getChars(StrokeType::DOUBLE);
	Console::Write(_x + _width, y, charsDouble.rightSeparation, Color::LightCyan);
	Console::Write(_x + _width, y + 2, charsDouble.rightSeparation, Color::LightCyan);
	for (short i = 0; i < _width; i++) {
		Console::Write(_x + i, y, charsDouble.horizontal, Color::LightCyan);
		Console::Write(_x + i, y + 2, charsDouble.horizontal, Color::LightCyan);
	}
	Console::Write(_x, y + 1, alignText(TextAlign::Center, header, 33), attr);
}

void GuiSidePanel::WriteInfo(short inX, short inY, std::string text, int attr) {
	Console::Write(_x + inX, _y + 17 + inY, text, attr);
}

void GuiSidePanel::ClearInfo()
{
	Console::Clear(_x, _y + 18, _width, 8);
}

void GuiSidePanel::WriteInstruct(short inX, short inY, std::string text, int attr)
{
	Console::Write(_x + inX, _y + 27 + inY, text, attr);
}

void GuiSidePanel::ClearInstruct()
{
	Console::Clear(_x, _y + 28, _width, 15);
}

bool GuiSidePanel::OnKeyPressed(unsigned short key)
{
	return false;
}

bool GuiSidePanel::OnMouseMove(short x, short y)
{
	return false;
}

bool GuiSidePanel::OnMouseClick(short x, short y, unsigned long mouseBtn)
{
	return false;
}

void GuiSidePanel::Draw() {
	using namespace utils;
	Clear();

	for (auto i = _y; i < _height; ++i)
	{
		Console::Write(_x + _width, i, utils::getChars(utils::DOUBLE).vertical, utils::Color::LightCyan);
	}

	std::stringstream stream;
	stream
		<< R"(     _____                   )" << "\n"
		<< R"(    /  __ \                  )" << "\n"
		<< R"(    | /  \/_ __ ___  ___ ___ )" << "\n"
		<< R"(    | |   | '__/ _ \/ __/ __|)" << "\n"
		<< R"(    | \__/\ | | (_) \__ \__ \)" << "\n"
		<< R"(     \____/_|  \___/|___/___/)" << "\n"
		<< R"(    ______                _ )" << "\n"
		<< R"(    | ___ \              | |)" << "\n"
		<< R"(    | |_/ /___   __ _  __| |)" << "\n"
		<< R"(    |    // _ \ / _` |/ _` |)" << "\n"
		<< R"(    | |\ \ (_) | (_| | (_| |)" << "\n"
		<< R"(    \_| \_\___/ \__,_|\__,_|)";
	Console::Write(_x, _y + 1, stream.str(), utils::Color::LightGreen);
	Console::Write(_x, _y + 14, alignText(Center, "By Hoang-Vu Le", _width), Color::DarkGray);

	WriteHeader(_y + 15, "INFORMATION");
	WriteInfo(0, 3, _info, Color::DarkGray);

	WriteHeader(_y + 25, "INSTRUCTION");
	WriteInstruct(0, 2,
		alignText(Center, std::string("        CONTROL       \n"), _width),
		Color::White
	);
	WriteInstruct(0, 3,
		alignText(Center, std::string("Up   : UP_ARROW    / W\n"), _width) +
		alignText(Center, std::string("Left : LEFT_ARROW  / A\n"), _width) +
		alignText(Center, std::string("Down : DOWN_ARROW  / S\n"), _width) +
		alignText(Center, std::string("Right: RIGHT_ARROW / D\n"), _width),
		Color::DarkGray
		);
	WriteInstruct(0, 8,
		alignText(Center, std::string("      WHILE PLAYING      \n"), _width),
		Color::White
	);
	WriteInstruct(0, 9,
		alignText(Center, std::string("Press L  : Save game     \n"), _width) +
		alignText(Center, std::string("Press T  : Load game     \n"), _width) +
		alignText(Center, std::string("Press P  : Pause/Continue\n"), _width) +
		alignText(Center, std::string("Press M  : Mute/Unmute   \n"), _width) +
		alignText(Center, std::string("Press Esc: Menu          \n"), _width),
		Color::DarkGray
		);
}