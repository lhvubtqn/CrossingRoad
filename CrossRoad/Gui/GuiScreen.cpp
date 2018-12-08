#include "GuiScreen.h"

#include "../Console/Config.h"
#include "../Console/Console.h"
#include "../Console/utils.h"

int GuiScreen::AddButton(short x, short y, std::string label) {    
	GuiButton btn;
	btn.show = true;
	btn.x = x;
	btn.y = y;
	btn.width = btn.height = 0;
	btn.label = label;

    size_t pos;
    size_t last = 0;
    std::string s = label + '\n';
    while ((pos = s.find_first_of('\n', last)) != std::string::npos) {
        btn.height++;
        if (pos - last > btn.width)
            btn.width = pos - last;            
        last = pos + 1;
    }
    _buttons.push_back(btn);
    return _buttons.size() - 1;
}

void GuiScreen::RemoveButton(int btnId) {
	_buttons.erase(_buttons.begin() + btnId);
}

void GuiScreen::Clear() {
	Console::Clear(x, y, width, height);
}

void GuiScreen::Draw() {
	Clear();
    GuiButton btn;
    for (size_t i = 0; i < _buttons.size(); i++) {
        btn = _buttons[i];
		if (btn.show)
			Console::Write(
				btn.x, btn.y, btn.label, 
				i == _selectedButton ? 
				utils::color(utils::Color::Black, utils::Color::Green) :
				utils::Color::Green
			);
    }
}

bool GuiScreen::OnKeyPressed(unsigned short key) {

    if (_buttons.size() == 0)
        return false;

	int hasVisibleButton = 0;
	for (GuiButton btn : _buttons)
		hasVisibleButton += btn.show;

	if (!hasVisibleButton)
		return false;

	bool result = false;
    WORD parsedKey = Config::ParseKey(key);

    switch (parsedKey) {
    case Config::Key::KEY_UP:
	case Config::Key::KEY_LEFT:
		if (_selectedButton == 0)
			_selectedButton = _buttons.size()  - 1;
		else
			_selectedButton--;
		result = true;
		break;
	case Config::Key::KEY_DOWN:
	case Config::Key::KEY_RIGHT:
		if (_selectedButton == _buttons.size() - 1)
			_selectedButton = 0;
		else
			_selectedButton++;
		result = true;
		break;
	case Config::Key::KEY_ENTER:
		OnButtonClick(_selectedButton);
		return true;
    }

	if (!_buttons[_selectedButton].show)
		OnKeyPressed(key);

    OnButtonHover(_selectedButton);
	return result;
}

bool GuiScreen::OnMouseMove(short x, short y) {    
    if (_buttons.size() == 0)
        return false;
    
    GuiButton btn;
    for (size_t i = 0; i < _buttons.size(); i++) {
        btn = _buttons[i];
        if (btn.show
		&& btn.y <= y && y < btn.y + btn.height
        && btn.x <= x && x < btn.x + btn.width) {
            _selectedButton = i;
            OnButtonHover(i);
			return true;
        }
    }

	return false;
}

bool GuiScreen::OnMouseClick(short x, short y, unsigned long mouseBtn) {
    if (_buttons.size() == 0)
        return false;
        
	if (OnMouseMove(x, y)) {
		OnButtonClick(_selectedButton);
		return true;
	}

	return false;
}

void GuiScreen::OnButtonHover(int btnId) {
    Draw();	
}