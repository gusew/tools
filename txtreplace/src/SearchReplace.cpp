#include <sstream>
#include "SearchReplace.hpp"

bool searchAndReplace(const std::string& sep, const ReplaceInstructions& instr, std::istream& input, std::ostream& output) {
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
      std::string replaceWith;
      if (instr.anyMatch(searchStr, replaceWith)) { // follow replace instruction
        output << replaceWith;
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


#ifdef UNIT_TEST
#include <cassert>
#include <iostream>

int main() {
  // some templates for the test cases
  const std::string separatorSimple("!");
  const std::string separatorComplex("<<>>");
  const std::string separatorMult("**");
  const std::string searchSimple("A simple search string with a !SEARCH key! and a few more words.");
  const std::string searchComplex("A complex search string with a <<>>ID_KEY<<>> and some odd <<>>ending!!!");
  const std::string searchMult("A string with **KEY1** replacement **KEY2**");
  const std::string searchRepeat("§limit§1key§limit§ string with §limit§1key§limit§ replacement of §limit§1key§limit§ word.");
  const std::string replaceSimple("replacement word_it");
  const std::string replaceComplex("<<>>ANOTHER_KEY<<>>");
  const std::string replaceMult1("multiple");
  const std::string replaceMult2("keys");
  const std::string replaceRepeat("a");
  const std::string resultSimple("A simple search string with a replacement word_it and a few more words.");
  const std::string resultComplex("A complex search string with a <<>>ANOTHER_KEY<<>> and some odd <<>>ending!!!");
  const std::string resultMult("A string with multiple replacement keys");
  const std::string resultRepeat("a string with a replacement of a word.");

  { // no match (no instruction)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    assert(searchAndReplace("++", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(searchSimple) == 0);
  }
  
  { // no match (single instruction)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("SEARCH key", "dummy");
    assert(searchAndReplace("++", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(searchSimple) == 0);
  }

  { // no match (but separator)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("not a SEARCH key", "dummy");
    assert(searchAndReplace(separatorSimple, instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(searchSimple) == 0);
  }
  
  { // no match (open separator)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("search", "dummy");
    assert(searchAndReplace("few", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(searchSimple) == 0);
  }

  { // single match (single instruction)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("SEARCH key", replaceSimple);
    assert(searchAndReplace("!", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(resultSimple) == 0);
  }

  { // single match (multiple instructions)
    std::stringbuf bufIn;
    bufIn.str(searchSimple);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("SEARCH key", replaceSimple);
    instr.insertInstruction("search", "dummy");
    assert(searchAndReplace("!", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(resultSimple) == 0);
  }
  
  { // multiple matches (single instruction) 
    std::stringbuf bufIn;
    bufIn.str(searchRepeat);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("1key", replaceRepeat);
    assert(searchAndReplace("§limit§", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(resultRepeat) == 0);
  }

  { // multiple matches (multiple instructions) 
    std::stringbuf bufIn;
    bufIn.str(searchMult);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("KEY1", replaceMult1);
    instr.insertInstruction("KEY2", replaceMult2);
    assert(searchAndReplace("**", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(resultMult) == 0);
  }
  
  { // no match (separator as last character) 
    std::stringbuf bufIn;
    bufIn.str(searchComplex);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("test", "dummy");
    assert(searchAndReplace("!!!", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(searchComplex) == 0);
  }

  { // awkward match at beginning (separator same as search string)
    std::stringbuf bufIn;
    bufIn.str("!!! is a test");
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("!", "that");
    assert(searchAndReplace("!", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare("!!! is a test") == 0);
  }
  
  { // awkward match at end (separator same as search string)
    std::stringbuf bufIn;
    bufIn.str("this is a test!!!");
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("!", "case");
    assert(searchAndReplace("!", instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare("this is a test!!!") == 0);
  }

  { // match (but no closing separator)
    std::stringbuf bufIn;
    bufIn.str(searchComplex);
    std::istream inStream(&bufIn);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction("ID_KEY", replaceComplex);
    assert(searchAndReplace(separatorComplex, instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(resultComplex) == 0);
  }

  { // one example for a very large text
    std::stringbuf bufBuildSearch; // create search text
    std::ostream outBuildSearch(&bufBuildSearch); 
    std::stringbuf bufBuildResult; // create result text
    std::ostream outBuildResult(&bufBuildResult);
    const std::string separator("___");
    const std::string words("Eating");
    const std::string search("ACTION");
    const std::string replace("Sleeping");

    for (unsigned word = 0; word < 1000000; ++word) {
      if (word % 100 == 0) { // search string that is replaced
        outBuildSearch << separator << search << separator << " ";
        outBuildResult << replace << " ";
      }
      else {
        outBuildSearch << words << " ";
        outBuildResult << words << " ";
      }
    }

    // setup search and replace procedure
    std::istream inStream(&bufBuildSearch);
    std::stringbuf bufOut;
    std::ostream outStream(&bufOut);
    
    ReplaceInstructions instr;
    instr.insertInstruction(search, replace);
    assert(searchAndReplace(separator, instr, inStream, outStream));
    std::string res(bufOut.str());
    assert(res.compare(bufBuildResult.str()) == 0);
  }
 

  std::cout << "Unit tests executed successfully." << std::endl;
  return EXIT_SUCCESS;
}

#endif

