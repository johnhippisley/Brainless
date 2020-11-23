#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "compiler.hpp"
#include "str.hpp"

program_t loadProgram(std::string path);
std::vector<ins_t> parseProgram(program_t program);
program_t genPseudoFor(std::vector<ins_t> instructions);

#endif
