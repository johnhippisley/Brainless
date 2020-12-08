#ifndef MACRO_HPP
#define MACRO_HPP

#include "instruction.hpp"
#include "compiler.hpp"
#include "str.hpp"

typedef struct MacroDefinition
{
	std::string name;
	program_t implementation;
} macro_t;

// To simplify the compiler, extended instructions are 'macros'
// Name X, Y, Z, A, B, C, ...
#define SPLIT(x) str::split(x, '\n')
const std::vector<macro_t> MACRO_LIST = 
{
	{"IF_0", 	SPLIT("MOV B0,X\nIF_D B0")},
	{"IF_1", 	SPLIT("MOV B1,X\nIF_D B1")},
	{"IF_2", 	SPLIT("MOV B2,X\nIF_D B2")},
	{"IF_3", 	SPLIT("MOV B3,X\nIF_D B3")},
	{"IF_4", 	SPLIT("MOV B4,X\nIF_D B4")},
	{"IF_5", 	SPLIT("MOV B5,X\nIF_D B5")},
	{"IF_6", 	SPLIT("MOV B6,X\nIF_D B6")},
	{"IF_7", 	SPLIT("MOV B7,X\nIF_D B7")},
	{"ENDIF_0", SPLIT("ENDIF_D B0")},
	{"ENDIF_1", SPLIT("ENDIF_D B1")},
	{"ENDIF_2", SPLIT("ENDIF_D B2")},
	{"ENDIF_3", SPLIT("ENDIF_D B3")},
	{"ENDIF_4", SPLIT("ENDIF_D B4")},
	{"ENDIF_5", SPLIT("ENDIF_D B5")},
	{"ENDIF_6", SPLIT("ENDIF_D B6")},
	{"ENDIF_7", SPLIT("ENDIF_D B7")},
	{"OR",		SPLIT("MOV B7,0\nCMP B7,X\nMOV B6,0\nCMP B6,Y\nMOV X,1\nIF_D B7\nIF_D B6\nMOV X,0\nENDIF_D B6\nENDIF_D B7")},
	{"DIV", 	SPLIT("MOV E7,X\nMOV E6,Y\nMOV E5,0\nWHILE E7\nDEC E7\nDEC E6\nMOV E4,0\nCMP E4,E6\nIF_7 E4\nINC E5\nMOV "
				  	  "E6,Y\nENDIF_7\nENDWHILE E7\nMOV X,E5\nMOV E4,0\nMOV E5,0\nMOV E6,0\nMOV E7,0")},
	{"PUTDEC",	SPLIT("MOV E0,X\nMOV E1,E0\nMOV E2,100\nMOV E3,10\nMOV E8,0\nDIV E1,E2\nIF_7 E1\nINC E1,'0'\nPUTCHAR E1\n"
					  "DEC E1,'0'\nENDIF_7\nCMP E8,E1\nMUL E1,E2\nSUB E0,E1\nMOV E1,E0\nDIV E1,E3\nNOT E8\nOR E8,E1\nIF_7"
					  " E8\nINC E1,'0'\nPUTCHAR E1\nDEC E1,'0'\nENDIF_7\nMUL E1,E3\nSUB E0,E1\nMOV E1,E0\nINC E1,'0'\nPUT"
					  "CHAR E1\nMOV E0,0\nMOV E1,0\nMOV E2,0\nMOV E3,0")},
	{"READ",	SPLIT("MOV I0,Y\nMOV I1,Y\nREAD_D\nMOV X,MR\nMOV MR,0\n")},
	{"WRITE",	SPLIT("MOV I0,X\nMOV I1,X\nMOV MR,Y\nWRITE_D\n")}
};

program_t resolveMacro(ins_t instruction);
program_t resolveMacros(program_t program);
program_t resolveMacrosRecursively(program_t program);

#endif
