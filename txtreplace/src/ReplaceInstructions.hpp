#ifndef REPLACEINSTRUCTIONS_HPP
#define REPLACEINSTRUCTIONS_HPP

#include <string>
#include <map>

typedef std::map<std::string, std::string> InstrMap;

class ReplaceInstructions {

  /** Maps a search string to a replacement string. */
  InstrMap _instr;

public:

  ReplaceInstructions() : _instr() {}

  /** Returns true, if the given search string matches to any of the instructions. In this case, return replacement string also. */
  bool anyMatch(const std::string& search, std::string& replacement) const;

  /** Inserts a new search and replace instruction into the storage. */
  void insertInstruction(const std::string& search, const std::string& replace);

};

#endif /* REPLACEINSTRUCTIONS_HPP */
