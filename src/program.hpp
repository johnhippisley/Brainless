#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "compiler.hpp"
#include "str.hpp"

program_t loadProgram(std::string path);
program_t reformat(std::vector<ins_t> instructions);
program_t replaceIdentifiersInProgram(program_t program);
std::vector<ins_t> parseProgram(program_t program);
program_t genPseudoFor(std::vector<ins_t> instructions);
std::vector<ins_t> resolveBlocks(std::vector<ins_t> instructions);

#endif
