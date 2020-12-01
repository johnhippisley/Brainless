#include "macro.hpp"
#include <iostream>
extern const char ARG_NAMES[];

program_t resolveMacro(ins_t instruction)
{
	program_t implementation;
	bool found = false;
	for(macro_t macro : MACRO_LIST)
	{
		if(str::equals(instruction.name, macro.name))
		{
			found = true;
			implementation = macro.implementation;
			break;
		}
	}	
	if(!found) return {}; // No macro found
	else
	{
		for(int a = 0; a < (int) instruction.args.size(); a++)
		{
			for(int i = 0; i < (int) implementation.size(); i++) 
			{
				implementation.at(i) = str::replaceAll(implementation.at(i), std::string(1, ARG_NAMES[a]), 
					instruction.args.at(a).argString);
			}
		}
	}
	return implementation;
}

program_t resolveMacros(program_t program)
{
	program_t resolved;
	for(std::string line : program)
	{
		program_t resolvedLine = resolveMacro(parseInstruction(line));
		if(resolvedLine.size() > 0)
		{
			for(std::string part : resolvedLine) resolved.push_back(part);
		} else resolved.push_back(line);
	}
	return resolved;
}

program_t resolveMacrosRecursively(program_t program)
{
	program_t before;
	program_t after = resolveMacros(program);
	while(after != before)
	{
		before = after;
		after = resolveMacros(after);
	}
	return after;	
}
