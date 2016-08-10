#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

/**
 * This little file IO tool determines the size of the specified file by jumping to the end of it,
 * then single bytes of data are read over 10 jumps through the file,
 * where 5 jumps occur forward, and 5 jumps occur backwards. */

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    std::cerr << "\tfilename - Path to file which will be accessed." << std::endl;
    return 1;
  }

  std::string filepath = argv[1];

  // first determine file lenght
  std::fstream fhandle;

  fhandle.open(filepath, std::fstream::in);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  unsigned fileLength = 0;
  fhandle.seekg(0, fhandle.end);
  fileLength = fhandle.tellg();
  std::cout << "The file has a size of " << std::to_string(fileLength) << " bytes." << std::endl;

  // ... then jump to beginning ...
  fhandle.seekg(0, fhandle.beg);

  // ... and jump back and forth through the file
  if (fileLength < 10) { // jumping not reasonable
    std::cerr << "File is too small to jump through it." << std::endl;
    return 3;
  }

  unsigned jumpStep = fileLength / 10;

  for (unsigned i = 0; i < 15; ++i) {
    std::streamoff jumpOff = jumpStep;
    if (i < 5) { // jumping 2 steps forward
      jumpOff *= 2;
    }
    else { // jumping 1 step backward
      jumpOff *= -1;
    }
    fhandle.seekg(jumpOff, fhandle.cur);      
    
    // read character at position and output
    unsigned currPos = fhandle.tellg();
    char charAtPos = fhandle.peek();
    std::cout << "Character at position " << std::to_string(currPos) << ": '" << charAtPos << "'." << std::endl;
  }
  fhandle.close();

	return 0;
}

