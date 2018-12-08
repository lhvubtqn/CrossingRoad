#include "utils.h"
#include <sstream>
#include <vector>
#include <Windows.h>

static const utils::TableChar charsDouble{
	char(187), char(201), char(188), char(200), char(205), char(203), char(202), char(186), char(204), char(185), char(206)
};

static const utils::TableChar charsSingle{
	char(191), char(218), char(217), char(192), char(196), char(194), char(193), char(179), char(195), char(180), char(197)
};

std::string utils::vk2Str(unsigned int key) {
	switch (key) {
        case VK_UP:
            return "ARROW_UP";
        case VK_DOWN: 
            return "ARROW_DOWN";
        case VK_LEFT:
            return "ARROW_LEFT";
        case VK_RIGHT:
            return "ARROW_RIGHT";
        case VK_SPACE: 
            return "SPACE";
        case VK_ESCAPE: 
            return "ESC";
        case VK_RETURN:
            return "ENTER"; 
		case VK_PAUSE:
			return "PAUSE";
        default:   
            if ((0x41 <= key && key <= 0x5A) || 
				(0x30 <= key && key <= 0x39) ||
				(0x60 <= key && key <= 0x6F)
				) 
			{
                std::string res(1, char(MapVirtualKeyA(key, MAPVK_VK_TO_CHAR)));
                return res;
            } else 
                return "NONE";
    }    
}

std::string utils::whitespace(int count) {
	if (count >= 0) {
		std::string result(count, ' ');
		return result;
	}

	return "WHITESPACE ERROR";
}

utils::TableChar utils::getChars(StrokeType t) {
	if (t == utils::DOUBLE)
		return charsDouble;
	return charsSingle;
}

int utils::cramp(int val, int start, int end) {
	if (val < start)
		val = start;
	if (val >= end)
		val = end - 1;
	return val;
}

int utils::color(int forgC, int backC) {
    return ((backC & 0x0F) << 4) + (forgC & 0x0F);
}

std::string utils::alignText(TextAlign align, std::string s, int width) {
	bool hasLineBreak = false;
	if (s.length() > 0 && s[s.length() - 1] == '\n')
	{
		s = s.substr(0, s.length() - 1);
		hasLineBreak = true;
	}

	switch (align) {
	case TextAlign::Left:
		s = s.substr(0, width);
		s += whitespace(width - s.length());
		break;

	case TextAlign::Right:
		s = s.substr(s.length() - width);
		s = whitespace(width - s.length()) + s;
		break;

	case TextAlign::Center:
		bool left = true;
		while (s.length() != width) {
			if (s.length() > width)
				s = left ? s.substr(1, s.length()) : s.substr(0, s.length() - 1);
			else
				s = left ? " " + s : s + " ";

			left = !left;
		}
		break;
	}
	if (hasLineBreak) 
		return s + '\n';
	else              
		return s;
}

std::string utils::textWithBorder(TextWithBorderOptions options, std::initializer_list<std::string> args) {

	int i = 0;
	int maxWidth = options.minWidth;
	int maxHeight = options.minHeight;

	std::string s;
	std::vector<std::string> ss;
	for (std::string arg: args) {	
		if (arg.length() > maxWidth)
			maxWidth = arg.length();		

		ss.push_back(arg);
	}

	if (ss.size() > maxHeight)
		maxHeight = ss.size();

	TableChar chars = getChars(options.type);
	std::stringstream strstream;

	strstream << chars.topLeft;
	for (i = 1; i < maxWidth + 1; i++)
		strstream << chars.horizontal;
	strstream << chars.topRight << "\n";

	for (i = 0; i < maxHeight; i++) {
		strstream << chars.vertical;
		if (i < ss.size())
			strstream << alignText(options.align, ss[i], maxWidth);
		else
			strstream << whitespace(maxWidth);
		strstream << chars.vertical << "\n";
	}
	strstream << chars.bottomLeft;
	for (i = 1; i < maxWidth + 1; i++)
		strstream << chars.horizontal;
	strstream << chars.bottomRight;

	return strstream.str();
}

std::string utils::strReplace(std::string src, std::string findThis, std::string replaceWith) {
	size_t pos;
	size_t last = 0;
	std::string result = src;
	while ((pos = src.find(findThis, last)) != std::string::npos) {
		last = pos + 1;
		result.replace(pos, findThis.length(), replaceWith);
	}			

	return result;
}