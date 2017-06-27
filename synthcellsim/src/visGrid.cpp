#include <iostream>
#include <string>
#include <sstream>
#include "FileHandler.hpp"
#include "GridSerializer.hpp"
#include "visGrid.hpp"
#include <cstdint>

char conv2Hex(uint8_t val) { 
  val &= 0xF;
  switch (val) {
    case 0xA: return 'A';
    case 0xB: return 'B';
    case 0xC: return 'C';
    case 0xD: return 'D';
    case 0xE: return 'E';
    case 0xF: return 'F';
    default: return val + 48;
  }
}

/**
 * This tool reads a grid file and visualizes the grid by creating a svg graphic object.
 */
int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <N> <infile> <outfile>" << std::endl;
    std::cerr << "\tN - size of grid in one dimension" << std::endl;
    std::cerr << "\tinfile - path to input grid file" << std::endl;
    std::cerr << "\toutfile - path to svg file for output" << std::endl;
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

 
  // open file for output
  FileHandler fout(outfile);
  if (!fout.openwrite()) {
    std::cerr << "Error: Failed to open file for output." << std::endl;
    return 3;
  }

  std::fstream& outfhandle(fout.handle());

  // output svg header structure
  outfhandle << SVG_HEADER << std::endl;

  // calculate dimensions of each cell (2 on each side for margin
  const double CELL_SIZE = (double)(MIN_DIM - 4) / g.getSize();
  
  if (CELL_SIZE <= 0.0) {
    std::cerr << "Error: Grid dimension is too big to visualize." << std::endl;
    return 4;
  }

  // obtain min and max values for adaptive color coding
  double tmpVal = 0.0;
  double statMin = 0.0;
  double statMax = 0.0;

  // iterate over each cell and perform stats calculations
  for (unsigned y = 0; y < paramN; ++y) {
    for (unsigned x = 0; x < paramN; ++x) {
      tmpVal = g.getCell(x, y);
      if (tmpVal > statMax) statMax = tmpVal;
      if (tmpVal < statMin) statMin = tmpVal;
    }
  }

  const double VALRANGE = statMax - statMin;

  // create cell rectangles
  uint8_t colorValue = 0; // R:0, G: (colorValue)_HEX, B: 0
  double posX, posY;

  for (unsigned y = 0; y < paramN; ++y) {
    posY = 2 + y * CELL_SIZE;

    for (unsigned x = 0; x < paramN; ++x) {
      posX = 2 + x * CELL_SIZE;
      colorValue = (uint8_t)( (g.getCell(x, y) - statMin) / VALRANGE * 255); // normalize: (val-min) / (max-min)

      outfhandle << "<rect y=\"" << std::to_string(posY) << "\" x=\"" << std::to_string(posX) << 
        "\" height=\"" << std::to_string(CELL_SIZE) << "\" width=\"" << std::to_string(CELL_SIZE) << 
        "\" id=\"cell" << std::to_string(x) << "_" << std::to_string(y) << 
        "\" style=\"fill:#00" << conv2Hex(colorValue >> 4) << conv2Hex(colorValue) << "00;\" />" << std::endl;
    }
  }

  // output svg footer structure
  outfhandle << SVG_FOOTER << std::endl;

	return 0;
}

