#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "FileHandler.hpp"

/**
 * This tool generates a specified number of numerical values with a specified random seed 
 * value and writes them to a file. This file can be used as an input to genGrid.
 */
int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <N> <S> <outfile>" << std::endl;
    std::cerr << "\tN - number of numerical values to generate" << std::endl;
    std::cerr << "\tS - random seed value" << std::endl;
    std::cerr << "\toutfile - path to file for output" << std::endl;
    return 1;
  }

  // get arguments
  unsigned paramN = 0, paramS = 0;
  try {
    paramN = std::stoi(argv[1]);
    paramS = std::stoi(argv[2]);
  } catch (...) { 
    std::cerr << "Error: Numbers N or S are specified in an invalid format (not an integer)." << std::endl;
    return 1;
  }

  std::string outfile = argv[3];

  // open input file for reading and output file for writing
  FileHandler fout(outfile);

  // open output file
  if (!fout.openwrite()) {
    std::cerr << "Error: Failed to open file for output." << std::endl;
    return 2;
  }

  std::fstream& outfhandle(fout.handle());

  // setup random number generator
  std::default_random_engine generator(paramS);
  std::uniform_real_distribution<double> distrib(0.0, 1.0);

  unsigned numberCnt = 0;
  std::stringbuf sbuffer;

  while (numberCnt < paramN) {
    outfhandle << std::to_string(distrib(generator)) << " ";
    ++numberCnt;
  }

	return 0;
}

