#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>

namespace utils {
	std::string vk2Str(unsigned int key);
	std::string whitespace(int count);
	int cramp(int val, int start, int end);

	struct TableChar {
		char topRight, topLeft, bottomRight, bottomLeft,
			horizontal, topSeparation, bottomSeparation,
			vertical, leftSeparation, rightSeparation,
			centreSeparation;
	};
	enum StrokeType {
		SINGLE, DOUBLE
	};
	TableChar getChars(StrokeType);	

	enum Color {
		Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, 
		LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
	};
	int color(int forgC, int backC);

	enum TextAlign {
		Left, Right, Center
	};
	std::string alignText(TextAlign align, std::string s, int width);

	struct TextWithBorderOptions {
		int minWidth = 0, minHeight = 0;
		StrokeType type = StrokeType::DOUBLE;
		TextAlign align = TextAlign::Center;
	};
	std::string textWithBorder(TextWithBorderOptions options, std::initializer_list<std::string> args);

	template<typename...Arg>
	std::string textWithBorder(TextWithBorderOptions options, Arg... ps) {
		return textWithBorder(options, { ps... });
	}

	std::string strReplace(std::string src, std::string findThis, std::string replaceWith);
};

#endif // !UTILS_H