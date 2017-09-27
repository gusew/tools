#include "ReplaceInstructions.hpp"

bool ReplaceInstructions::anyMatch(const std::string& search, std::string& replacement) const {
  InstrMap::const_iterator it;
  it = _instr.find(search);

  if (it != _instr.end()) {
    replacement = it->second;
    return true;
  }
  else
    return false;
}

void ReplaceInstructions::insertInstruction(const std::string& search, const std::string& replace) {
  _instr.insert(std::pair<std::string, std::string>(search, replace));
}

