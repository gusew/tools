#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

const unsigned NEWLINE_AT = 100;

/**
 * This little file IO tool first reads all bytes in a specified file and then appends
 * the same number of read bytes to the file at the end. */

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    std::cerr << "\tfilename - Path to file which will be accessed." << std::endl;
    return 1;
  }

  std::string filepath = argv[1];

  // first read file until the end ...
  std::fstream fhandle;

  fhandle.open(filepath, std::fstream::in);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to read from the specified file." << std::endl;
    return 2;
  }

  unsigned haveRead = 0;
  std::string line;

  while (fhandle.good()) {
    if (fhandle.get() != EOF) ++haveRead;
  }
//  std::cout << "Total number of bytes read: " << std::to_string(haveRead) << "." << std::endl;
  fhandle.close();

  // ... then append the number of read bytes at the end
  fhandle.open(filepath, std::fstream::out | std::fstream::app);
  if (!fhandle.is_open()) {
    std::cerr << "Error: Failed to write to the specified file (no permissions?)." << std::endl;
    return 3;
  }

  for (unsigned i = 0; i < haveRead; ++i) {
    if (i % (NEWLINE_AT) == 0) // start always with a newline
      fhandle << std::endl;
    else
      fhandle << "Z";
  }

  fhandle.close();

	return 0;
}

