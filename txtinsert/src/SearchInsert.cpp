#include <sstream>
#include "SearchInsert.hpp"

bool searchAndInsert(const std::string& sep, const std::string& search, std::istream& insert, std::istream& input, std::ostream& output) {
  // state machine, states: 
  // 1) look for first separator character, normal output
  // 2) separator character match, no output, but store temporarily
  // 3) opening separator matched and store search string, no output, but continue storing
  // 4) closing separator character match, no output, continue storing
  // 5) closing separator matched and search string evaluated with instructions, replacement output or original text, if no instruction matched
  // 6) end-of-text (EOT), flush storage to output and return

  // allowed state chages:
  // 1) -> 2) on character match
  // 2) -> 1) separator characters do not match
  // 2) -> 3) all separator characters matched
  // 3) -> 4) first separator character matched
  // 4) -> 3) further separator character not matched
  // 4) -> 5) all separator characters matched
  // all -> 6) in EOT case

  unsigned stateId = 1;
  std::stringbuf bufferTmp; // stores temporarily extracted characters
  std::ostream outBufTmp(&bufferTmp);
  std::stringbuf bufferSearch; // stores the search string
  std::ostream outBufSearch(&bufferSearch);
  std::stringbuf bufferSep2; // stores all characters of the assumed second separator
  std::ostream outBufSep2(&bufferSep2);
  unsigned idxSep = 0; // moves index "pointer" in separator for string comparison

  while (input.good()) {
    if (stateId == 1) {
      // extract characters from input as long as no special character is encountered
      input.get(*output.rdbuf(), sep[0]);
      if (input.gcount() == 0 && input.fail() && !input.eof()) { // prevent from setting failbit to true
        input.clear();
      }
      stateId = 2; // match with first character of separator
      idxSep = 0;
      bufferTmp.str("");
    }
    else if (stateId == 2) {
      char read = input.get();
      if (read == sep[idxSep]) { // characters still match
        outBufTmp << read;
        ++idxSep;

        // check if matched all characters of separator
        if (idxSep == sep.length()) {
          stateId = 3; // now get the search string
          bufferSearch.str("");
        }
      }
      else { // separator not matching, append all temporarily stored characters to output and reset
        stateId = 1;
        output << bufferTmp.str() << read;
        bufferTmp.str("");
      }
    }
    else if (stateId == 3) {
      // extract characters for search string as long as no special character is encountered
      input.get(bufferSearch, sep[0]);
      if (input.gcount() == 0 && input.fail() && !input.eof()) { // prevent from setting failbit to true
        input.clear();
        // cases: a) no search string is given and here the closing separator begins, 
        // or b) search string is same as separator which is invalid usage
      }
      stateId = 4; // match with first separator character
      idxSep = 0;
      bufferSep2.str("");
    }
    else if (stateId == 4) {
      char read = input.get();
      if (read == sep[idxSep]) { // characters still match
        outBufSep2 << read;
        ++idxSep;

        // check if matched all characters of separator
        if (idxSep == sep.length()) {
          stateId = 5; // now execute instructions, if any
          idxSep = 0;
        }
      }
      else { // separator not matching, append all temporarily stored characters of assumed separator to search string
        stateId = 3;
        outBufSearch << bufferSep2.str() << read;
      }
    }
    else if (stateId == 5) { // search for replace instruction
      std::string searchStr(bufferSearch.str());
      if (searchStr.compare(search) == 0) { // insert text from file
        insert >> output.rdbuf();
      }
      else { // no match found, so restore original text
        output << sep << searchStr << sep;
      }
      bufferTmp.str("");
      bufferSearch.str("");
      bufferSep2.str("");
      stateId = 1;
    }
    else {
      std::cerr << "Error: invalid state in search and replace stage." << std::endl;
      return false;
    }
  }

  // flush remaining text from buffer to output, if available
  std::string remaining1(bufferTmp.str());
  std::string remaining2(bufferSearch.str());
  std::string remaining3(bufferSep2.str());
  if (remaining1.length() > 0) output << remaining1;
  if (remaining2.length() > 0) output << remaining2;
  if (remaining3.length() > 0) output << remaining3;

  return true;
}

// A very similar function is properly tested in SearchReplace.cpp of txtreplace.

