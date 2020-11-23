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

const int HOME_OFFSET = 15;

/*
	List of all instructions, required arguments, and their implementations 
	Algorithms taken from https://https://esolangs.org/wiki/Brainfuck_algorithms
	and various other sources.
*/
#define _L arg_t::LOCATION
#define _V arg_t::VALUE
#define _H arg_t::HARD_CODE
const std::vector<insDef_t> instructionList = 
{
	// Special instructions
	{"DEBUG",	{},			"!"},
	{"BF",		{_H},		"X"},
	// Memory instructions
	{"MOV",		{_L, _V},	"@(X)[-]+(Y)HC"},
	{"MOV",		{_L, _L},	"T0[-]@(X)[-]@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]HC"},
	{"SWAP",	{_L, _L},	"T0[-]@(X)[T0+@(X)-]@(Y)[@(X)+@(Y)-]T0[@(Y)+T0-]HC"},
	// Bitwise instructions
	{"NOT",		{_L},		"T0[-]@(X)[T0+@(X)[-]]+T0[@(X)-T0-]HC"},
	{"AND",		{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]T1[T1[-]@(Y)[T1+T0+@(Y)-]T0[@(Y)+T0-]T1[@(X)+T1[-]]"
							"]HC"},
	{"OR",		{_L, _L},	"T0[-]T1[-]@(X)[T1+T0+@(X)-]T0[@(X)+T0-]T0[-]T2[-]@(Y)[T2+T0+@(Y)-]T0[@(Y)+T"
							"0-]T3[-]T4[-]T2[<<->]<+[<]+>[-<->>+>[<-<<+>>]<[-<]]>T0[-]@(X)[-]T4[@(X)+T0+"
							"T4-]T0[T4+T0-]T8[-]T7[-]T6[-]T5[-]T4[-]T3[-]T2[-]T2[-]T1[-]T0[-]HC"},
	// Arithmetic instructions
	{"ADD",		{_L, _L},	"T0[-]@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]HC"},
	{"SUB",		{_L, _L},	"T0[-]@(Y)[@(X)-T0+@(Y)-]T0[@(Y)+T0-]HC"},
	{"MUL",		{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]T1[@(Y)[@(X)+T0+@(Y)-]T0[@(Y)+T0-]T1-]HC"},
	{"DIV",		{_L, _L},	"@(8)[-]@(7)[-]@(6)[-]@(8)[-]@(5)[-]@(Y)[@(5)+@(8)+@(Y)-]@(8)[@(Y)+@(8)-]@(4"
							")[-]@(8)[-]@(3)[-]@(X)[@(3)+@(8)+@(X)-]@(8)[@(X)+@(8)-]@(3)[->+>-[>+>>]>[+["
							"-<+>]>+>>]<<<<<<]@(8)[-]@(X)[-]@(7)[@(X)+@(8)+@(7)-]@(8)[@(7)+@(8)-]@(8)[-]"
							"@(7)[-]@(6)[-]@(5)[-]@(4)[-]@(3)[-]HC"},
	// Standard output instructions
	{"PUTCHAR",	{_V},		"T0[-]+(X).[-]HC"},
	{"PUTCHAR",	{_L},		"@(X).HC"},
	{"PUTSTR",	{_L},		"@(X)[.>]HC"},
	{"PUTINT",	{_L},		"T0[-]T8[-]@(X)[T8+T0+@(X)-]T0[@(X)+T0-]T8>>+(10)<<[->+>-[>+>>]>[+[-<+>]>+>>"
							"]<(6)]>>[-]>>>+(10)<[->-[>+>>]>[+[-<+>]>+>>]<(5)]>[-]>>[>+(6)[-<+(8)>]<.<<+"
							">+>[-]]<[<[->-<]+(6)[->+(8)<]>.[-]]<<+(6)[-<+(8)>]<.[-]<<[-<+>]T8[-]T7[-]T6"
							"[-]T5[-]T4[-]T3[-]T2[-]T2[-]T1[-]T0[-]HC"},
	{"PUTINT",	{_V},		"T8[-]+(X)>>+(10)<<[->+>-[>+>>]>[+[-<+>]>+>>]<(6)]>>[-]>>>+(10)<[->-[>+>>]>["
							"+[-<+>]>+>>]<(5)]>[-]>>[>+(6)[-<+(8)>]<.<<+>+>[-]]<[<[->-<]+(6)[->+(8)<]>.["
							"-]]<<+(6)[-<+(8)>]<.[-]<<[-<+>]<[-]T8[-]T7[-]T6[-]T5[-]T4[-]T3[-]T2[-]T2[-]"
							"T1[-]T0[-]HC"},
	// Standard input instructions
	{"GETCHAR",	{_L},		"@(X),HC"},
	// Comparison instructions
	{"COMP",	{_L, _L},	"T0[-]T1[-]@(X)[T1+@(X)-]+@(Y)[T1-T0+@(Y)-]T0[@(Y)+T0-]T1[@(X)-T1[-]]"},
	{"WHILE",	{_L},		"@(X)[HC"},
	{"ENDWHILE",{_L},		"@(X)]HC"}
	// TODO Add IF _L (BOOL)
};

const char ARG_NAMES[] = "XYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char INSTRUCTIONS[] = "+-,.[]><"; 

std::string generatePseudo(ins_t ins);
std::string compile(program_t pseudoProgram);
std::string decodeShorthand(std::string pseudo);
int getCursorPosAfter(std::string program);
bool isOp(char character);

#endif

