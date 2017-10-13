#include <iostream>
#include <string>
#include "ReplaceInstructions.hpp"
#include "FileHandler.hpp"
#include "SearchReplace.hpp"
#include <sstream>


void printUsage(char* progname) {
  std::cerr << "Usage: " << progname << " <S> (<R> <T> | -r <instrfile>) [-i <infile>] [-o <outfile>]" << std::endl;
  std::cerr << "\tS - separator string" << std::endl;
  std::cerr << "\tR - expression to replace" << std::endl;
  std::cerr << "\tT - replacement string" << std::endl;
  std::cerr << "\tinstrfile - path to file with replacement instructions" << std::endl;
  std::cerr << "\tinfile - path to input text file (optional, stdin is default)" << std::endl;
  std::cerr << "\toutfile - path to file for output (optional, stdout is default)" << std::endl;
}


/**
 * This tool performs a search and replace step for an input file with given replace instructions.
 * The search is performed in a character-wise manner, where each character that is not to be
 * replaced is output at the same time, as it is read (on-the-go replacement). 
 */
int main(int argc, char** argv) {
  if (argc < 4) {
    printUsage(argv[0]);
    return 1;
  }

  // get arguments
  const std::string SEPARATOR(argv[1]); 
  if (SEPARATOR.length() == 0) {
    std::cerr << "Error: no separator string was specified." << std::endl;
    return 1;
  }
  const std::string SEARCH(argv[2]); // can be search string or "-r"
  const std::string REPLACE(argv[3]); // can be replacement string or path to file with instructions
  const bool useInstructions = (SEARCH.compare("-r") == 0);
  std::string inpath, outpath;

  bool specifyInpath = false;
  bool specifyOutpath = false;
  for (int i = 4; i < argc; ++i) { // "-i" or "-o" is given
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
  // in case same file is written, as is read, warn user
  if (inpath.length() > 0 && outpath.length() > 0 && inpath.compare(outpath) == 0) {
    std::cerr << "Warning! Identical files specified for input and output. Result will be empty." << std::endl;
    std::cerr << "Press Ctrl+C for aborting this operation." << std::endl;
    std::cin.get();
  }

// DEBUG:
//  std::cout << "separator: \"" << SEPARATOR << "\"" << std::endl;
//  if (!useInstructions)
//    std::cout << "search for '" << SEARCH << "' and replace with '" << REPLACE << "'." << std::endl;
//  else
//    std::cout << "load instructions from: " << REPLACE << std::endl;
//  std::cout << "use input path: " << inpath << ", use output path: " << outpath << std::endl;

  // load replace instructions
  ReplaceInstructions instr;
  if (!useInstructions)
    instr.insertInstruction(SEARCH, REPLACE);
  else {
    // read instructions from file
    FileHandler fh(REPLACE);
    if (!fh.openread()) {
      std::cerr << "Error: instruction file cannot be opened (may not exist)." << std::endl;
      return 2;
    }
    std::fstream& instrhdl(fh.handle());
    std::stringbuf tmp;
    std::string search, replace;
    unsigned line = 0;
    // format: linewise, first line: search string, second line: replacement string
    while (instrhdl.good()) {
      tmp.str("");
      instrhdl.get(tmp, '\n');
      instrhdl.get(); // ignore line break
      
      if (line % 2 == 0) { // get search string
        search = tmp.str();
      }
      else {
        replace = tmp.str();
//        std::cout << "instruction: '" << search << "' -> '" << replace << "'." << std::endl;
        if (search.length() > 0 && search.compare(SEPARATOR) != 0)
          instr.insertInstruction(search, replace);
      }
      ++line;
    }
  }


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

  // perform search and replace on the input
  if (!searchAndReplace(SEPARATOR, instr, (useStdInput ? std::cin : fhin.handle()), (useStdOutput ? std::cout : fhout.handle()) )) {
    std::cerr << "Error: search and replace has failed during operation." << std::endl;
    return 5;
  }

  return 0;
}

