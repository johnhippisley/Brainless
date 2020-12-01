#include "compiler.hpp"
#include <iostream>

std::string generatePseudo(ins_t ins)
{
	// Find an instruction definition
	std::string pseudo;
	for(insDef_t def : instructionList)
	{
		if(str::equals(ins.name, def.name) && ins.args.size() == def.args.size())
		{
			bool match = true;
			for(size_t i = 0; i < ins.args.size(); i++)
				if(def.args.at(i) != ins.args.at(i).type) match = false;	
			if(match)
			{
				pseudo = def.pseudo;
				break;
			}
		}
	}
	if(str::equals(pseudo, "")) 
	{
		std::cout << "Couldn't match instruction '" << ins.name << "'" << std::endl;
		return ""; // No matching instruction
	}
	// Replace argument place holders with their respected argument
	for(size_t a = 0; a < ins.args.size(); a++)
	{
		arg_t arg = ins.args.at(a);
		std::string replace;
		if(arg.type == arg_t::VALUE || arg.type == arg_t::LOCATION) replace = std::to_string(arg.argValue);
		else replace = arg.argString;
		pseudo = str::replaceAll(pseudo, std::string(1, ARG_NAMES[a]), replace);
	}
	return replaceIdentifiers(pseudo);
}

std::string compile(program_t pseudoProgram)
{
	std::string header = std::string(">(").append(std::to_string(HOME_OFFSET)).append(std::string(")")); // Sets up memory structure 
	std::string compiled = decodeShorthand(header);
	for(std::string pseudo : pseudoProgram) compiled += decodeShorthand(pseudo);
	// Resolve opcode @(x); moves data pointer to x
	for(size_t op = 0; op < compiled.length(); op++)
	{
		if(compiled.at(op) == '@')
		{
			std::string prevExec = compiled.substr(0, op);
			while(prevExec.at(prevExec.length() - 1) == '[') prevExec = prevExec.substr(0, prevExec.length() - 1); // Prevent a last '['
			// Replace @(x) with neccessary '<' or '>'
			int currentPos = (int) getCursorPosAfter(prevExec);
			int goalPos = str::toInt(compiled.substr(op + 2, compiled.find(")", op + 1) - 2));
			std::string replaceString(abs(goalPos - currentPos), goalPos < currentPos ? '<' : '>');
			compiled.replace(op, compiled.find(")", op) - op + 1, replaceString);
		}
	}
	return compiled;
}

// Decodes the shorthand 'x'(y) = x... y times
std::string decodeShorthand(std::string pseudo)
{
	size_t pos;
	while((pos = pseudo.find("(")) != std::string::npos)
	{
		char before = pseudo.at(pos - 1);
		if(before != '@')
		{
			size_t segEnd = pseudo.find(")", pos);
			std::string segment = str::cut(pseudo, pos, segEnd);
			int repeat = str::toInt(str::inBetween(segment, "(", ")"));
			std::string replace(repeat, before);
			pseudo.replace(pos - 1, segEnd - pos + 2, replace);
		} else pseudo.at(pos) = '{'; // Temporarily switch as to not hold loop (lazy, I know)
	}
	return str::replaceAll(pseudo, "{", "(");
}

bool isOp(char character)
{
	bool ret = false;
	for(int i = 0; i < 8; i++)
		if(character == INSTRUCTIONS[i]) ret = true;
	return ret;
}

int getCursorPosAfter(std::string program)
{
	int dp = 0;
	for(int ip = 0; ip < (int) program.length(); ip++)
	{
		if(isOp(program.at(ip)) || program.at(ip) == '@')
		{
			switch(program.at(ip))
			{
				case '<': dp--; break;
				case '>': dp++; break;
				case '@': // Implementation of custom command @(x); sets dp = x
					std::string segment = str::cut(program, ip, program.find(")", ip));
					std::cout << segment << std::endl;
					dp = str::toInt(str::inBetween(segment, "@(", ")"));
					break;
			}
		}
	}
	return dp;	
}
