#include <iostream>
#include <string>
#include "FileHandler.hpp"
#include "SearchInsert.hpp"
#include <sstream>


void printUsage(char* progname) {
  std::cerr << "Usage: " << progname << " <S> <R> -r <insertfile> [-i <infile>] [-o <outfile>]" << std::endl;
  std::cerr << "\tS - separator string" << std::endl;
  std::cerr << "\tR - expression to replace" << std::endl;
  std::cerr << "\tinsertfile - path to file with text to insert at replace position(s)" << std::endl;
  std::cerr << "\tinfile - path to input text file (optional, stdin is default)" << std::endl;
  std::cerr << "\toutfile - path to file for output (optional, stdout is default)" << std::endl;
}


/**
 * This tool performs a search and replace step for an input file with support of a multiline
 * replace string by using an input file with the content to insert.
 * The search is performed in a character-wise manner, where each character that is not to be
 * replaced is output at the same time, as it is read (on-the-go replacement). 
 */
int main(int argc, char** argv) {
  if (argc < 5) {
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
  const std::string PARAM_R(argv[3]);
  const std::string INSERT_PATH(argv[4]);
  if (PARAM_R.compare("-r") != 0) { // check if insertfile specified
    std::cerr << "Error: no path to an insertfile was specified." << std::endl;
    return 2;
  }

  std::string inpath, outpath;
  bool specifyInpath = false;
  bool specifyOutpath = false;
  for (int i = 5; i < argc; ++i) { // "-i" or "-o" is given
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
//  std::cout << "search for '" << SEARCH << "' and replace with contents in file '" << INSERT_PATH << "'." << std::endl;
//  std::cout << "use input path: " << inpath << ", use output path: " << outpath << std::endl;

  // prepare file handle for file with content to insert (without actually reading it now)
  FileHandler fh(INSERT_PATH);
  if (!fh.openread()) {
    std::cerr << "Error: file with content to insert cannot be opened (may not exist)." << std::endl;
    return 3;
  }
  std::fstream& inserthdl(fh.handle());


  // create input data handler
  bool useStdInput = (inpath.length() == 0);
  FileHandler fhin(inpath);

  if (!useStdInput && !fhin.openread()) {
    std::cerr << "Error: input file cannot be opened or does not exist." << std::endl;
    return 4;
  }

  // create output data handler
  bool useStdOutput = (outpath.length() == 0);
  FileHandler fhout(outpath);

  if (!useStdOutput && !fhout.openwrite()) {
    std::cerr << "Error: output file cannot be opened or does not exist." << std::endl;
    return 5;
  }

  // perform search and insert on the input
  if (!searchAndInsert(SEPARATOR, SEARCH, inserthdl, 
    (useStdInput ? std::cin : fhin.handle()), 
    (useStdOutput ? std::cout : fhout.handle()) )) {
    std::cerr << "Error: search and replace has failed during operation." << std::endl;
    return 6;
  }

  return 0;
}

