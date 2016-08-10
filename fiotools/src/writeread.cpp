#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

const unsigned NEWLINE_AT = 100;

/** 
 * This little file IO tool first writes to a specified file a certain number of bytes and
 * then closes and opens the file again and reads the same number of bytes from the file-start. */

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <filename> <numbytes>" << std::endl;
    std::cerr << "\tfilename - Path to file which will be accessed." << std::endl;
    std::cerr << "\tnumbytes - Number of bytes which will be written to that file." << std::endl;
    return 1;
  }

  std::string filepath = argv[1];
  unsigned numbytes = 0;
  try {
    numbytes = std::stoi(argv[2]);    
  } catch (...) { 
    std::cerr << "Error: Number of bytes is specified in an invalid format (not a number)." << std::endl;
    return 2;
  }


  // first write number of bytes to the file ...
  std::fstream fhandle;
  fhandle.open(filepath, std::fstream::out | std::fstream::trunc);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to write to the specified file (no permissions?)." << std::endl;
    return 3;
  }

  for (unsigned i = 0; i < numbytes; ++i) {
    if (i % (NEWLINE_AT + 1) == NEWLINE_AT)
      fhandle << std::endl;
    else
      fhandle << "X";
  }

  fhandle.close();


  // ... then read from the start of the file
  fhandle.open(filepath, std::fstream::in);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 4;
  }

  unsigned haveRead = 0;
  std::string line;

  while (fhandle.good()) {
    std::getline(fhandle, line);
    haveRead += line.length(); 
    if (line.length() == NEWLINE_AT)
      ++haveRead; // one additional char for the newline
  }

  fhandle.close();

  if (haveRead != numbytes && haveRead != (NEWLINE_AT + 1)) {
    std::cerr << "Error: Read " << std::to_string(haveRead) << 
      " bytes instead of " << std::to_string(numbytes) << "." << std::endl;
    return 5;
  }
 
	return 0;
}

