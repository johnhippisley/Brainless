#include "program.hpp"
#include <iostream>
#include <fstream>

program_t loadProgram(std::string path)
{
	program_t program;
	std::ifstream ifstream(path);
	if(ifstream.fail())
	{
		std::cout << "Error loading '" << path << "'" << std::endl;
		return program;
	}
	for(std::string line; std::getline(ifstream, line);)
	{
		std::string formatted = formatInstruction(line);
		if(!str::equals(formatted, "")) program.push_back(formatted);
	}
	return program;
}

program_t reformat(std::vector<ins_t> instructions)
{
	program_t ret;
	for(ins_t ins : instructions)
	{
		std::string line = ins.name.append(" ");
		for(int a = 0; a < (int) ins.args.size(); a++)
		{
			line.append(ins.args.at(a).argString);
			if(a < (int) ins.args.size() - 1) line.append(",");
		}
		ret.push_back(line);
	}
	return ret;
}

program_t replaceIdentifiersInProgram(program_t program)
{
	for(int i = 0; i < (int) program.size(); i++)
		program.at(i) = replaceIdentifiers(program.at(i));
	return program;
}

std::vector<ins_t> parseProgram(program_t program)
{
	std::vector<ins_t> instructions;
	for(std::string pseudo : program)
	{
		ins_t current = parseInstruction(pseudo);
		if(current.valid) instructions.push_back(current);
	}
	return instructions;
}

program_t genPseudoFor(std::vector<ins_t> instructions)
{
	program_t program;
	for(ins_t instruction : instructions)
	{
		std::string pseudo = generatePseudo(instruction);
		if(!str::equals(pseudo, "")) program.push_back(pseudo);
	}
	return program;
}

std::vector<ins_t> resolveBlocks(std::vector<ins_t> instructions)
{
	// Resolve WHILE blocks
	std::vector<std::string> whileStack;
	for(int i = 0; i < (int) instructions.size(); i++)
	{
		ins_t current = instructions.at(i);
		if(str::equals(current.name, "WHILE")) whileStack.push_back(current.args.at(0).argString);
		else if(str::equals(current.name, "ENDWHILE") && current.args.size() == 0)
		{
			if(whileStack.size() == 0)
			{
				std::cout << "'WHILE' has no match" << std::endl;
				return {};
			}
			arg_t newArg = { whileStack.back(), -1, arg_t::LOCATION };
			instructions.at(i).args.push_back(newArg);
			whileStack.pop_back();
		}
	}
	// Resolve IF blocks
	int nested = 0;
	for(int i = 0; i < (int) instructions.size(); i++)
	{
		if(nested < 0 || nested > 7)
		{
			std::cout << "Unmatched 'IF' or too many nested" << std::endl;
			return {};
		}
		ins_t current = instructions.at(i);
		if(str::equals(current.name, "IF")) instructions.at(i).name.append(std::string("_").append(std::to_string(nested++)));
		else if(str::equals(current.name, "ENDIF")) instructions.at(i).name.append(std::string("_").append(std::to_string(--nested)));
	}
	return instructions;
}

