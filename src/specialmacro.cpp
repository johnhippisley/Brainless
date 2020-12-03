#include "specialmacro.hpp"
#include <iostream>

std::string resolveEscapeCodes(std::string str)
{
	for(int c = 0; c < (int) str.length() - 1; c++)
	{
		if(str.at(c) == '\\')
		{
			char escape = tolower(str.at(c + 1));
			if(escape == 'a') str.replace(c, 2, std::string(1, '\x07'));
			else if(escape == 'b') str.replace(c, 2, std::string(1, '\x08'));
			else if(escape == 't') str.replace(c, 2, std::string(1, '\x09'));
			else if(escape == 'n') str.replace(c, 2, std::string(1, '\x0A'));
			else if(escape == 'v') str.replace(c, 2, std::string(1, '\x0B'));
			else if(escape == 'f') str.replace(c, 2, std::string(1, '\x0C'));
			else if(escape == 'r') str.replace(c, 2, std::string(1, '\x0D'));
			else if(escape == 'e') str.replace(c, 2, std::string(1, '\x1B'));
			else if(escape == '\\') str.replace(c, 2, std::string(1, '\\'));
			else if(escape == '\'') str.replace(c, 2, std::string(1, '\''));
			else if(escape == '"') str.replace(c, 2, std::string(1, '"'));
			else if(escape == '0') str.replace(c, 2, std::string(1, '\0'));
		}
	}
	return str;
}

std::vector<int> stringToValues(std::string str)
{
	std::string deEscaped = resolveEscapeCodes(str);
	std::vector<int> ret;
	for(size_t c = 0; c < str.length(); c++) ret.push_back((int) str.at(c));
	return ret;
}

template<class T> 
std::vector<T> insertVector(size_t position, std::vector<T> vector, std::vector<T> insert)
{
	for(size_t i = 0; i < insert.size(); i++)
	{
		auto iter = vector.begin() + position + i;
		vector.insert(iter, insert.at(i));
	}
	return vector;
}

program_t generatePutCharSeries(std::string str)
{
	program_t ret;
	std::string strTemplate = "PUTCHAR ";
	for(int i = 0; i < (int) str.length(); i++) ret.push_back(strTemplate + std::to_string((int) str.at(i)));
	return ret;
}

program_t generateMovSeries(int location, std::vector<int> data)
{
	program_t ret;
	std::string strTemplate = "MOV @(x),y";
	for(int i = 0; i < (int) data.size(); i++) 
		ret.push_back(str::replaceAll(str::replaceAll(strTemplate, "x", std::to_string(location + i)), "y", std::to_string(data.at(i))));
	return ret;
}

program_t resolveSpecialMacros(program_t formattedProgram)
{
	for(int l = 0; l < (int) formattedProgram.size(); l++)
	{
		ins_t line = parseInstruction(formattedProgram.at(l));
		if(line.args.size() > 0)
		{
			std::string arg = line.args.at(0).argString;
			if(line.valid)
			{
				bool macroFound = false;
				program_t replace;
				if(str::equals(line.name, "PRINT"))
				{
					macroFound = true;
					replace = generatePutCharSeries(resolveEscapeCodes(str::inBetween(arg, "\"", "\"")));
				}
				else if(str::equals(line.name, "PRINTLN"))
				{
					macroFound = true;
					replace = generatePutCharSeries(resolveEscapeCodes(str::inBetween(arg, "\"", "\"").append(std::string(1, '\n'))));
				}
				else if(str::equals(line.name, "DATA"))
				{
					macroFound = true;
					int location = str::toInt(str::inBetween(replaceIdentifiers(arg), "@(", ")"));
					std::string arg2 = line.args.at(1).argString;
					std::vector<int> data;
					if(line.args.size() == 2 && arg2.at(0) == '"')
						data = stringToValues(resolveEscapeCodes(str::inBetween(arg2, "\"", "\"")));
					else if(line.args.size() >= 2)
						for(size_t a = 1; a < line.args.size(); a++) data.push_back(line.args.at(a).argValue);
					replace = generateMovSeries(location, data);
				}
				if(macroFound)
				{
					formattedProgram.erase(formattedProgram.begin() + l);
					formattedProgram = insertVector(l, formattedProgram, replace);
				}
			}
		}
	
	}
	return formattedProgram;
}

