#include "specialmacro.hpp"
#include <iostream>

program_t generatePutCharSeries(std::string str)
{
	program_t ret;
	std::string strTemplate = "PUTCHAR ";
	for(int i = 0; i < (int) str.length(); i++) ret.push_back(strTemplate + std::to_string((int) str.at(i)));
	return ret;
}

program_t resolveSpecialMacros(program_t formattedProgram)
{
	for(int l = 0; l < (int) formattedProgram.size(); l++)
	{
		std::string line = formattedProgram.at(l);
		size_t space = line.find(" ");
		if(space != std::string::npos)
		{
			std::string name = line.substr(0, space);
			std::string arg;
			bool replaceMacro = false;
			if(str::equals(name, "PRINT"))
			{
				replaceMacro = true;
				arg = str::inBetween(line, "\"", "\"");
				for(int c = 1; c < (int) arg.length() - 1; c++)
				{
					// Find instances of '\n' that aren't escaped
					if(arg.at(c - 1) != '\\' && arg.at(c) == '\\' && toupper(arg.at(c + 1)) == 'N')
						arg.replace(c, 2, std::string(1, (char) 10));
				}
			}
			else if(str::equals(name, "PRINTLN"))
			{
				replaceMacro = true;
				arg = str::inBetween(line, "\"", "\"").append(std::string(1, (char) 10));
			}	
			if(replaceMacro)
			{
				program_t replace = generatePutCharSeries(arg);
				formattedProgram.erase(formattedProgram.begin() + l);
				for(int i = 0; i < (int) replace.size(); i++)
				{
					program_t::iterator iter = formattedProgram.begin() + l + i;
					formattedProgram.insert(iter, replace.at(i));
				}
			}
		}
	}
	return formattedProgram;
}

