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
