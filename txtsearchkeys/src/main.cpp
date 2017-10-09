#include <iostream>
#include <string>
#include "FileHandler.hpp"
#include <sstream>


void printUsage(char* progname) {
  std::cerr << "Usage: " << progname << " <S> [-i <infile>] [-o <outfile>]" << std::endl;
  std::cerr << "\tS - separator string" << std::endl;
  std::cerr << "\tinfile - path to input text file (optional, stdin is default)" << std::endl;
  std::cerr << "\toutfile - path to file for output (optional, stdout is default)" << std::endl;
}

/** Forward declaration of search function */
bool searchForKeys(const std::string& sep, std::istream& input, std::ostream& output);

/**
 * This tool performs a search over an input text and detects all search keys that occur 
 * in the input text, enclosed by the specified separator string.
 * The search is performed in a character-wise manner, where each detected search key is
 * output as early as possible (on-the-go). 
 */
int main(int argc, char** argv) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  // get arguments
  const std::string SEPARATOR(argv[1]); 
  if (SEPARATOR.length() == 0) {
    std::cerr << "Error: no separator string was specified." << std::endl;
    return 2;
  }
  std::string inpath, outpath;
  bool specifyInpath = false;
  bool specifyOutpath = false;
  for (int i = 2; i < argc; ++i) { // "-i" or "-o" is given
    std::string param(argv[i]);

    if (specifyInpath) {
      inpath = param;
      specifyInpath = false;
    }
    else if (specifyOutpath) {
      outpath = param;
      specifyOutpath = false;
    }
    else if (param.compare("-i") == 0)
      specifyInpath = true;
    else if (param.compare("-o") == 0)
      specifyOutpath = true;
  }

// DEBUG:
//  std::cout << "separator: \"" << SEPARATOR << "\"" << std::endl;
//  std::cout << "use input path: " << inpath << ", use output path: " << outpath << std::endl;

  // create input data handler
  bool useStdInput = (inpath.length() == 0);
  FileHandler fhin(inpath);

  if (!useStdInput && !fhin.openread()) {
    std::cerr << "Error: input file cannot be opened or does not exist." << std::endl;
    return 3;
  }

  // create output data handler
  bool useStdOutput = (outpath.length() == 0);
  FileHandler fhout(outpath);

  if (!useStdOutput && !fhout.openwrite()) {
    std::cerr << "Error: output file cannot be opened or does not exist." << std::endl;
    return 4;
  }

  // perform search for keys in input
  if (!searchForKeys(SEPARATOR, (useStdInput ? std::cin : fhin.handle()), (useStdOutput ? std::cout : fhout.handle()) )) {
    std::cerr << "Error: search has failed during operation." << std::endl;
    return 5;
  }

  return 0;
}

bool searchForKeys(const std::string& sep, std::istream& input, std::ostream& output) {
  // state machine, states: 
  // 1) look for first separator character, no output
  // 2) separator character match, no output
  // 3) opening separator matched and store search string, no output
  // 4) closing separator character match, no output
  // 5) closing separator matched and search string is output
  // 6) end-of-text (EOT), return

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
      input.get(nullptr, sep[0]);
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
      else { // separator not matching, discard all temporarily stored characters and reset
        stateId = 1;
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
    else if (stateId == 5) { // output detected search key
      output << bufferSearch.str() << std::endl;
      bufferTmp.str("");
      bufferSearch.str("");
      bufferSep2.str("");
      stateId = 1;
    }
    else {
      std::cerr << "Error: invalid state in search stage." << std::endl;
      return false;
    }
  }

  // discard remaining text in buffer, if available
  return true;
}

