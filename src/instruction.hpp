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
	
	{"mr", "@(39)"},	{"i1", "@(38)"},	{"i0", "@(37)"},	{"ds", "@(36)"},
	{"e11", "@(35)"},	{"e10", "@(34)"},	{"e9", "@(33)"},	{"e8", "@(32)"}, 
	{"e7", "@(31)"},	{"e6", "@(30)"},	{"e5", "@(29)"},	{"e4", "@(28)"}, 
	{"e3", "@(27)"},	{"e2", "@(26)"},	{"e1", "@(25)"},	{"e0", "@(24)"}, 
	{"b7", "@(23)"},	{"b6", "@(22)"},	{"b5", "@(21)"},	{"b4", "@(20)"}, 
	{"b3", "@(19)"},	{"b2", "@(18)"},	{"b1", "@(17)"},	{"b0", "@(16)"}, 
	{"hc", "@(15)"},	{"r0", "@(14)"},	{"r1", "@(13)"},	{"r2", "@(12)"},
	{"r3", "@(11)"},	{"r4", "@(10)"},	{"r5", "@(9)"},		{"t0", "@(8)"},
	{"t1", "@(7)"},		{"t2", "@(6)"},		{"t3", "@(5)"},		{"t4", "@(4)"},
	{"t5", "@(3)"},		{"t6", "@(2)"},		{"t7", "@(1)"},		{"t8", "@(0)"}
};

std::string replaceIdentifiers(std::string instruction);
std::string formatInstruction(std::string instruction);
ins_t parseInstruction(std::string fIns);

#endif
