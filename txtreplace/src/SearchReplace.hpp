#ifndef SEARCHREPLACE_HPP
#define SEARCHREPLACE_HPP

#include <iostream>
#include <string>
#include "ReplaceInstructions.hpp"

bool searchAndReplace(const std::string& sep, const ReplaceInstructions& instr, std::istream& input, std::ostream& output);

#endif /* SEARCHREPLACE_HPP */
