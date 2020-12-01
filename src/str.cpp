#include "str.hpp"
#include <iostream>

// Collection of String functions			
bool str::isNumber(std::string str)
{
    std::string::const_iterator iterator = str.begin();
    while (iterator != str.end() && std::isdigit(*iterator)) ++iterator;
    return !str.empty() && iterator == str.end();
}
	
std::string str::toUpper(std::string str)
{
	std::string ret = str;
	std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
	return ret;
}

std::vector<std::string> str::split(std::string str, char delim)
{
    std::vector<std::string> ret;
    std::stringstream sStream(str);
    std::string item;
    while (getline(sStream, item, delim)) ret.push_back(item);
    return ret;
}

std::vector<std::string> str::splitNotQuoted(std::string str, char delim)
{
	std::vector<std::string> tokens;
	std::string delimStr = std::string(1, delim);
	size_t prev = 0, pos = 0;
	do
	{
		pos = str::findNotQuoted(str, delimStr, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delimStr.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

std::string str::inBetween(std::string str, std::string start, std::string end)
{
	if(str.find(start) == std::string::npos || start.length() <= 0 || end.length() <= 0) return "";
	size_t tStart = str.find(start) + start.length() - 1;
	size_t tEnd = str.find(end, tStart + 1);
	if(tEnd == std::string::npos) return "";
	size_t length = tEnd - tStart - 1;
	if(tStart + length > str.length()) return "";
	return str.substr(tStart + 1, length);
}

bool str::isInside(std::string str, size_t pos, char token)
{
	bool left = false, right = false;
	for(size_t forward=pos; forward < str.length(); forward++) 
		if(str.at(forward) == token) right = true;
	for(int backward=pos; backward >= 0; backward--)
		if(str.at(backward) == token) left = true;
	return (left && right);
}

size_t str::findNotQuoted(std::string str, std::string token, size_t start)
{
	for(size_t offset = start; offset < str.length(); offset++)
	{
		size_t current = str.find(token, offset);
		if(current != std::string::npos && !isInside(str, current, '\'') && !isInside(str, current, '"')) return current;
	}
	return std::string::npos;
}

std::string str::replaceAll(std::string str, std::string search,std::string replace)
{
	size_t pos = 0;
	while ((pos = str.find(search, pos)) != std::string::npos)
	{
		str.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return str;
}

std::string str::replaceAllNotQuoted(std::string str, std::string search, std::string replace)
{
	std::vector<size_t> replacePos;
	size_t pos = -1;
	while ((pos = str.find(search, pos + 1)) != std::string::npos)
		if(!isInside(str, pos, '\'') && !isInside(str, pos, '\"')) replacePos.push_back(pos);
	int offset = 0;
	for(size_t cpos : replacePos)
	{
		str.replace(cpos + offset, search.length(), replace);
		offset += replace.length() - search.length();
	}		
	return str;
}

std::string str::cut(std::string str, size_t pos1, size_t pos2) { return str.substr(pos1, pos2 - pos1 + 1); }
std::string str::stripWhite(std::string str){ return replaceAll(replaceAll(str, "\t", " "), "  ", " "); }
bool str::equals(std::string a, std::string b){ return !a.compare(b); }
int str::toInt(std::string str){ return atoi(str.c_str()); }
