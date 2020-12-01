/*
	Written by John G. Hippisley
	November/December 2020
*/

#include "program.hpp"
#include "specialmacro.hpp"
#include "instruction.hpp"
#include "compiler.hpp"
#include "macro.hpp"
#include "str.hpp"
#include <iostream>
#include <fstream>

bool VERBOSE;

void usage()
{
	std::cout << "Usage: ./brainless <source-file> [-V]" << std::endl;
	std::cout << "-V\tRun with verbose output" << std::endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	if(argc < 2) usage();
	if(argc > 2 && str::equals(str::toUpper(argv[2]), "-V"))
	{
		VERBOSE = true; 
		std::cout << "Running in verbose mode." << std::endl << std::endl;
	}
	else VERBOSE = false;
	program_t fProgram = loadProgram(argv[1]);
	if(fProgram.size() == 0) exit(-1);
	if(VERBOSE)
	{
		std::cout << "Loaded program:" << std::endl;
		for(int i = 0; i < (int) fProgram.size(); i++)
			std::cout << i + 1 << ")\t" << fProgram.at(i) << std::endl; 
		std::cout << std::endl;
	}
	program_t smrProgram = resolveSpecialMacros(fProgram);
	if(VERBOSE)
	{
		std::cout << "Special macros resolved:" << std::endl;
		for(int i = 0; i < (int) smrProgram.size(); i++)
			std::cout << i + 1 << ")\t" << smrProgram.at(i) << std::endl; 
		std::cout << std::endl;
	}
	std::vector<ins_t> blocksResolved = resolveBlocks(parseProgram(smrProgram));
	if(VERBOSE)
	{
		std::cout << "Parsed with blocks resolved:" << std::endl;
		int i = 0;
		for(ins_t ins : blocksResolved) 
		{
			int padding = 12 - ins.name.length(); 
			std::cout << ++i << ")\t\"" << ins.name << "\"";
			if(ins.args.size() > 0)
			{
				std::cout << ":" << std::string(padding > 0 ? padding : 0, ' ') << "{ ";
				for(int a = 0; a < (int) ins.args.size(); a++) 
					std::cout << "\"" << ins.args.at(a).argString << "\"" << (ins.args.at(a).argValue != -1 ? 
						std::string(" (").append(std::to_string(ins.args.at(a).argValue)).append(")"): "")
					 	<< (a < (int) ins.args.size() - 1 ? ", " : "");
				std::cout << " }";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	program_t fProgramWithoutMacros = resolveMacrosRecursively(reformat(blocksResolved));
	if(VERBOSE)
	{
		std::cout << "Macros resolved:" << std::endl;
		for(int i = 0; i < (int) fProgramWithoutMacros.size(); i++)
			std::cout << i + 1 << ")\t" << fProgramWithoutMacros.at(i) << std::endl; 
		std::cout << std::endl;
	}
	program_t idResolved = replaceIdentifiersInProgram(fProgramWithoutMacros);
	if(VERBOSE)
	{
		std::cout << "Identifiers resolved:" << std::endl;
		for(int i = 0; i < (int) idResolved.size(); i++)
			std::cout << i + 1 << ")\t" << idResolved.at(i) << std::endl;
		std::cout << std::endl;
	}
	program_t pProgram = genPseudoFor(parseProgram(idResolved));
	if(VERBOSE)
	{
		std::cout << "Generated pseduo-BF:" << std::endl;
		int ln = 0;
		for(std::string line : pProgram) std::cout << ++ln << ")\t" << line << std::endl;
	}
	std::string compiled = compile(pProgram);
	if(VERBOSE) std::cout << std::endl <<  "Compiled BF:" << std::endl;
	std::cout << compiled << std::endl;
}
