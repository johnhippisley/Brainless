#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "instruction.hpp"
#include "str.hpp"

typedef std::vector<std::string> program_t;

typedef struct InstructionDefinition
{
	std::string name;
	std::vector<arg_t::argType_t> args;	
	std::string pseudo;
} insDef_t;

const int HOME_OFFSET = 15, DATA_OFFSET = 40;

/*
	List of basic instructions, required arguments, and their implementations 
	Algorithms taken from https://https://esolangs.org/wiki/Brainfuck_algorithms
	and http://www.inshame.com/2008/02/efficient-brainfuck-tables.html
*/
#define _L arg_t::LOCATION
#define _V arg_t::VALUE
#define _H arg_t::HARD_CODE

// Rule #1: You cannot leave a loop with a different DP value than where you started!
const std::vector<insDef_t> instructionList = 
{
	// Special instructions
	{"DEBUG",		{},			"!"},																			// Puts '!' in output
	{"BF",			{_H},		"X"},																			// Puts X in output
	// Memory instructions
	{"MOV",			{_L, _V},	"@(X)[-]+(Y)HC"},																// Sets X = Y
	{"MOV",			{_L, _L},	"T0[-]@(X)[-]@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]HC"},								// Sets X = @(Y)
	{"SWAP",		{_L, _L},	"T0[-]@(X)[T0+@(X)-]@(Y)[@(X)+@(Y)-]T0[@(Y)+T0-]HC"},							// Swaps X and Y
	{"SHIFTR",		{_L},		"@(X)[>]<[[>+<-]<]>HC"},														// Shifts all cells until
																												// a zero value to the right 
																												// starting at X, where @(X) = 0 
	{"WRITE_D",		{},			"DS>[>>>[-<<<<+>>>>]<[->+<]<[->+<]<[->+<]>-]>>>[-]<[->+<]<[[-<+>]<<<[->>>>+<"	// Writes MR into D0 + I0
								"<<<]>>-]<<HC"},
	{"READ_D",		{},			"DS>[>>>[-<<<<+>>>>]<<[->+<]<[->+<]>-]>>>[-<+<<+>>>]<<<[->>>+<<<]>[[-<+>]>[-"	// Reads D0 + I0 into MR
								"<+>]<<<<[->>>>+<<<<]>>-]<<HC"},
	// Bitwise instructions
	{"NOT",			{_L},		"T0[-]@(X)[T0+@(X)[-]]+T0[@(X)-T0-]HC"},										// Sets X = !@(X)
	{"AND",			{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]T1[T1[-]@(Y)[T1+T0+@(Y)-]T0[@(Y)+T0-]T1[@(X)+T1[-]]"	// Sets X = @(X) & @(Y)
								"]HC"},
	// Arithmetic instructions
	{"INC",			{_L},		"@(X)+HC"},																		// Sets X = @(X) + 1
	{"INC",			{_L, _V},	"@(X)+(Y)HC"},																	// Sets X = @(X) + @(Y)
	{"DEC",			{_L},		"@(X)-HC"},																		// Sets X = @(X) - 1
	{"DEC",			{_L, _V},	"@(X)-(Y)HC"},																	// Sets X = @(X) - @(Y)
	{"ADD",			{_L, _L},	"T0[-]@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]HC"},										// Sets X = @(X) + @(Y)
	{"SUB",			{_L, _L},	"T0[-]@(Y)[@(X)-T0+@(Y)-]T0[@(Y)+T0-]HC"},										// Sets X = @(X) - @(Y)
	{"MUL",			{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]T1[@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]T1-]HC"},			// Sets X = @(X) * @(Y)
	// Standard output instructions
	{"PUTCHAR",		{_V},		"T0[-]+(X).[-]HC"},																// Puts X into STDOUT
	{"PUTCHAR",		{_L},		"@(X).HC"},																		// Puts @(X) into STDOUT
	{"PUTSTR",		{_L},		"@(X)>[.>]<[<]HC"},																// Puts null-terminated string 
																												// starting at X into STDOUT,  
																												// where @(X) = 0
	// Standard input instructions
	{"GETCHAR",		{_L},		"@(X),HC"},																		// Stores character from STDIN to X
	// Comparison instructions
	{"CMP",			{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]+@(Y)[T1-T0+@(Y)-]T0[@(Y)+T0-]T1[@(X)-T1[-]]HC"},		// Sets X = (X == Y)
	{"WHILE",		{_L},		"@(X)[HC"},																		// Loops until 'ENDWHILE X' while @(X) > 0
	{"ENDWHILE",	{_L},		"@(X)]HC"},																		// Marks end of while loop
	{"IF_D",		{_L},		"@(X)[HC"},																		// Executes up to 'ENDIF' if @(X) > 0
	{"ENDIF_D",		{_L},		"@(X)[-]]HC"}																	// Marks end of if statement; destroys X
};

const char ARG_NAMES[] = "XYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char INSTRUCTIONS[] = "+-,.[]><"; 

std::string generatePseudo(ins_t ins);
std::string compile(program_t pseudoProgram);
std::string decodeShorthand(std::string pseudo);
int getCursorPosAfter(std::string program);
bool isOp(char character);

#endif

