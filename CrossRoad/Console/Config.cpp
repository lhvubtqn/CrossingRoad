#include "Config.h"

#include <conio.h>
#include <fstream>
#include <algorithm>

using namespace std;

/**
 * @Warning: MUST NOT FUCK UP THIS PART
 */ 
string const Config::STR_CONFIG[] = { 
    "NULL", "SIZE",
    "KEY_UP", "KEY_DOWN", "KEY_LEFT", "KEY_RIGHT", 
	"KEY_ENTER", "KEY_PAUSE", "KEY_ESCAPSE", 
	"KEY_LOAD", "KEY_SAVE", "KEY_MUTE", // Control
	 "FRAME_LENGTH", "CONSOLE_WIDTH", "CONSOLE_HEIGHT",// Display settings
	
};
int const Config::CONFIG_COUNT = (sizeof(Config::STR_CONFIG) / sizeof(Config::STR_CONFIG[0]));
/**
 * @Note: The later part is debuggable ~
 */ 

WORD** Config::_configs;

void Config::DefaultConfig() {
	Set(SIZE, 15);
    Set(CONSOLE_WIDTH, 1260);
    Set(CONSOLE_HEIGHT, 713);
	Set(FRAME_LENGTH, 20);
    Set(KEY_UP, 0x57, 0); // W
    Set(KEY_UP, VK_UP, 1); // up arrow
    Set(KEY_DOWN, 0x53, 0); // S
    Set(KEY_DOWN, VK_DOWN, 1); // down arrow
    Set(KEY_LEFT, 0x41, 0); // A
    Set(KEY_LEFT, VK_LEFT, 1); // left arrow
    Set(KEY_RIGHT, 0x44, 0); // D
    Set(KEY_RIGHT, VK_RIGHT, 1); // right arrow
    Set(KEY_ENTER, VK_SPACE, 0); // SPACE
    Set(KEY_ENTER, VK_RETURN, 1); // ENTER
    Set(KEY_PAUSE, 0x50, 0); // P
	Set(KEY_PAUSE, VK_PAUSE, 1); // Pause
    Set(KEY_ESCAPSE, VK_ESCAPE); // ESC
	Set(KEY_LOAD, 84); // T
	Set(KEY_SAVE, 76); // L
	Set(KEY_MUTE, 77); //  M
}
/**
 * Each line of config.txt has format like:
 *      TOKEN=00,01,
 */
void Config::ParseConfig(string s) {
    size_t eq = s.find(DELIMITER);
    if (eq == string::npos) return;

    string token = s.substr(0, eq);
    string rest = s.substr(eq + 1, s.length());

    int index = -1;
    for (int i = 0; i < CONFIG_COUNT; i++)
        if (token == STR_CONFIG[i]) {
            index = i; 
            break;
        }
    
    if (index <= 0) 
        return;

    int count = 0;
    size_t pos;
    size_t last = 0;
    while (count < 2 && (pos = rest.find_first_of(Config::SEPERATOR, last)) != string::npos) {
        _configs[index][count++] = stoi(rest.substr(last, pos - last));
        last = pos + 1;
    }
}

void Config::Init() {
    _configs = new WORD*[CONFIG_COUNT];
    for (int i = 0; i < CONFIG_COUNT; i++) {
        _configs[i] = new WORD[2];
        _configs[i][0] = 0;
        _configs[i][1] = 0;
    }
}

void Config::Destroy() {
	for (int i = 0; i < CONFIG_COUNT; i++)
		delete[] _configs[i];

	delete[] _configs;
}

void Config::Load(const char* path) {

    if (!_configs)
        Init();
    
    DefaultConfig();

    ifstream f(path); 
    if (f.is_open()) {
        string line;
        while (getline(f, line))
            ParseConfig(line);
    }
    
    f.close();
}

void Config::Save(const char* path) {
    ofstream f(path);
    if (f.is_open()) {
        for (int i = 0; i < CONFIG_COUNT; i++) {
            f << STR_CONFIG[i] << DELIMITER;
            for (size_t j = 0; j < 2; j++)
                if (_configs[i][j])
                    f << _configs[i][j] << SEPERATOR;
            f << '\n';
        }
    }
    f.close();
}

WORD Config::ParseKey(WORD k) {
    /**
     * The list of pressable key go from UP to MUTE    
     */ 
    for (int i = Key::KEY_UP; i <= Key::KEY_MUTE; i++) 
        for (int j = 0; j < 2; j++)
            if (_configs[i][j] == k) 
                return i;
        
    return k;
}

// Getter
WORD Config::Get(int k, int i) {
    if (i > 1) i = 1;
    if (k >= Key::SIZE) i = 0;
        
    return _configs[k][i];
}
// Setter
void Config::Set(int key, WORD value, int index) {
    if (index < 0 || index > 1)
        index = 0;
    _configs[key][index] = value;
}