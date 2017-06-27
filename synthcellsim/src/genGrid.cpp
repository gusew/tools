#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "FileHandler.hpp"

/**
 * This tool generates two dimensional grid data files (parameter is the quadratic grid size) 
 * and fills each grid cell with a value as calculated from a set of values from an input file. 
 * The input file has to contain at least N^2 x M numbers, where N is the grid size in one 
 * dimension, and M is the number of numbers that are aggregated by an operation like sum or 
 * average. This task is data intensive, but quite modest in computation intensity.
 */
int main(int argc, char** argv) {
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <N> <M> <infile> <outfile>" << std::endl;
    std::cerr << "\tN - size of grid in one dimension" << std::endl;
    std::cerr << "\tM - number of input values to aggregate for each grid cell" << std::endl;
    std::cerr << "\tinfile - path to file with input values" << std::endl;
    std::cerr << "\toutfile - path to file for output" << std::endl;
    return 1;
  }

  // get arguments
  unsigned paramN = 0, paramM = 0;
  try {
    paramN = std::stoi(argv[1]);
    paramM = std::stoi(argv[2]);
  } catch (...) { 
    std::cerr << "Error: Numbers N or M are specified in an invalid format (not an integer)." << std::endl;
    return 1;
  }

  std::string infile = argv[3];
  std::string outfile = argv[4];
//  std::cout << "Infile: " << infile << ", outfile: " << outfile << std::endl;

  // open input file for reading and output file for writing
  FileHandler fin(infile);
  FileHandler fout(outfile);

  if (!fin.openread()) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  // open output file
  if (!fout.openwrite()) {
    std::cerr << "Error: Failed to open file for output." << std::endl;
    return 3;
  }

  std::fstream& infhandle(fin.handle());
  std::fstream& outfhandle(fout.handle());

  unsigned numberCnt = 0;
  unsigned cellCnt = 0;
  unsigned cellLimit = paramN*paramN;
  double aggregNumber = 0.0;
  double tmpVal;
  std::stringbuf sbuffer;

  while (cellCnt < cellLimit) {
    // check if reading is possible
    if (!infhandle.good()) {
      std::cerr << "Error: Not enough numbers in input file for specified grid size." << std::endl;
      return 4;
    }

    // read next number as string
    infhandle.get(sbuffer, ' ');
    infhandle.ignore(1); // skip delimiter and proceed to next number
//    std::cout << "inbuffer: " << sbuffer.str() << std::endl;

    try { // try to convert to double
      tmpVal = std::stod(sbuffer.str());
    } catch (...) { // not a number
      std::cerr << "Error: Input data is not a number '" << sbuffer.str() << "'." << std::endl;
      return 5; 
    }
    // reset buffer for next number
    sbuffer.str("");

    // aggregate
    aggregNumber += tmpVal; // operator: sum
    ++numberCnt; 
//    std::cout << "tmpVal: " << std::to_string(tmpVal) << ", aggreg: " << std::to_string(aggregNumber) << ", numberCnt: " << std::to_string(numberCnt) << std::endl;

    // decide whether enough values aggregated for a cell
    if (numberCnt == paramM) {
      double cellval = aggregNumber / paramM; // operator: average
      outfhandle << std::to_string(cellval) << " ";
      ++cellCnt;
      numberCnt = 0;
    }
  }

	return 0;
}

