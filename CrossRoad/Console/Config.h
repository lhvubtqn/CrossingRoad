#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <Windows.h>

class Config {
private:
    static WORD** _configs;
public:
    static int const CONFIG_COUNT;
    static std::string const STR_CONFIG[];    
    static const char DELIMITER = '=';
    static const char SEPERATOR = ',';
    enum Key {
        UNDEFINED, SIZE,
        KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 
        KEY_ENTER, KEY_PAUSE, KEY_ESCAPSE, 
		KEY_LOAD, KEY_SAVE, KEY_MUTE,
        FRAME_LENGTH, CONSOLE_WIDTH, CONSOLE_HEIGHT
    }; 
public:
    static WORD Get(int key, int index = 0);
    static void Set(int key, WORD value, int index = 0);
private:
    static void DefaultConfig();
    static void ParseConfig(std::string);    
public:
    static void Init();
	static void Destroy();
    static void Load(const char*); // Load configuration from file
    static void Save(const char*); // Save configuration to a file    
    static WORD ParseKey(WORD); // Turn an int value from _getch() to a Config::Key enum value
};

#endif // !CONFIG_H