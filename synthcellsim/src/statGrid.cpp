#include <iostream>
#include <string>
#include <sstream>
#include "FileHandler.hpp"
#include "GridSerializer.hpp"
#include <cmath>

/**
 * This tool reads a grid file and outputs main statistics about its state as a summary. 
 * This task can be used before and after a simulation. It is approximately as 
 * data-intensive as computationally demanding.
 */
int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <N> <infile> <outfile>" << std::endl;
    std::cerr << "\tN - size of grid in one dimension" << std::endl;
    std::cerr << "\tinfile - path to input grid file" << std::endl;
    std::cerr << "\toutfile - path to file for output" << std::endl;
    return 1;
  }

  // get arguments
  unsigned paramN = 0;
  try {
    paramN = std::stoi(argv[1]);
  } catch (...) { 
    std::cerr << "Error: Number N is specified in an invalid format (not an integer)." << std::endl;
    return 1;
  }

  std::string infile = argv[2];
  std::string outfile = argv[3];

  // get grid data from input file
  Grid g(paramN);
  if (!GridSerializer::deserialize(infile, g)) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  double tmpVal = 0.0;
  double cellLimit = paramN*paramN;
  double statSum = 0.0;
  double statMin = 0.0;
  double statMax = 0.0;
  unsigned statGreater = 0;
  unsigned statZero = 0;
  unsigned statLess = 0;

  // iterate over each cell and perform stats calculations
  for (unsigned y = 0; y < paramN; ++y) {
    for (unsigned x = 0; x < paramN; ++x) {
      tmpVal = g.getCell(x, y);
      
      statSum += tmpVal;
      if (tmpVal > statMax) statMax = tmpVal;
      if (tmpVal < statMin) statMin = tmpVal;
      
      if (tmpVal > 0.0) ++statGreater;
      else if (tmpVal < 0.0) ++statLess;
      else ++statZero;
    }
  }
  double statAvg = statSum / cellLimit;

  // iterate a second time for calculation of variance and std. deviation
  double sumSquares = 0.0; // sumQuares = sum over all cells_x (x - avg)^2
  double tmpDiff = 0.0;
  for (unsigned y = 0; y < paramN; ++y) {
    for (unsigned x = 0; x < paramN; ++x) {
      tmpVal = g.getCell(x, y);
      tmpDiff = tmpVal - statAvg;

      sumSquares += tmpDiff*tmpDiff;
    }
  }
  double statStddev = sqrt( sumSquares / (cellLimit-1) ); 

  FileHandler fout(outfile);
  if (!fout.openwrite()) {
    std::cerr << "Error: Failed to open file for output." << std::endl;
    return 3;
  }

  std::fstream& outfhandle(fout.handle());

  outfhandle << "sum " << std::to_string(statSum) << std::endl; // sum of all cell values 
  outfhandle << "avg " << std::to_string(statAvg) << std::endl; // average over all cells
  outfhandle << "stddev " << std::to_string(statStddev) << std::endl; // standard deviation of all values
  outfhandle << "min " << std::to_string(statMin) << std::endl; // minimum value cell
  outfhandle << "max " << std::to_string(statMax) << std::endl; // maximum value cell
  outfhandle << "greaterZero " << std::to_string(statGreater) << std::endl; // number of values > 0
  outfhandle << "numZero " << std::to_string(statZero) << std::endl; // number of zero cells
  outfhandle << "lessZero " << std::to_string(statLess) << std::endl; // number of cells < 0

	return 0;
}

