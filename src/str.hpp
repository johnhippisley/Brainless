#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>

namespace str
{
	bool equals(std::string a, std::string b);
	bool isNumber(std::string str);
	bool isInside(std::string str, size_t pos, char token);
	size_t findNotQuoted(std::string str, std::string token, size_t start);
	std::string toUpper(std::string str);
	std::vector<std::string> split(std::string str, char delim);
	std::vector<std::string> splitNotQuoted(std::string str, char delim);
	std::string inBetween(std::string str, std::string start, std::string end);
	std::string replaceAll(std::string str, std::string search, std::string replace);
	std::string replaceAllNotQuoted(std::string str, std::string search, std::string replace);
	std::string cut(std::string str, size_t pos1, size_t pos2);
	std::string stripWhite(std::string str);
	int toInt(std::string str);
}

#endif
