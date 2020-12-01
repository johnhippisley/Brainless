#include "instruction.hpp"
#include "compiler.hpp"
#include <iostream>

// Formats an instruction into to easily readible, uniform version
std::string formatInstruction(std::string instruction)
{
	// Basic validity detection
	bool valid = false;
	for(size_t i = 0; i < instruction.length(); i++) if(isalpha(instruction.at(i))) valid = true;
	if(!valid) return "";
	// Remove comments
	size_t commentPos = instruction.find("#");
	if(commentPos != std::string::npos) instruction = instruction.substr(0, commentPos);
	// Remove leading and trailing whitespace
	int insBegin = -1, insEnd = -1;
	for(size_t c = 0; c < instruction.length() && insBegin == -1; c++)
		if(instruction.at(c) != ' ' && instruction.at(c) != '\t' && instruction.at(c) != '\n') insBegin = c;
	for(int c = instruction.length() - 1; c >= 0 && insEnd == -1; c--)
		if(instruction.at(c) != ' ' && instruction.at(c) != '\t' && instruction.at(c) != '\n') insEnd = c;
	if(insBegin >= 0 && insEnd >= 0) instruction = str::cut(instruction, insBegin, insEnd);
	// Force uppper case instruction name
	std::string insName = str::toUpper(instruction.substr(0, instruction.find(" ")));
	instruction.replace(0, instruction.find(" "), insName);
	// Remove extra whitespace
	instruction = str::stripWhite(instruction);
	// Remove whitespace in between comas outside of "" and ''
	instruction = str::replaceAllNotQuoted(instruction, ", ", ",");
	return instruction;
}

std::string replaceIdentifiers(std::string instruction)
{
	for(std::pair<std::string, std::string> pair: IDENTIFIERS)
	{
		instruction = str::replaceAllNotQuoted(instruction, pair.first, pair.second);
		instruction = str::replaceAllNotQuoted(instruction, str::toUpper(pair.first), pair.second);
	}
	// Replace identifier Dx with @(x + DATA_OFFSET)
	for(int begin = 0; begin < (int) instruction.length() - 1; begin++)
	{
		if(toupper(instruction.at(begin)) == 'D' && begin + 1 < (int) instruction.length() &&
			isdigit(instruction.at(begin + 1)) && !str::isInside(instruction, begin, '\'') && 
			!str::isInside(instruction, begin, '"'))
		{
			size_t end = begin + 1;
			while(end < instruction.length() && isdigit(instruction.at(end))) end++;
			std::string number = std::to_string(str::toInt(str::cut(instruction, begin + 1, end - 1)) + DATA_OFFSET);
			instruction.replace(begin + 1, end - begin - 1, number);
			end += number.length() - (end - begin - 1);
			instruction.insert(end, ")");
			instruction.replace(begin, 1, "@(");
		} 
	}
	return instruction;
}

// Parses a formatted instruction into the ins_t data structure
ins_t parseInstruction(std::string fIns)
{
	ins_t ret;
	ret.valid = false;
	// Basic validation
	for(size_t i = 0; i < fIns.length(); i++) if(isalpha(fIns.at(i))) ret.valid = true;
	if(!ret.valid) return ret;
	ret.name = str::split(fIns, ' ')[0];
	size_t spaceSeperator = str::findNotQuoted(fIns, " ", 0); 
	if(spaceSeperator == std::string::npos) return ret; // No arguments
	std::vector<std::string> args = str::splitNotQuoted(fIns.substr(spaceSeperator + 1), ',');
	for(std::string argString : args)
	{
		arg_t newArg;
		newArg.argString = argString;
		// Determine argument type and value
		if(argString.at(0) == '@' && !str::equals(ret.name, "BF"))
		{
			newArg.argValue = str::toInt(str::inBetween(newArg.argString, "@(", ")"));
			newArg.type = arg_t::LOCATION;
		}
		else if(argString.at(0) == '\'')
		{
			newArg.argValue = (int) str::inBetween(newArg.argString, "'", "'").at(0);
			newArg.type = arg_t::VALUE;	
		}
		else if(isdigit(argString.at(0)))
		{
			newArg.argValue = str::toInt(newArg.argString);
			newArg.type = arg_t::VALUE;	
		}
		else
		{
			newArg.argValue = -1;
			newArg.type = arg_t::HARD_CODE;	
		}
		ret.args.push_back(newArg);
	}
	return ret;
}
