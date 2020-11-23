#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "str.hpp"

typedef struct Argument
{
	std::string argString;
	int argValue;
	enum argType_t { VALUE, LOCATION, HARD_CODE } type; 
} arg_t;

typedef struct Instruction
{
	bool valid;
	std::string name;
	std::vector<arg_t> args;
} ins_t;

const std::vector<std::pair<std::string, std::string>> IDENTIFIERS =
{
	{"hc", "@(15)"},	{"r0", "@(14)"},	{"r1", "@(13)"},	{"r2", "@(12)"},
	{"r3", "@(11)"},	{"r4", "@(10)"},	{"r5", "@(9)"},		{"t0", "@(8)"},
	{"t1", "@(7)"},		{"t2", "@(6)"},		{"t3", "@(5)"},		{"t4", "@(4)"},
	{"t5", "@(3)"},		{"t6", "@(2)"},		{"t7", "@(1)"},		{"t8", "@(0)"}
};

std::string replaceIdentifiers(std::string instruction);
std::string formatInstruction(std::string instruction);
ins_t parseInstruction(std::string fIns);

#endif
