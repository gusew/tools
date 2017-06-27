#include <iostream>
#include <string>
#include "GridSerializer.hpp"

/**
 * This tool loads a generated grid file and runs a specified number of simulation iterations 
 * by applying some defined rules (optionally rule coefficients can be used as parameter to 
 * the task). The last grid state is output as output file. This task keeps input file size 
 * equal to output file size, but is computationally intensive with runtime complexity lying in O(N^2).
 */
int main(int argc, char** argv) {
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <N> <C> <infile> <outfile>" << std::endl;
    std::cerr << "\tN - size of grid in one dimension" << std::endl;
    std::cerr << "\tC - number of simulation iterations" << std::endl;
    std::cerr << "\tinfile - path to input grid file" << std::endl;
    std::cerr << "\toutfile - path to file for output" << std::endl;
    return 1;
  }

  // get arguments
  unsigned paramN = 0;
  unsigned paramC = 0;
  try {
    paramN = std::stoi(argv[1]);
    paramC = std::stoi(argv[2]);
  } catch (...) { 
    std::cerr << "Error: Number N or C is specified in an invalid format (not an integer)." << std::endl;
    return 1;
  }

  std::string infile = argv[3];
  std::string outfile = argv[4];

  // get grid data from input file
  Grid g(paramN);
  if (!GridSerializer::deserialize(infile, g)) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  double tmpVal;
  GridContainer neighb;
  Grid gnxt(paramN); // stores the set values for the next iteration
  gnxt.initZeros();

  for (unsigned i = 0; i < paramC; ++i) { // simulation step
    for (unsigned y = 0; y < paramN; ++y) { // iterate over each cell
      for (unsigned x = 0; x < paramN; ++x) { 
        tmpVal = g.getCell(x, y);
        g.getNeighbors(x, y, neighb);
      
        // cell manipulation operation
        double sumNeighbors = neighb[0] + neighb[1] + neighb[2] + neighb[3];
//std::cout << "cell: " << std::to_string(tmpVal) << ", neighb: " << std::to_string(sumNeighbors) << "; ";

        // compare current value to neighbors:
        if (sumNeighbors <= 0.1) // initialize in free environment
          gnxt.setCell(x, y, 1);
        else if (sumNeighbors > tmpVal * 0.05 && tmpVal > sumNeighbors) // constant domination
          gnxt.setCell(x, y, tmpVal * 0.95);
        else if (sumNeighbors <= tmpVal * 0.05) // rapid decline as there is no one to dominate over
          gnxt.setCell(x, y, tmpVal * 0.4);
        else if (tmpVal < sumNeighbors && tmpVal > sumNeighbors * 0.75) // overpopulation, shrink slowly
          gnxt.setCell(x, y, tmpVal * 0.9);
        else if (tmpVal <= sumNeighbors * 0.75 && tmpVal > sumNeighbors * 0.45) // expansion, grow rapidly
          gnxt.setCell(x, y, tmpVal + abs(tmpVal) * 0.3);
        else if (tmpVal <= sumNeighbors * 0.45 && tmpVal > 0.0) // subordination, shrink rapidly
          gnxt.setCell(x, y, tmpVal * 0.7);
        else // current cell less than zero -> birth of a new value, if close enough to zero
          gnxt.setCell(x, y, sumNeighbors * 0.5 + tmpVal * 0.5);

        // game of life: // TODO GoL operates with diagonal neighbors
        // 1: underpopulation: cell with less than 2 live neighbors dies
        // 2: cell with 2 or 3 live neighbors continues living
        // 3: overpopulation: cell with more than 3 live neighbors dies
        // 4: reproduction: cell with exactly 3 live neighbors becomes a live cell
      }
    }

//std::cout << std::endl;
    // apply next calculated values to grid object
    g.swapCells(gnxt);
  }

  if (!GridSerializer::serialize(g, outfile)) {
    std::cerr << "Error: Failed to open file for output." << std::endl;
    return 3;
  }

  return 0;
}

