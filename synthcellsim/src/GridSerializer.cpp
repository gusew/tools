#include "GridSerializer.hpp"
#include "FileHandler.hpp"
#include <fstream>
#include <sstream>

bool GridSerializer::deserialize(const std::string& file, Grid& g) {
  FileHandler fin(file);
  if (!fin.openread()) {
    return false;
  }

  std::fstream& infhandle(fin.handle());

  unsigned cellCnt = 0;
  unsigned cellLimit = g.getSize() * g.getSize();
  double tmpVal;
  std::stringbuf sbuffer;
  GridContainer line;

  // read and store grid data into memory
  while (cellCnt < cellLimit) {
    // check if reading is possible
    if (!infhandle.good()) {
      //std::cerr << "Error: Not enough numbers in input file for specified grid size." << std::endl;
      return false;
    }

    // read next number as string
    infhandle.get(sbuffer, ' ');
    infhandle.ignore(1); // skip delimiter and proceed to next number
//    std::cout << "inbuffer: " << sbuffer.str() << std::endl;

    try { // try to convert to double
      tmpVal = std::stod(sbuffer.str());
    } catch (...) { // not a number
      //std::cerr << "Error: Input data is not a number '" << sbuffer.str() << "'." << std::endl;
      return false; 
    }
    // reset buffer for next number
    sbuffer.str("");
    
    // add number to line container
    line.push_back(tmpVal);
    if (line.size() == g.getSize()) {
      if (!g.fillLine(line)) {
        //std::cerr << "Error: Failed to append another number line to grid." << std::endl;
        return false;
      }
      else
        line.clear();
    }    
    
    ++cellCnt;
  }

  return true;
}

bool GridSerializer::serialize(const Grid& g, const std::string& file) {
  FileHandler fout(file);
  if (!fout.openwrite()) {
    //std::cerr << "Error: Failed to open file for output." << std::endl;
    return false;
  }

  std::fstream& outfhandle(fout.handle());

  // output final grid
  for (unsigned y = 0; y < g.getSize(); ++y) {
    for (unsigned x = 0; x < g.getSize(); ++x) {
      outfhandle << std::to_string(g.getCell(x, y)) << " ";
    }
  }

  return true;
}
